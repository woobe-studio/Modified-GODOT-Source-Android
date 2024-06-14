/**************************************************************************/
/*  centralizer.cpp                                                           */
/**************************************************************************/

#include "centralizer.h"

#include "scene/main/viewport.h"

void Centralizer::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE:
            set_position_preset(current_preset);
            add_to_group("Centralizer");
            break;
        case NOTIFICATION_EXIT_TREE:
            remove_from_group("Centralizer");
            break;
    }
}

void Centralizer::update_position() {
    set_position_preset(current_preset);
}


void Centralizer::set_position_preset(LayoutPreset p_preset) {
    current_preset = p_preset;
    if(!get_viewport())
        return;

    Vector2 viewport_size = get_viewport()->get_visible_rect().size;
    Vector2 position;

    switch (p_preset) {
        case PRESET_TOP_LEFT:
            position = Vector2(0, 0);
            break;
        case PRESET_TOP_RIGHT:
            position = Vector2(viewport_size.x, 0);
            break;
        case PRESET_BOTTOM_LEFT:
            position = Vector2(0, viewport_size.y);
            break;
        case PRESET_BOTTOM_RIGHT:
            position = Vector2(viewport_size.x, viewport_size.y);
            break;
        case PRESET_CENTER:
            position = viewport_size * 0.5;
            break;
    }

    set_global_position(position);
}

int Centralizer::get_position_preset() const {
    return current_preset;
}

void Centralizer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_position_preset", "preset"), &Centralizer::set_position_preset);
    ClassDB::bind_method(D_METHOD("get_position_preset"), &Centralizer::get_position_preset);
    ClassDB::bind_method(D_METHOD("update_position"), &Centralizer::update_position);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "preset", PROPERTY_HINT_ENUM, "Top Left,Top Right,Bottom Left,Bottom Right,Center"), "set_position_preset", "get_position_preset");
}

Centralizer::Centralizer() {
    current_preset = PRESET_CENTER;
}

Centralizer::~Centralizer() {
}