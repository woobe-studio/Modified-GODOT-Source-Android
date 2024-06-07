/**************************************************************************/
/*  world.cpp                                                             */
/**************************************************************************/


#include "world.h"

#include "core/math/camera_matrix.h"
#include "core/math/octree.h"
#include "scene/scene_string_names.h"
#include "servers/navigation_server.h"

struct SpatialIndexer {

	bool changed;
	uint64_t pass;
	uint64_t last_frame;

	void _update(uint64_t p_frame) {
		if (p_frame == last_frame) {
			return;
		}
		last_frame = p_frame;
		changed = false;
	}

	SpatialIndexer() {
		pass = 0;
		last_frame = 0;
		changed = false;
	}
};

void World::_update(uint64_t p_frame) {
#ifndef _3D_DISABLED
	indexer->_update(p_frame);
#endif
}

RID World::get_space() const {
	return space;
}

RID World::get_scenario() const {
	return scenario;
}

RID World::get_navigation_map() const {
	return navigation_map;
}

void World::set_environment(const Ref<Environment> &p_environment) {
	if (environment == p_environment) {
		return;
	}

	environment = p_environment;
	if (environment.is_valid()) {
		VS::get_singleton()->scenario_set_environment(scenario, environment->get_rid());
	} else {
		VS::get_singleton()->scenario_set_environment(scenario, RID());
	}

	emit_changed();
}

Ref<Environment> World::get_environment() const {
	return environment;
}

void World::set_fallback_environment(const Ref<Environment> &p_environment) {
	if (fallback_environment == p_environment) {
		return;
	}

	fallback_environment = p_environment;
	if (fallback_environment.is_valid()) {
		VS::get_singleton()->scenario_set_fallback_environment(scenario, p_environment->get_rid());
	} else {
		VS::get_singleton()->scenario_set_fallback_environment(scenario, RID());
	}

	emit_changed();
}

Ref<Environment> World::get_fallback_environment() const {
	return fallback_environment;
}

void World::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_space"), &World::get_space);
	ClassDB::bind_method(D_METHOD("get_scenario"), &World::get_scenario);
	ClassDB::bind_method(D_METHOD("get_navigation_map"), &World::get_navigation_map);
	ClassDB::bind_method(D_METHOD("set_environment", "env"), &World::set_environment);
	ClassDB::bind_method(D_METHOD("get_environment"), &World::get_environment);
	ClassDB::bind_method(D_METHOD("set_fallback_environment", "env"), &World::set_fallback_environment);
	ClassDB::bind_method(D_METHOD("get_fallback_environment"), &World::get_fallback_environment);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment"), "set_environment", "get_environment");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fallback_environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment"), "set_fallback_environment", "get_fallback_environment");
	ADD_PROPERTY(PropertyInfo(Variant::_RID, "space", PROPERTY_HINT_NONE, "", 0), "", "get_space");
	ADD_PROPERTY(PropertyInfo(Variant::_RID, "scenario", PROPERTY_HINT_NONE, "", 0), "", "get_scenario");
	ADD_PROPERTY(PropertyInfo(Variant::_RID, "navigation_map", PROPERTY_HINT_NONE, "", 0), "", "get_navigation_map");
}

World::World() {
	scenario = RID_PRIME(VisualServer::get_singleton()->scenario_create());

	// Create default navigation map
	navigation_map = NavigationServer::get_singleton()->map_create();
	NavigationServer::get_singleton()->map_set_active(navigation_map, true);
	NavigationServer::get_singleton()->map_set_up(navigation_map, GLOBAL_DEF("navigation/3d/default_map_up", Vector3(0, 1, 0)));
	NavigationServer::get_singleton()->map_set_cell_size(navigation_map, GLOBAL_DEF("navigation/3d/default_cell_size", 0.25));
	NavigationServer::get_singleton()->map_set_cell_height(navigation_map, GLOBAL_DEF("navigation/3d/default_cell_height", 0.25));
	NavigationServer::get_singleton()->map_set_edge_connection_margin(navigation_map, GLOBAL_DEF("navigation/3d/default_edge_connection_margin", 0.25));

#ifdef _3D_DISABLED
	indexer = NULL;
#else
	indexer = memnew(SpatialIndexer);
#endif
}

World::~World() {
	VisualServer::get_singleton()->free(scenario);
	NavigationServer::get_singleton()->free(navigation_map);

#ifndef _3D_DISABLED
	memdelete(indexer);
#endif
}
