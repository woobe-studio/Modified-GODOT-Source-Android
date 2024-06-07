/**************************************************************************/
/*  power_uwp.h                                                           */
/**************************************************************************/


#ifndef POWER_UWP_H
#define POWER_UWP_H

#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/os.h"

class PowerUWP {
private:
	int nsecs_left;
	int percent_left;
	OS::PowerState power_state;

	bool UpdatePowerInfo();

public:
	PowerUWP();
	virtual ~PowerUWP();

	OS::PowerState get_power_state();
	int get_power_seconds_left();
	int get_power_percent_left();
};

#endif // POWER_UWP_H
