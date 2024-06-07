/**************************************************************************/
/*  power_iphone.cpp                                                      */
/**************************************************************************/


#include "power_iphone.h"

bool PowerIphone::UpdatePowerInfo() {
	return false;
}

OS::PowerState PowerIphone::get_power_state() {
	if (UpdatePowerInfo()) {
		return power_state;
	} else {
		return OS::POWERSTATE_UNKNOWN;
	}
}

int PowerIphone::get_power_seconds_left() {
	if (UpdatePowerInfo()) {
		return nsecs_left;
	} else {
		return -1;
	}
}

int PowerIphone::get_power_percent_left() {
	if (UpdatePowerInfo()) {
		return percent_left;
	} else {
		return -1;
	}
}

PowerIphone::PowerIphone() :
		nsecs_left(-1),
		percent_left(-1),
		power_state(OS::POWERSTATE_UNKNOWN) {
	// TODO Auto-generated constructor stub
}

PowerIphone::~PowerIphone() {
	// TODO Auto-generated destructor stub
}
