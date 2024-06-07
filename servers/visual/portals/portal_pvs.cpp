/**************************************************************************/
/*  portal_pvs.cpp                                                        */
/**************************************************************************/


#include "portal_pvs.h"

void PVS::clear() {
	_room_pvs.clear();
	_room_secondary_pvs.clear();
	_loaded = false;
}
