/**************************************************************************/
/*  geometry.cpp                                                          */
/**************************************************************************/


#include "geometry.h"

#include "core/local_vector.h"
#include "core/print_string.h"

#include "thirdparty/misc/clipper.hpp"
#include "thirdparty/misc/triangulator.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define SCALE_FACTOR 100000.0 // Based on CMP_EPSILON.

// This implementation is very inefficient, commenting unless bugs happen. See the other one.
/*
bool Geometry::is_point_in_polygon(const Vector2 &p_point, const Vector<Vector2> &p_polygon) {

	Vector<int> indices = Geometry::triangulate_polygon(p_polygon);
	for (int j = 0; j + 3 <= indices.size(); j += 3) {
		int i1 = indices[j], i2 = indices[j + 1], i3 = indices[j + 2];
		if (Geometry::is_point_in_triangle(p_point, p_polygon[i1], p_polygon[i2], p_polygon[i3]))
			return true;
	}
	return false;
}
*/

void Geometry::OccluderMeshData::clear() {
	faces.clear();
	vertices.clear();
}

void Geometry::MeshData::clear() {
	faces.clear();
	edges.clear();
	vertices.clear();
}

struct _FaceClassify {
	struct _Link {
		int face;
		int edge;
		void clear() {
			face = -1;
			edge = -1;
		}
		_Link() {
			face = -1;
			edge = -1;
		}
	};
	bool valid;
	int group;
	_Link links[3];
	Face3 face;
	_FaceClassify() {
		group = -1;
		valid = false;
	};
};

static bool _group_face(_FaceClassify *p_faces, int len, int p_index, int p_group) {
	if (p_faces[p_index].group >= 0) {
		return false;
	}

	p_faces[p_index].group = p_group;

	for (int i = 0; i < 3; i++) {
		ERR_FAIL_INDEX_V(p_faces[p_index].links[i].face, len, true);
		_group_face(p_faces, len, p_faces[p_index].links[i].face, p_group);
	}

	return true;
}

/*** GEOMETRY WRAPPER ***/

enum _CellFlags {

	_CELL_SOLID = 1,
	_CELL_EXTERIOR = 2,
	_CELL_STEP_MASK = 0x1C,
	_CELL_STEP_NONE = 0 << 2,
	_CELL_STEP_Y_POS = 1 << 2,
	_CELL_STEP_Y_NEG = 2 << 2,
	_CELL_STEP_X_POS = 3 << 2,
	_CELL_STEP_X_NEG = 4 << 2,
	_CELL_STEP_Z_POS = 5 << 2,
	_CELL_STEP_Z_NEG = 6 << 2,
	_CELL_STEP_DONE = 7 << 2,
	_CELL_PREV_MASK = 0xE0,
	_CELL_PREV_NONE = 0 << 5,
	_CELL_PREV_Y_POS = 1 << 5,
	_CELL_PREV_Y_NEG = 2 << 5,
	_CELL_PREV_X_POS = 3 << 5,
	_CELL_PREV_X_NEG = 4 << 5,
	_CELL_PREV_Z_POS = 5 << 5,
	_CELL_PREV_Z_NEG = 6 << 5,
	_CELL_PREV_FIRST = 7 << 5,

};

static inline void _plot_face(uint8_t ***p_cell_status, int x, int y, int z, int len_x, int len_y, int len_z, const Vector3 &voxelsize, const Face3 &p_face) {
	AABB aabb(Vector3(x, y, z), Vector3(len_x, len_y, len_z));
	aabb.position = aabb.position * voxelsize;
	aabb.size = aabb.size * voxelsize;

	if (!p_face.intersects_aabb(aabb)) {
		return;
	}

	if (len_x == 1 && len_y == 1 && len_z == 1) {
		p_cell_status[x][y][z] = _CELL_SOLID;
		return;
	}

	int div_x = len_x > 1 ? 2 : 1;
	int div_y = len_y > 1 ? 2 : 1;
	int div_z = len_z > 1 ? 2 : 1;

#define _SPLIT(m_i, m_div, m_v, m_len_v, m_new_v, m_new_len_v) \
	if (m_div == 1) {                                          \
		m_new_v = m_v;                                         \
		m_new_len_v = 1;                                       \
	} else if (m_i == 0) {                                     \
		m_new_v = m_v;                                         \
		m_new_len_v = m_len_v / 2;                             \
	} else {                                                   \
		m_new_v = m_v + m_len_v / 2;                           \
		m_new_len_v = m_len_v - m_len_v / 2;                   \
	}

	int new_x;
	int new_len_x;
	int new_y;
	int new_len_y;
	int new_z;
	int new_len_z;

	for (int i = 0; i < div_x; i++) {
		_SPLIT(i, div_x, x, len_x, new_x, new_len_x);

		for (int j = 0; j < div_y; j++) {
			_SPLIT(j, div_y, y, len_y, new_y, new_len_y);

			for (int k = 0; k < div_z; k++) {
				_SPLIT(k, div_z, z, len_z, new_z, new_len_z);

				_plot_face(p_cell_status, new_x, new_y, new_z, new_len_x, new_len_y, new_len_z, voxelsize, p_face);
			}
		}
	}
}

