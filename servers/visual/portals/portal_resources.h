/**************************************************************************/
/*  portal_resources.h                                                    */
/**************************************************************************/


#ifndef PORTAL_RESOURCES_H
#define PORTAL_RESOURCES_H

#include "core/math/geometry.h"
#include "portal_types.h"

// Although the portal renderer is owned by a scenario,
// resources are not associated with a scenario and can be shared
// potentially across multiple scenarios. They must therefore be held in
// some form of global.

class PortalResources {
	friend class PortalRenderer;

public:
	OccluderResourceHandle occluder_resource_create();
	void occluder_resource_prepare(OccluderResourceHandle p_handle, VSOccluder_Instance::Type p_type);
	void occluder_resource_update_spheres(OccluderResourceHandle p_handle, const Vector<Plane> &p_spheres);
	void occluder_resource_update_mesh(OccluderResourceHandle p_handle, const Geometry::OccluderMeshData &p_mesh_data);
	void occluder_resource_destroy(OccluderResourceHandle p_handle);

	const VSOccluder_Resource &get_pool_occluder_resource(uint32_t p_pool_id) const { return _occluder_resource_pool[p_pool_id]; }
	VSOccluder_Resource &get_pool_occluder_resource(uint32_t p_pool_id) { return _occluder_resource_pool[p_pool_id]; }

	// Local space is shared resources
	const VSOccluder_Sphere &get_pool_occluder_local_sphere(uint32_t p_pool_id) const { return _occluder_local_sphere_pool[p_pool_id]; }
	VSOccluder_Hole &get_pool_occluder_local_hole(uint32_t p_pool_id) { return _occluder_local_hole_pool[p_pool_id]; }

private:
	TrackedPooledList<VSOccluder_Resource> _occluder_resource_pool;
	TrackedPooledList<VSOccluder_Sphere, uint32_t, true> _occluder_local_sphere_pool;
	TrackedPooledList<VSOccluder_Poly, uint32_t, true> _occluder_local_poly_pool;
	TrackedPooledList<VSOccluder_Hole, uint32_t, true> _occluder_local_hole_pool;
};

#endif // PORTAL_RESOURCES_H
