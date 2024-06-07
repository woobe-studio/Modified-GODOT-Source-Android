/**************************************************************************/
/*  power_android.h                                                       */
/**************************************************************************/


#ifndef POWER_ANDROID_H
#define POWER_ANDROID_H

#include "core/os/os.h"

#include <android/native_window_jni.h>

class PowerAndroid {
	struct LocalReferenceHolder {
		JNIEnv *m_env;
		const char *m_func;
	};

private:
	static struct LocalReferenceHolder refs;
	static JNIEnv *env;
	static jmethodID mid;
	static jobject context;
	static jstring action;
	static jclass cls;
	static jobject filter;
	static jobject intent;
	static jstring iname;
	static jmethodID imid;
	static jstring bname;
	static jmethodID bmid;

	int nsecs_left;
	int percent_left;
	OS::PowerState power_state;

	bool GetPowerInfo_Android();

public:
	static int s_active;

	PowerAndroid();
	virtual ~PowerAndroid();

	OS::PowerState get_power_state();
	int get_power_seconds_left();
	int get_power_percent_left();
};

#endif // POWER_ANDROID_H
