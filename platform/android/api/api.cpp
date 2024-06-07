/**************************************************************************/
/*  api.cpp                                                               */
/**************************************************************************/


#include "api.h"

#include "core/engine.h"
#include "java_class_wrapper.h"
#include "jni_singleton.h"

#if !defined(ANDROID_ENABLED)
static JavaClassWrapper *java_class_wrapper = nullptr;
#endif

void register_android_api() {
#if !defined(ANDROID_ENABLED)
	// On Android platforms, the `java_class_wrapper` instantiation and the
	// `JNISingleton` registration occurs in
	// `platform/android/java_godot_lib_jni.cpp#Java_org_godotengine_godot_GodotLib_setup`
	java_class_wrapper = memnew(JavaClassWrapper); // Dummy
	ClassDB::register_class<JNISingleton>();
#endif

	ClassDB::register_class<JavaClass>();
	ClassDB::register_class<JavaClassWrapper>();
	Engine::get_singleton()->add_singleton(Engine::Singleton("JavaClassWrapper", JavaClassWrapper::get_singleton()));
}

void unregister_android_api() {
#if !defined(ANDROID_ENABLED)
	memdelete(java_class_wrapper);
#endif
}

void JavaClassWrapper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("wrap", "name"), &JavaClassWrapper::wrap);
}

#if !defined(ANDROID_ENABLED)

Variant JavaClass::call(const StringName &, const Variant **, int, Variant::CallError &) {
	return Variant();
}

JavaClass::JavaClass() {
}

Variant JavaObject::call(const StringName &, const Variant **, int, Variant::CallError &) {
	return Variant();
}

JavaClassWrapper *JavaClassWrapper::singleton = nullptr;

Ref<JavaClass> JavaClassWrapper::wrap(const String &) {
	return Ref<JavaClass>();
}

JavaClassWrapper::JavaClassWrapper() {
	singleton = this;
}

#endif
