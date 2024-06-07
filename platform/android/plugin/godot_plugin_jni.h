/**************************************************************************/
/*  godot_plugin_jni.h                                                    */
/**************************************************************************/


#ifndef GODOT_PLUGIN_JNI_H
#define GODOT_PLUGIN_JNI_H

#include <android/log.h>
#include <jni.h>

extern "C" {
JNIEXPORT void JNICALL Java_org_godotengine_godot_plugin_GodotPlugin_nativeRegisterSingleton(JNIEnv *env, jclass clazz, jstring name, jobject obj);
JNIEXPORT void JNICALL Java_org_godotengine_godot_plugin_GodotPlugin_nativeRegisterMethod(JNIEnv *env, jclass clazz, jstring sname, jstring name, jstring ret, jobjectArray args);
JNIEXPORT void JNICALL Java_org_godotengine_godot_plugin_GodotPlugin_nativeRegisterSignal(JNIEnv *env, jclass clazz, jstring j_plugin_name, jstring j_signal_name, jobjectArray j_signal_param_types);
JNIEXPORT void JNICALL Java_org_godotengine_godot_plugin_GodotPlugin_nativeEmitSignal(JNIEnv *env, jclass clazz, jstring j_plugin_name, jstring j_signal_name, jobjectArray j_signal_params);
JNIEXPORT void JNICALL Java_org_godotengine_godot_plugin_GodotPlugin_nativeRegisterGDNativeLibraries(JNIEnv *env, jclass clazz, jobjectArray gdnlib_paths);
}

#endif // GODOT_PLUGIN_JNI_H
