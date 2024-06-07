/**************************************************************************/
/*  navigation_2d.h                                                       */
/**************************************************************************/


#ifndef NAVIGATION_2D_H
#define NAVIGATION_2D_H

#include "scene/2d/navigation_polygon.h"
#include "scene/2d/node_2d.h"

class Navigation2D : public Node2D {
	GDCLASS(Navigation2D, Node2D);

	RID map;
	real_t cell_size = 1.0;
	real_t edge_connection_margin = 1.0;
	uint32_t navigation_layers = 1;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	RID get_rid() const {
		return map;
	}

	void set_cell_size(float p_cell_size);
	float get_cell_size() const {
		return cell_size;
	}

	void set_navigation_layers(uint32_t p_navigation_layers);
	uint32_t get_navigation_layers() const;

	void set_edge_connection_margin(float p_edge_connection_margin);
	float get_edge_connection_margin() const {
		return edge_connection_margin;
	}

	Vector<Vector2> get_simple_path(const Vector2 &p_start, const Vector2 &p_end, bool p_optimize = true) const;
	Vector2 get_closest_point(const Vector2 &p_point) const;
	RID get_closest_point_owner(const Vector2 &p_point) const;

	virtual String get_configuration_warning() const;

	Navigation2D();
	~Navigation2D();
};

#endif // NAVIGATION_2D_H