Vector<Vector<Vector2>> Geometry::decompose_polygon_in_convex(Vector<Point2> polygon) {
	Vector<Vector<Vector2>> decomp;
	List<TriangulatorPoly> in_poly, out_poly;

	TriangulatorPoly inp;
	inp.Init(polygon.size());
	for (int i = 0; i < polygon.size(); i++) {
		inp.GetPoint(i) = polygon[i];
	}
	inp.SetOrientation(TRIANGULATOR_CCW);
	in_poly.push_back(inp);
	TriangulatorPartition tpart;
	if (tpart.ConvexPartition_HM(&in_poly, &out_poly) == 0) { // Failed.
		ERR_PRINT("Convex decomposing failed!");
		return decomp;
	}

	decomp.resize(out_poly.size());
	int idx = 0;
	for (List<TriangulatorPoly>::Element *I = out_poly.front(); I; I = I->next()) {
		TriangulatorPoly &tp = I->get();

		decomp.write[idx].resize(tp.GetNumPoints());

		for (int64_t i = 0; i < tp.GetNumPoints(); i++) {
			decomp.write[idx].write[i] = tp.GetPoint(i);
		}

		idx++;
	}

	return decomp;
}

Geometry::MeshData Geometry::build_convex_mesh(const PoolVector<Plane> &p_planes) {
	MeshData mesh;

#define SUBPLANE_SIZE 1024.0

	real_t subplane_size = 1024.0; // Should compute this from the actual plane.
	for (int i = 0; i < p_planes.size(); i++) {
		Plane p = p_planes[i];

		Vector3 ref = Vector3(0.0, 1.0, 0.0);

		if (ABS(p.normal.dot(ref)) > 0.95f) {
			ref = Vector3(0.0, 0.0, 1.0); // Change axis.
		}

		Vector3 right = p.normal.cross(ref).normalized();
		Vector3 up = p.normal.cross(right).normalized();

		Vector<Vector3> vertices;

		Vector3 center = p.get_any_point();
		// make a quad clockwise
		vertices.push_back(center - up * subplane_size + right * subplane_size);
		vertices.push_back(center - up * subplane_size - right * subplane_size);
		vertices.push_back(center + up * subplane_size - right * subplane_size);
		vertices.push_back(center + up * subplane_size + right * subplane_size);

		for (int j = 0; j < p_planes.size(); j++) {
			if (j == i) {
				continue;
			}

			Vector<Vector3> new_vertices;
			Plane clip = p_planes[j];

			if (clip.normal.dot(p.normal) > 0.95f) {
				continue;
			}

			if (vertices.size() < 3) {
				break;
			}

			for (int k = 0; k < vertices.size(); k++) {
				int k_n = (k + 1) % vertices.size();

				Vector3 edge0_A = vertices[k];
				Vector3 edge1_A = vertices[k_n];

				real_t dist0 = clip.distance_to(edge0_A);
				real_t dist1 = clip.distance_to(edge1_A);

				if (dist0 <= 0) { // Behind plane.

					new_vertices.push_back(vertices[k]);
				}

				// Check for different sides and non coplanar.
				if ((dist0 * dist1) < 0) {
					// Calculate intersection.
					Vector3 rel = edge1_A - edge0_A;

					real_t den = clip.normal.dot(rel);
					if (Math::is_zero_approx(den)) {
						continue; // Point too short.
					}

					real_t dist = -(clip.normal.dot(edge0_A) - clip.d) / den;
					Vector3 inters = edge0_A + rel * dist;
					new_vertices.push_back(inters);
				}
			}

			vertices = new_vertices;
		}

		if (vertices.size() < 3) {
			continue;
		}

		// Result is a clockwise face.

		MeshData::Face face;

		// Add face indices.
		for (int j = 0; j < vertices.size(); j++) {
			int idx = -1;
			for (int k = 0; k < mesh.vertices.size(); k++) {
				if (mesh.vertices[k].distance_to(vertices[j]) < 0.001f) {
					idx = k;
					break;
				}
			}

			if (idx == -1) {
				idx = mesh.vertices.size();
				mesh.vertices.push_back(vertices[j]);
			}

			face.indices.push_back(idx);
		}
		face.plane = p;
		mesh.faces.push_back(face);

		// Add edge.

		for (int j = 0; j < face.indices.size(); j++) {
			int a = face.indices[j];
			int b = face.indices[(j + 1) % face.indices.size()];

			bool found = false;
			for (int k = 0; k < mesh.edges.size(); k++) {
				if (mesh.edges[k].a == a && mesh.edges[k].b == b) {
					found = true;
					break;
				}
				if (mesh.edges[k].b == a && mesh.edges[k].a == b) {
					found = true;
					break;
				}
			}

			if (found) {
				continue;
			}
			MeshData::Edge edge;
			edge.a = a;
			edge.b = b;
			mesh.edges.push_back(edge);
		}
	}

	return mesh;
}

