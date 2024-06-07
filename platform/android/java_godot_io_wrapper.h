/**************************************************************************/
/*  java_godot_io_wrapper.h                                               */
/**************************************************************************/


// note, swapped java and godot around in the file name so all the java
// wrappers are together

#ifndef JAVA_GODOT_IO_WRAPPER_H
#define JAVA_GODOT_IO_WRAPPER_H

#include <android/log.h>
#include <jni.h>

#include "string_android.h"

// Class that makes functions in java/src/org/godotengine/godot/GodotIO.java callable from C++
class GodotIOJavaWrapper {
private:
	jobject godot_io_instance;
	jclass cls;

	jmethodID _open_URI = 0;
	jmethodID _get_cache_dir = 0;
	jmethodID _get_data_dir = 0;
	jmethodID _get_display_cutouts = 0;
	jmethodID _get_locale = 0;
	jmethodID _get_model = 0;
	jmethodID _get_screen_DPI = 0;
	jmethodID _get_scaled_density = 0;
	jmethodID _get_screen_refresh_rate = 0;
	jmethodID _get_window_safe_area = 0;
	jmethodID _get_unique_id = 0;
	jmethodID _show_keyboard = 0;
	jmethodID _hide_keyboard = 0;
	jmethodID _set_screen_orientation = 0;
	jmethodID _get_screen_orientation = 0;
	jmethodID _get_system_dir = 0;

public:
	GodotIOJavaWrapper(JNIEnv *p_env, jobject p_godot_io_instance);
	~GodotIOJavaWrapper();

	jobject get_instance();

	Error open_uri(const String &p_uri);
	String get_cache_dir();
	String get_user_data_dir();
	String get_locale();
	String get_model();
	int get_screen_dpi();
	float get_scaled_density();
	void get_window_safe_area(int (&p_rect_xywh)[4]);
	Array get_display_cutouts();
	float get_screen_refresh_rate(float p_fallback);
	String get_unique_id();
	bool has_vk();
	void show_vk(const String &p_existing, bool p_multiline, int p_max_input_length, int p_cursor_start, int p_cursor_end);
	void hide_vk();
	int get_vk_height();
	void set_vk_height(int p_height);
	void set_screen_orientation(int p_orient);
	int get_screen_orientation() const;
	String get_system_dir(int p_dir, bool p_shared_storage);
};

#endif // JAVA_GODOT_IO_WRAPPER_H
