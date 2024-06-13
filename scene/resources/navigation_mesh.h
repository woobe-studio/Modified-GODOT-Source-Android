/**************************************************************************/
/*  navigation_mesh.h                                                     */
/**************************************************************************/


#ifndef NAVIGATION_MESH_H
#define NAVIGATION_MESH_H

#include "scene/resources/mesh.h"

class Mesh;

class NavigationMesh : public Resource {
	GDCLASS(NavigationMesh, Resource);

	PoolVector<Vector3> vertices;
	struct Polygon {
		Vector<int> indices;
	};
	Vector<Polygon> polygons;
	Ref<ArrayMesh> debug_mesh;

protected:
	static void _bind_methods();
	virtual void _validate_property(PropertyInfo &property) const;

#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
#endif // DISABLE_DEPRECATED

	void _set_polygons(const Array &p_array);
	Array _get_polygons() const;

public:
	enum SamplePartitionType {
		SAMPLE_PARTITION_WATERSHED = 0,
		SAMPLE_PARTITION_MONOTONE,
		SAMPLE_PARTITION_LAYERS,
		SAMPLE_PARTITION_MAX
	};

	enum ParsedGeometryType {
		PARSED_GEOMETRY_MESH_INSTANCES = 0,
		PARSED_GEOMETRY_STATIC_COLLIDERS,
		PARSED_GEOMETRY_BOTH,
		PARSED_GEOMETRY_MAX
	};

	enum SourceGeometryMode {
		SOURCE_GEOMETRY_NAVMESH_CHILDREN = 0,
		SOURCE_GEOMETRY_GROUPS_WITH_CHILDREN,
		SOURCE_GEOMETRY_GROUPS_EXPLICIT,
		SOURCE_GEOMETRY_MAX
	};

protected:
	float cell_size = 0.25f;
	float cell_height = 0.25f;

	ParsedGeometryType parsed_geometry_type = PARSED_GEOMETRY_MESH_INSTANCES;
	uint32_t collision_mask = 0xFFFFFFFF;

	SourceGeometryMode source_geometry_mode = SOURCE_GEOMETRY_NAVMESH_CHILDREN;
	StringName source_group_name = "navmesh";

public:

	void set_collision_mask(uint32_t p_mask);
	uint32_t get_collision_mask() const;

	void set_collision_mask_bit(int p_bit, bool p_value);
	bool get_collision_mask_bit(int p_bit) const;

	void set_cell_size(float p_value);
	float get_cell_size() const;

	void set_cell_height(float p_value);
	float get_cell_height() const;

	void set_vertices(const PoolVector<Vector3> &p_vertices);
	PoolVector<Vector3> get_vertices() const;

	void add_polygon(const Vector<int> &p_polygon);
	int get_polygon_count() const;
	Vector<int> get_polygon(int p_idx);
	void clear_polygons();

	NavigationMesh();
};

VARIANT_ENUM_CAST(NavigationMesh::SamplePartitionType);
VARIANT_ENUM_CAST(NavigationMesh::ParsedGeometryType);
VARIANT_ENUM_CAST(NavigationMesh::SourceGeometryMode);

#endif // NAVIGATION_MESH_H