PoolVector<Plane> Geometry::build_box_planes(const Vector3 &p_extents) {
	PoolVector<Plane> planes;

	planes.push_back(Plane(Vector3(1, 0, 0), p_extents.x));
	planes.push_back(Plane(Vector3(-1, 0, 0), p_extents.x));
	planes.push_back(Plane(Vector3(0, 1, 0), p_extents.y));
	planes.push_back(Plane(Vector3(0, -1, 0), p_extents.y));
	planes.push_back(Plane(Vector3(0, 0, 1), p_extents.z));
	planes.push_back(Plane(Vector3(0, 0, -1), p_extents.z));

	return planes;
}

PoolVector<Plane> Geometry::build_cylinder_planes(real_t p_radius, real_t p_height, int p_sides, Vector3::Axis p_axis) {
	ERR_FAIL_INDEX_V(p_axis, 3, PoolVector<Plane>());

	PoolVector<Plane> planes;

	for (int i = 0; i < p_sides; i++) {
		Vector3 normal;
		normal[(p_axis + 1) % 3] = Math::cos(i * (real_t)(2.0 * Math_PI) / p_sides);
		normal[(p_axis + 2) % 3] = Math::sin(i * (real_t)(2.0 * Math_PI) / p_sides);

		planes.push_back(Plane(normal, p_radius));
	}

	Vector3 axis;
	axis[p_axis] = 1.0;

	planes.push_back(Plane(axis, p_height * 0.5f));
	planes.push_back(Plane(-axis, p_height * 0.5f));

	return planes;
}

PoolVector<Plane> Geometry::build_capsule_planes(real_t p_radius, real_t p_height, int p_sides, int p_lats, Vector3::Axis p_axis) {
	ERR_FAIL_INDEX_V(p_axis, 3, PoolVector<Plane>());

	PoolVector<Plane> planes;

	Vector3 axis;
	axis[p_axis] = 1;

	Vector3 axis_neg;
	axis_neg[(p_axis + 1) % 3] = 1;
	axis_neg[(p_axis + 2) % 3] = 1;
	axis_neg[p_axis] = -1;

	for (int i = 0; i < p_sides; i++) {
		Vector3 normal;
		normal[(p_axis + 1) % 3] = Math::cos(i * (real_t)(2.0 * Math_PI) / p_sides);
		normal[(p_axis + 2) % 3] = Math::sin(i * (real_t)(2.0 * Math_PI) / p_sides);

		planes.push_back(Plane(normal, p_radius));

		for (int j = 1; j <= p_lats; j++) {
			Vector3 angle = normal.linear_interpolate(axis, j / (real_t)p_lats).normalized();
			Vector3 pos = axis * p_height * 0.5f + angle * p_radius;
			planes.push_back(Plane(pos, angle));
			planes.push_back(Plane(pos * axis_neg, angle * axis_neg));
		}
	}

	return planes;
}

struct _AtlasWorkRect {
	Size2i s;
	Point2i p;
	int idx;
	_FORCE_INLINE_ bool operator<(const _AtlasWorkRect &p_r) const { return s.width > p_r.s.width; }
};

struct _AtlasWorkRectResult {
	Vector<_AtlasWorkRect> result;
	int max_w;
	int max_h;
};

