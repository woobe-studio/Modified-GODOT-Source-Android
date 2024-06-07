/**************************************************************************/
/*  rvo_agent.h                                                           */
/**************************************************************************/


#ifndef RVO_AGENT_H
#define RVO_AGENT_H

#include "core/object.h"
#include "nav_rid.h"

#include <Agent.h>

class NavMap;

class RvoAgent : public NavRid {
	struct AvoidanceComputedCallback {
		ObjectID id;
		StringName method;
		Variant udata;
		Variant new_velocity;
	};

	NavMap *map = nullptr;
	RVO::Agent agent;
	AvoidanceComputedCallback callback;
	uint32_t map_update_id = 0;

public:
	RvoAgent();

	void set_map(NavMap *p_map);
	NavMap *get_map() {
		return map;
	}

	RVO::Agent *get_agent() {
		return &agent;
	}

	bool is_map_changed();

	void set_callback(ObjectID p_id, const StringName p_method, const Variant p_udata = Variant());
	bool has_callback() const;

	void dispatch_callback();
};

#endif // RVO_AGENT_H
