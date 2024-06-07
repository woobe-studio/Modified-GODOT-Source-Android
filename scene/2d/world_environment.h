/**************************************************************************/
/*  world_environment.h                                                   */
/**************************************************************************/


#ifndef WORLD_ENVIRONMENT_H
#define WORLD_ENVIRONMENT_H

#include "scene/main/node.h"
#include "scene/main/scene_tree.h"

class WorldEnvironment : public Node {
	GDCLASS(WorldEnvironment, Node);

	Ref<Environment> environment;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_environment(const Ref<Environment> &p_environment);
	Ref<Environment> get_environment() const;

	String get_configuration_warning() const;

	WorldEnvironment();
};

#endif // WORLD_ENVIRONMENT_H