void Geometry::make_atlas(const Vector<Size2i> &p_rects, Vector<Point2i> &r_result, Size2i &r_size) {
	// Super simple, almost brute force scanline stacking fitter.
	// It's pretty basic for now, but it tries to make sure that the aspect ratio of the
	// resulting atlas is somehow square. This is necessary because video cards have limits.
	// On texture size (usually 2048 or 4096), so the more square a texture, the more chances.
	// It will work in every hardware.
	// For example, it will prioritize a 1024x1024 atlas (works everywhere) instead of a
	// 256x8192 atlas (won't work anywhere).

	ERR_FAIL_COND(p_rects.size() == 0);
	for (int i = 0; i < p_rects.size(); i++) {
		ERR_FAIL_COND(p_rects[i].width <= 0);
		ERR_FAIL_COND(p_rects[i].height <= 0);
	}

	Vector<_AtlasWorkRect> wrects;
	wrects.resize(p_rects.size());
	for (int i = 0; i < p_rects.size(); i++) {
		wrects.write[i].s = p_rects[i];
		wrects.write[i].idx = i;
	}
	wrects.sort();
	int widest = wrects[0].s.width;

	Vector<_AtlasWorkRectResult> results;

	for (int i = 0; i <= 12; i++) {
		int w = 1 << i;
		int max_h = 0;
		int max_w = 0;
		if (w < widest) {
			continue;
		}

		Vector<int> hmax;
		hmax.resize(w);
		for (int j = 0; j < w; j++) {
			hmax.write[j] = 0;
		}

		// Place them.
		int ofs = 0;
		int limit_h = 0;
		for (int j = 0; j < wrects.size(); j++) {
			if (ofs + wrects[j].s.width > w) {
				ofs = 0;
			}

			int from_y = 0;
			for (int k = 0; k < wrects[j].s.width; k++) {
				if (hmax[ofs + k] > from_y) {
					from_y = hmax[ofs + k];
				}
			}

			wrects.write[j].p.x = ofs;
			wrects.write[j].p.y = from_y;
			int end_h = from_y + wrects[j].s.height;
			int end_w = ofs + wrects[j].s.width;
			if (ofs == 0) {
				limit_h = end_h;
			}

			for (int k = 0; k < wrects[j].s.width; k++) {
				hmax.write[ofs + k] = end_h;
			}

			if (end_h > max_h) {
				max_h = end_h;
			}

			if (end_w > max_w) {
				max_w = end_w;
			}

			if (ofs == 0 || end_h > limit_h) { // While h limit not reached, keep stacking.
				ofs += wrects[j].s.width;
			}
		}

		_AtlasWorkRectResult result;
		result.result = wrects;
		result.max_h = max_h;
		result.max_w = max_w;
		results.push_back(result);
	}

	// Find the result with the best aspect ratio.

	int best = -1;
	real_t best_aspect = 1e20;

	for (int i = 0; i < results.size(); i++) {
		real_t h = next_power_of_2(results[i].max_h);
		real_t w = next_power_of_2(results[i].max_w);
		real_t aspect = h > w ? h / w : w / h;
		if (aspect < best_aspect) {
			best = i;
			best_aspect = aspect;
		}
	}

	r_result.resize(p_rects.size());

	for (int i = 0; i < p_rects.size(); i++) {
		r_result.write[results[best].result[i].idx] = results[best].result[i].p;
	}

	r_size = Size2(results[best].max_w, results[best].max_h);
}

Vector<Vector<Point2>> Geometry::_polypaths_do_operation(PolyBooleanOperation p_op, const Vector<Point2> &p_polypath_a, const Vector<Point2> &p_polypath_b, bool is_a_open) {
	using namespace ClipperLib;

	ClipType op = ctUnion;

	switch (p_op) {
		case OPERATION_UNION:
			op = ctUnion;
			break;
		case OPERATION_DIFFERENCE:
			op = ctDifference;
			break;
		case OPERATION_INTERSECTION:
			op = ctIntersection;
			break;
		case OPERATION_XOR:
			op = ctXor;
			break;
	}
	Path path_a, path_b;

	// Need to scale points (Clipper's requirement for robust computation).
	for (int i = 0; i != p_polypath_a.size(); ++i) {
		path_a << IntPoint(p_polypath_a[i].x * (real_t)SCALE_FACTOR, p_polypath_a[i].y * (real_t)SCALE_FACTOR);
	}
	for (int i = 0; i != p_polypath_b.size(); ++i) {
		path_b << IntPoint(p_polypath_b[i].x * (real_t)SCALE_FACTOR, p_polypath_b[i].y * (real_t)SCALE_FACTOR);
	}
	Clipper clp;
	clp.AddPath(path_a, ptSubject, !is_a_open); // Forward compatible with Clipper 10.0.0.
	clp.AddPath(path_b, ptClip, true); // Polylines cannot be set as clip.

	Paths paths;

	if (is_a_open) {
		PolyTree tree; // Needed to populate polylines.
		clp.Execute(op, tree);
		OpenPathsFromPolyTree(tree, paths);
	} else {
		clp.Execute(op, paths); // Works on closed polygons only.
	}
	// Have to scale points down now.
	Vector<Vector<Point2>> polypaths;

	for (Paths::size_type i = 0; i < paths.size(); ++i) {
		Vector<Vector2> polypath;

		const Path &scaled_path = paths[i];

		for (Paths::size_type j = 0; j < scaled_path.size(); ++j) {
			polypath.push_back(Point2(
					static_cast<real_t>(scaled_path[j].X) / (real_t)SCALE_FACTOR,
					static_cast<real_t>(scaled_path[j].Y) / (real_t)SCALE_FACTOR));
		}
		polypaths.push_back(polypath);
	}
	return polypaths;
}

