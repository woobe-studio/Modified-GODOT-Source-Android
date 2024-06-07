/**************************************************************************/
/*  thread_jandroid.h                                                     */
/**************************************************************************/


#ifndef THREAD_JANDROID_H
#define THREAD_JANDROID_H

#include <jni.h>

void init_thread_jandroid(JavaVM *p_jvm, JNIEnv *p_env);

void setup_android_thread();
JNIEnv *get_jni_env();

#endif // THREAD_JANDROID_H
