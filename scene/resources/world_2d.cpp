/**************************************************************************/
/*  world_2d.cpp                                                          */
/**************************************************************************/


#include "world_2d.h"

#include "core/project_settings.h"
#include "scene/main/viewport.h"
#include "servers/navigation_2d_server.h"
#include "servers/physics_2d_server.h"
#include "servers/visual_server.h"

struct SpatialIndexer2D {
	struct CellRef {
		int ref;

		_FORCE_INLINE_ int inc() {
			ref++;
			return ref;
		}
		_FORCE_INLINE_ int dec() {
			ref--;
			return ref;
		}

		_FORCE_INLINE_ CellRef() {
			ref = 0;
		}
	};

	struct CellKey {
		union {
			struct {
				int32_t x;
				int32_t y;
			};
			uint64_t key;
		};

		bool operator==(const CellKey &p_key) const { return key == p_key.key; }
		_FORCE_INLINE_ bool operator<(const CellKey &p_key) const {
			return key < p_key.key;
		}
	};

	struct CellData {
	};

	Map<CellKey, CellData> cells;
	int cell_size;

	struct ViewportData {
		Rect2 rect;
	};

	Map<Viewport *, ViewportData> viewports;

	bool changed;

	uint64_t pass;

	void _add_viewport(Viewport *p_viewport, const Rect2 &p_rect) {
		ERR_FAIL_COND(viewports.has(p_viewport));
		ViewportData vd;
		vd.rect = p_rect;
		viewports[p_viewport] = vd;
		changed = true;
	}

	void _update_viewport(Viewport *p_viewport, const Rect2 &p_rect) {
		Map<Viewport *, ViewportData>::Element *E = viewports.find(p_viewport);
		ERR_FAIL_COND(!E);
		if (E->get().rect == p_rect) {
			return;
		}
		E->get().rect = p_rect;
		changed = true;
	}

	void _remove_viewport(Viewport *p_viewport) {
		ERR_FAIL_COND(!viewports.has(p_viewport));
		viewports.erase(p_viewport);
	}

	void _update() {
		if (!changed) {
			return;
		}

		for (Map<Viewport *, ViewportData>::Element *E = viewports.front(); E; E = E->next()) {
			Point2i begin = E->get().rect.position;
			begin /= cell_size;
			Point2i end = E->get().rect.position + E->get().rect.size;
			end /= cell_size;
			pass++;

			uint64_t visible_cells = (uint64_t)(end.x - begin.x) * (uint64_t)(end.y - begin.y);

			if (visible_cells > 10000) {
				//well you zoomed out a lot, it's your problem. To avoid freezing in the for loops below, we'll manually check cell by cell

				for (Map<CellKey, CellData>::Element *F = cells.front(); F; F = F->next()) {
					const CellKey &ck = F->key();

					if (ck.x < begin.x || ck.x > end.x) {
						continue;
					}
					if (ck.y < begin.y || ck.y > end.y) {
						continue;
					}
				}

			} else {
				//check cells in grid fashion
				for (int i = begin.x; i <= end.x; i++) {
					for (int j = begin.y; j <= end.y; j++) {
						CellKey ck;
						ck.x = i;
						ck.y = j;

						Map<CellKey, CellData>::Element *F = cells.find(ck);
						if (!F) {
							continue;
						}
					}
				}
			}
		}

		changed = false;
	}

	SpatialIndexer2D() {
		pass = 0;
		changed = false;
		cell_size = GLOBAL_DEF("world/2d/cell_size", 100);
	}
};

void World2D::_register_viewport(Viewport *p_viewport, const Rect2 &p_rect) {
	indexer->_add_viewport(p_viewport, p_rect);
}

void World2D::_update_viewport(Viewport *p_viewport, const Rect2 &p_rect) {
	indexer->_update_viewport(p_viewport, p_rect);
}
void World2D::_remove_viewport(Viewport *p_viewport) {
	indexer->_remove_viewport(p_viewport);
}

void World2D::_update() {
	indexer->_update();
}

RID World2D::get_canvas() {
	return canvas;
}

RID World2D::get_space() {
	return space;
}

RID World2D::get_scenario() const {
    return scenario;
}

RID World2D::get_navigation_map() const {
	return navigation_map;
}

