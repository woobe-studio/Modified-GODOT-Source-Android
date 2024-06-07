/**************************************************************************/
/*  register_types.cpp                                                    */
/**************************************************************************/


#include "register_types.h"

#include "core/engine.h"
#include "godot_navigation_server.h"
#include "servers/navigation_server.h"

#ifndef _3D_DISABLED
#endif

#ifdef TOOLS_ENABLED
#endif

NavigationServer *new_server() {
	return memnew(GodotNavigationServer);
}

void register_navigation_types() {
	NavigationServerManager::set_default_server(new_server);

#ifndef _3D_DISABLED
#endif

#ifdef TOOLS_ENABLED
#endif
}

void unregister_navigation_types() {
#ifndef _3D_DISABLED
#endif
}
