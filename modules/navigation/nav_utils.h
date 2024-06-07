/**************************************************************************/
/*  nav_utils.h                                                           */
/**************************************************************************/


#ifndef NAV_UTILS_H
#define NAV_UTILS_H

#include "core/math/vector3.h"

#include <vector>

class NavRegion;

namespace gd {
struct Polygon;

union PointKey {
	struct {
		int64_t x : 21;
		int64_t y : 22;
		int64_t z : 21;
	};

	uint64_t key = 0;
};

struct EdgeKey {
	PointKey a;
	PointKey b;

	bool operator<(const EdgeKey &p_key) const {
		return (a.key == p_key.a.key) ? (b.key < p_key.b.key) : (a.key < p_key.a.key);
	}

	EdgeKey(const PointKey &p_a = PointKey(), const PointKey &p_b = PointKey()) :
			a(p_a),
			b(p_b) {
		if (a.key > b.key) {
			SWAP(a, b);
		}
	}
};

struct Point {
	Vector3 pos;
	PointKey key;
};

struct Edge {
	/// This edge ID
	int this_edge = -1;

	/// The gateway in the edge, as, in some case, the whole edge might not be navigable.
	struct Connection {
		Polygon *polygon = nullptr;
		int edge = -1;
		Vector3 pathway_start;
		Vector3 pathway_end;
	};
	Vector<Connection> connections;
};

struct Polygon {
	NavRegion *owner = nullptr;

	/// The points of this `Polygon`
	LocalVector<Point> points;

	/// Are the points clockwise ?
	bool clockwise;

	/// The edges of this `Polygon`
	LocalVector<Edge> edges;

	/// The center of this `Polygon`
	Vector3 center;
};

struct NavigationPoly {
	uint32_t self_id = 0;
	/// This poly.
	const Polygon *poly;

	/// Those 4 variables are used to travel the path backwards.
	int back_navigation_poly_id = -1;
	uint32_t back_navigation_edge = UINT32_MAX;
	Vector3 back_navigation_edge_pathway_start;
	Vector3 back_navigation_edge_pathway_end;

	/// The entry location of this poly.
	Vector3 entry;
	/// The distance to the destination.
	float traveled_distance = 0.0;

	NavigationPoly() { poly = nullptr; }

	NavigationPoly(const Polygon *p_poly) :
			poly(p_poly) {}

	bool operator==(const NavigationPoly &other) const {
		return this->poly == other.poly;
	}

	bool operator!=(const NavigationPoly &other) const {
		return !operator==(other);
	}
};

struct ClosestPointQueryResult {
	Vector3 point;
	Vector3 normal;
	RID owner;
};

} // namespace gd

#endif // NAV_UTILS_H