void World2D::set_environment(const Ref<Environment> &p_environment) {
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

Ref<Environment> World2D::get_environment() const {
    return environment;
}

void World2D::set_fallback_environment(const Ref<Environment> &p_environment) {
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

Ref<Environment> World2D::get_fallback_environment() const {
    return fallback_environment;
}

void World2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_canvas"), &World2D::get_canvas);
	ClassDB::bind_method(D_METHOD("get_space"), &World2D::get_space);
    ClassDB::bind_method(D_METHOD("get_scenario"), &World2D::get_scenario);
    ClassDB::bind_method(D_METHOD("get_navigation_map"), &World2D::get_navigation_map);
    ClassDB::bind_method(D_METHOD("set_environment", "env"), &World2D::set_environment);
    ClassDB::bind_method(D_METHOD("get_environment"), &World2D::get_environment);
    ClassDB::bind_method(D_METHOD("set_fallback_environment", "env"), &World2D::set_fallback_environment);
    ClassDB::bind_method(D_METHOD("get_fallback_environment"), &World2D::get_fallback_environment);

	ClassDB::bind_method(D_METHOD("get_direct_space_state"), &World2D::get_direct_space_state);

	ADD_PROPERTY(PropertyInfo(Variant::_RID, "canvas", PROPERTY_HINT_NONE, "", 0), "", "get_canvas");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment"), "set_environment", "get_environment");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fallback_environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment"), "set_fallback_environment", "get_fallback_environment");
    ADD_PROPERTY(PropertyInfo(Variant::_RID, "space", PROPERTY_HINT_NONE, "", 0), "", "get_space");
    ADD_PROPERTY(PropertyInfo(Variant::_RID, "scenario", PROPERTY_HINT_NONE, "", 0), "", "get_scenario");
	ADD_PROPERTY(PropertyInfo(Variant::_RID, "navigation_map", PROPERTY_HINT_NONE, "", 0), "", "get_navigation_map");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "direct_space_state", PROPERTY_HINT_RESOURCE_TYPE, "Physics2DDirectSpaceState", 0), "", "get_direct_space_state");
}

Physics2DDirectSpaceState *World2D::get_direct_space_state() {
	return Physics2DServer::get_singleton()->space_get_direct_state(space);
}

World2D::World2D() {
    scenario = RID_PRIME(VisualServer::get_singleton()->scenario_create());
	canvas = RID_PRIME(VisualServer::get_singleton()->canvas_create());
	space = RID_PRIME(Physics2DServer::get_singleton()->space_create());

	//set space2D to be more friendly with pixels than meters, by adjusting some constants
	Physics2DServer::get_singleton()->space_set_active(space, true);
	Physics2DServer::get_singleton()->area_set_param(space, Physics2DServer::AREA_PARAM_GRAVITY, GLOBAL_DEF("physics/2d/default_gravity", 98));
	Physics2DServer::get_singleton()->area_set_param(space, Physics2DServer::AREA_PARAM_GRAVITY_VECTOR, GLOBAL_DEF("physics/2d/default_gravity_vector", Vector2(0, 1)));
	Physics2DServer::get_singleton()->area_set_param(space, Physics2DServer::AREA_PARAM_LINEAR_DAMP, GLOBAL_DEF("physics/2d/default_linear_damp", 0.1));
	ProjectSettings::get_singleton()->set_custom_property_info("physics/2d/default_linear_damp", PropertyInfo(Variant::REAL, "physics/2d/default_linear_damp", PROPERTY_HINT_RANGE, "-1,100,0.001,or_greater"));
	Physics2DServer::get_singleton()->area_set_param(space, Physics2DServer::AREA_PARAM_ANGULAR_DAMP, GLOBAL_DEF("physics/2d/default_angular_damp", 1.0));
	ProjectSettings::get_singleton()->set_custom_property_info("physics/2d/default_angular_damp", PropertyInfo(Variant::REAL, "physics/2d/default_angular_damp", PROPERTY_HINT_RANGE, "-1,100,0.001,or_greater"));

	// Create default navigation map
	navigation_map = Navigation2DServer::get_singleton()->map_create();
	Navigation2DServer::get_singleton()->map_set_active(navigation_map, true);
	Navigation2DServer::get_singleton()->map_set_cell_size(navigation_map, GLOBAL_DEF("navigation/2d/default_cell_size", 1.0));
	Navigation2DServer::get_singleton()->map_set_cell_height(navigation_map, GLOBAL_DEF("navigation/2d/default_cell_height", 1.0));
	Navigation2DServer::get_singleton()->map_set_edge_connection_margin(navigation_map, GLOBAL_DEF("navigation/2d/default_edge_connection_margin", 1.0));

	indexer = memnew(SpatialIndexer2D);
}

World2D::~World2D() {
    VisualServer::get_singleton()->free(scenario);
	VisualServer::get_singleton()->free(canvas);
	Physics2DServer::get_singleton()->free(space);
	Navigation2DServer::get_singleton()->free(navigation_map);
	memdelete(indexer);
}
