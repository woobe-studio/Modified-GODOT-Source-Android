#ifndef ADVANCED_GUI_DISABLED
    /**************************************************************************/
    /*  rich_text_effect.cpp                                                  */
    /**************************************************************************/


    #include "rich_text_effect.h"

    #include "core/script_language.h"

    void RichTextEffect::_bind_methods() {
        BIND_VMETHOD(MethodInfo(Variant::BOOL, "_process_custom_fx", PropertyInfo(Variant::OBJECT, "char_fx", PROPERTY_HINT_RESOURCE_TYPE, "CharFXTransform")));
    }

    Variant RichTextEffect::get_bbcode() const {
        Variant r;
        if (get_script_instance()) {
            if (!get_script_instance()->get("bbcode", r)) {
                String path = get_script_instance()->get_script()->get_path();
                r = path.get_file().get_basename();
            }
        }
        return r;
    }

    bool RichTextEffect::_process_effect_impl(Ref<CharFXTransform> p_cfx) {
        bool return_value = false;
        if (get_script_instance()) {
            Variant v = get_script_instance()->call("_process_custom_fx", p_cfx);
            if (v.get_type() != Variant::BOOL) {
                return_value = false;
            } else {
                return_value = (bool)v;
            }
        }
        return return_value;
    }

    RichTextEffect::RichTextEffect() {
    }

    void CharFXTransform::_bind_methods() {
        ClassDB::bind_method(D_METHOD("get_relative_index"), &CharFXTransform::get_relative_index);
        ClassDB::bind_method(D_METHOD("set_relative_index", "index"), &CharFXTransform::set_relative_index);

        ClassDB::bind_method(D_METHOD("get_absolute_index"), &CharFXTransform::get_absolute_index);
        ClassDB::bind_method(D_METHOD("set_absolute_index", "index"), &CharFXTransform::set_absolute_index);

        ClassDB::bind_method(D_METHOD("get_elapsed_time"), &CharFXTransform::get_elapsed_time);
        ClassDB::bind_method(D_METHOD("set_elapsed_time", "time"), &CharFXTransform::set_elapsed_time);

        ClassDB::bind_method(D_METHOD("is_visible"), &CharFXTransform::is_visible);
        ClassDB::bind_method(D_METHOD("set_visibility", "visibility"), &CharFXTransform::set_visibility);

        ClassDB::bind_method(D_METHOD("get_offset"), &CharFXTransform::get_offset);
        ClassDB::bind_method(D_METHOD("set_offset", "offset"), &CharFXTransform::set_offset);

        ClassDB::bind_method(D_METHOD("get_color"), &CharFXTransform::get_color);
        ClassDB::bind_method(D_METHOD("set_color", "color"), &CharFXTransform::set_color);

        ClassDB::bind_method(D_METHOD("get_environment"), &CharFXTransform::get_environment);
        ClassDB::bind_method(D_METHOD("set_environment", "environment"), &CharFXTransform::set_environment);

        ClassDB::bind_method(D_METHOD("get_character"), &CharFXTransform::get_character);
        ClassDB::bind_method(D_METHOD("set_character", "character"), &CharFXTransform::set_character);

        ADD_PROPERTY(PropertyInfo(Variant::INT, "relative_index"), "set_relative_index", "get_relative_index");
        ADD_PROPERTY(PropertyInfo(Variant::INT, "absolute_index"), "set_absolute_index", "get_absolute_index");
        ADD_PROPERTY(PropertyInfo(Variant::REAL, "elapsed_time"), "set_elapsed_time", "get_elapsed_time");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "visible"), "set_visibility", "is_visible");
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset"), "set_offset", "get_offset");
        ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
        ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "env"), "set_environment", "get_environment");
        ADD_PROPERTY(PropertyInfo(Variant::INT, "character"), "set_character", "get_character");
    }

    CharFXTransform::CharFXTransform() {
        relative_index = 0;
        absolute_index = 0;
        visibility = true;
        offset = Point2();
        color = Color();
        character = 0;
        elapsed_time = 0.0f;
    }

    CharFXTransform::~CharFXTransform() {
        environment.clear();
    }
#endif
