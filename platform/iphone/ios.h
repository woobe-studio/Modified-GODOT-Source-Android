/**************************************************************************/
/*  ios.h                                                                 */
/**************************************************************************/


#ifndef IOS_H
#define IOS_H

#include "core/object.h"
#import <CoreHaptics/CoreHaptics.h>

class iOS : public Object {
	GDCLASS(iOS, Object);

	static void _bind_methods();

private:
	CHHapticEngine *haptic_engine API_AVAILABLE(ios(13)) = NULL;

	CHHapticEngine *get_haptic_engine_instance() API_AVAILABLE(ios(13));
	void start_haptic_engine();
	void stop_haptic_engine();

public:
	static void alert(const char *p_alert, const char *p_title);

	bool supports_haptic_engine();
	void vibrate_haptic_engine(float p_duration_seconds);

	String get_model() const;
	String get_rate_url(int p_app_id) const;

	iOS();
};

#endif // IOS_H
