/**************************************************************************/
/*  world_environment.cpp                                                 */
/**************************************************************************/


#include "world_environment.h"
#include "scene/main/viewport.h"

void WorldEnvironment::_notification(int p_what) {
	if (p_what == Node::NOTIFICATION_ENTER_TREE) {
		if (environment.is_valid()) {
			if (get_viewport()->find_world_2d()->get_environment().is_valid()) {
				WARN_PRINT("World 2D already has an environment (Another WorldEnvironment?), overriding.");
			}
			get_viewport()->find_world_2d()->set_environment(environment);
			add_to_group("_world_environment_" + itos(get_viewport()->find_world_2d()->get_scenario().get_id()));
		}

	} else if (p_what == Node::NOTIFICATION_EXIT_TREE) {
		if (environment.is_valid() && get_viewport()->find_world_2d()->get_environment() == environment) {
			get_viewport()->find_world_2d()->set_environment(Ref<Environment>());
			remove_from_group("_world_environment_" + itos(get_viewport()->find_world_2d()->get_scenario().get_id()));
		}
	}
}

void WorldEnvironment::set_environment(const Ref<Environment> &p_environment) {
	if (is_inside_tree() && environment.is_valid() && get_viewport()->find_world_2d()->get_environment() == environment) {
		get_viewport()->find_world_2d()->set_environment(Ref<Environment>());
		remove_from_group("_world_environment_" + itos(get_viewport()->find_world_2d()->get_scenario().get_id()));
		//clean up
	}

	environment = p_environment;
	if (is_inside_tree() && environment.is_valid()) {
		if (get_viewport()->find_world_2d()->get_environment().is_valid()) {
			WARN_PRINT("World 2D already has an environment (Another WorldEnvironment?), overriding.");
		}
		get_viewport()->find_world_2d()->set_environment(environment);
		add_to_group("_world_environment_" + itos(get_viewport()->find_world_2d()->get_scenario().get_id()));
	}

	update_configuration_warning();
}

Ref<Environment> WorldEnvironment::get_environment() const {
	return environment;
}

String WorldEnvironment::get_configuration_warning() const {
	String warning = Node::get_configuration_warning();
	if (!environment.is_valid()) {
		if (warning != String()) {
			warning += "\n\n";
		}
		warning += TTR("WorldEnvironment requires its \"Environment\" property to contain an Environment to have a visible effect.");
		return warning;
	}

	if (/*!is_visible_in_tree() ||*/ !is_inside_tree()) {
		return String();
	}

	List<Node *> nodes;
	get_tree()->get_nodes_in_group("_world_environment_" + itos(get_viewport()->find_world_2d()->get_scenario().get_id()), &nodes);

	if (nodes.size() > 1) {
		if (warning != String()) {
			warning += "\n\n";
		}
		warning += TTR("Only one WorldEnvironment is allowed per scene (or set of instanced scenes).");
	}
	return warning;
}

void WorldEnvironment::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_environment", "env"), &WorldEnvironment::set_environment);
	ClassDB::bind_method(D_METHOD("get_environment"), &WorldEnvironment::get_environment);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment"), "set_environment", "get_environment");
}

WorldEnvironment::WorldEnvironment() {
}