Vector<Vector<Point2>> Geometry::_polypath_offset(const Vector<Point2> &p_polypath, real_t p_delta, PolyJoinType p_join_type, PolyEndType p_end_type) {
	using namespace ClipperLib;

	JoinType jt = jtSquare;

	switch (p_join_type) {
		case JOIN_SQUARE:
			jt = jtSquare;
			break;
		case JOIN_ROUND:
			jt = jtRound;
			break;
		case JOIN_MITER:
			jt = jtMiter;
			break;
	}

	EndType et = etClosedPolygon;

	switch (p_end_type) {
		case END_POLYGON:
			et = etClosedPolygon;
			break;
		case END_JOINED:
			et = etClosedLine;
			break;
		case END_BUTT:
			et = etOpenButt;
			break;
		case END_SQUARE:
			et = etOpenSquare;
			break;
		case END_ROUND:
			et = etOpenRound;
			break;
	}
	ClipperOffset co(2.0f, 0.25f * (real_t)SCALE_FACTOR); // Defaults from ClipperOffset.
	Path path;

	// Need to scale points (Clipper's requirement for robust computation).
	for (int i = 0; i != p_polypath.size(); ++i) {
		path << IntPoint(p_polypath[i].x * (real_t)SCALE_FACTOR, p_polypath[i].y * (real_t)SCALE_FACTOR);
	}
	co.AddPath(path, jt, et);

	Paths paths;
	co.Execute(paths, p_delta * (real_t)SCALE_FACTOR); // Inflate/deflate.

	// Have to scale points down now.
	Vector<Vector<Point2>> polypaths;

	for (Paths::size_type i = 0; i < paths.size(); ++i) {
		Vector<Vector2> polypath;

		const Path &scaled_path = paths[i];

		for (Paths::size_type j = 0; j < scaled_path.size(); ++j) {
			polypath.push_back(Point2(
					static_cast<real_t>(scaled_path[j].X) / (real_t)SCALE_FACTOR,
					static_cast<real_t>(scaled_path[j].Y) / (real_t)SCALE_FACTOR));
		}
		polypaths.push_back(polypath);
	}
	return polypaths;
}

// note this function is slow, because it builds meshes etc. Not ideal to use in realtime.
// Planes must face OUTWARD from the center of the convex hull, by convention.

Vector<Vector3> Geometry::compute_convex_mesh_points(const Plane *p_planes, int p_plane_count, real_t p_epsilon) {
	Vector<Vector3> points;

	// Iterate through every unique combination of any three planes.
	for (int i = p_plane_count - 1; i >= 0; i--) {
		for (int j = i - 1; j >= 0; j--) {
			for (int k = j - 1; k >= 0; k--) {
				// Find the point where these planes all cross over (if they
				// do at all).
				Vector3 convex_shape_point;
				if (p_planes[i].intersect_3(p_planes[j], p_planes[k], &convex_shape_point)) {
					// See if any *other* plane excludes this point because it's
					// on the wrong side.
					bool excluded = false;
					for (int n = 0; n < p_plane_count; n++) {
						if (n != i && n != j && n != k) {
							real_t dist = p_planes[n].distance_to(convex_shape_point);
							if (dist > p_epsilon) {
								excluded = true;
								break;
							}
						}
					}

					// Only add the point if it passed all tests.
					if (!excluded) {
						points.push_back(convex_shape_point);
					}
				}
			}
		}
	}

	return points;
}

