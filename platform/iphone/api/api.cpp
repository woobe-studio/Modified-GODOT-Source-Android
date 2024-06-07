/**************************************************************************/
/*  api.cpp                                                               */
/**************************************************************************/


#include "api.h"

#if defined(IPHONE_ENABLED)

void register_iphone_api() {
	godot_ios_plugins_initialize();
}

void unregister_iphone_api() {
	godot_ios_plugins_deinitialize();
}

#else

void register_iphone_api() {}
void unregister_iphone_api() {}

#endif
