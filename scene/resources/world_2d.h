/**************************************************************************/
/*  world_2d.h                                                            */
/**************************************************************************/


#ifndef WORLD_2D_H
#define WORLD_2D_H

#include "core/project_settings.h"
#include "core/resource.h"
#include "scene/resources/environment.h"
#include "servers/physics_2d_server.h"

class Viewport;
struct SpatialIndexer2D;

class World2D : public Resource {
	GDCLASS(World2D, Resource);

    RID scenario;
	RID canvas;
	RID space;
	RID navigation_map;

    Ref<Environment> environment;
    Ref<Environment> fallback_environment;

    SpatialIndexer2D *indexer;

protected:
	static void _bind_methods();
	friend class Viewport;

	void _register_viewport(Viewport *p_viewport, const Rect2 &p_rect);
	void _update_viewport(Viewport *p_viewport, const Rect2 &p_rect);
	void _remove_viewport(Viewport *p_viewport);

	void _update();

public:
    RID get_scenario() const;
	RID get_canvas();
	RID get_space();
	RID get_navigation_map() const;

	Physics2DDirectSpaceState *get_direct_space_state();

    void set_fallback_environment(const Ref<Environment> &p_environment);
    Ref<Environment> get_fallback_environment() const;
    void set_environment(const Ref<Environment> &p_environment);
    Ref<Environment> get_environment() const;

	World2D();
	~World2D();
};

#endif // WORLD_2D_H
