/**************************************************************************/
/*  power_iphone.h                                                        */
/**************************************************************************/


#ifndef POWER_IPHONE_H
#define POWER_IPHONE_H

#include <os/os.h>

class PowerIphone {
private:
	int nsecs_left;
	int percent_left;
	OS::PowerState power_state;

	bool UpdatePowerInfo();

public:
	PowerIphone();
	virtual ~PowerIphone();

	OS::PowerState get_power_state();
	int get_power_seconds_left();
	int get_power_percent_left();
};

#endif // POWER_IPHONE_H
