/**************************************************************************/
/*  world.h                                                               */
/**************************************************************************/


#ifndef WORLD_H
#define WORLD_H

#include "core/resource.h"
#include "scene/resources/environment.h"
#include "servers/visual_server.h"

struct SpatialIndexer;

class World : public Resource {
	GDCLASS(World, Resource);
	RES_BASE_EXTENSION("world");

private:
	RID space;
	RID scenario;
	RID navigation_map;

	SpatialIndexer *indexer;
	Ref<Environment> environment;
	Ref<Environment> fallback_environment;

protected:
	static void _bind_methods();

	friend class Viewport;
	void _update(uint64_t p_frame);

public:
	RID get_space() const;
	RID get_scenario() const;
	RID get_navigation_map() const;

	void set_environment(const Ref<Environment> &p_environment);
	Ref<Environment> get_environment() const;

	void set_fallback_environment(const Ref<Environment> &p_environment);
	Ref<Environment> get_fallback_environment() const;

	World();
	~World();
};

#endif // WORLD_H
