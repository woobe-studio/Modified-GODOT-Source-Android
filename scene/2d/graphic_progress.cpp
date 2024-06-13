#include "graphic_progress.h"

void GraphicProgress::set_under_texture(const Ref<Texture>& texture) {
    under_texture = texture;
    update_size();
    update();
}

Ref<Texture> GraphicProgress::get_under_texture() const {
    return under_texture;
}

void GraphicProgress::set_progress_texture(const Ref<Texture>& texture) {
    progress_texture = texture;
    update_size();
    update();
}

Ref<Texture> GraphicProgress::get_progress_texture() const {
    return progress_texture;
}

void GraphicProgress::set_over_texture(const Ref<Texture>& texture) {
    over_texture = texture;
    update_size();
    update();
}

Ref<Texture> GraphicProgress::get_over_texture() const {
    return over_texture;
}

void GraphicProgress::update_size() {
    Size2 new_size = Size2();
    if (under_texture.is_valid()) {
        new_size.width = MAX(new_size.width, under_texture->get_width());
        new_size.height = MAX(new_size.height, under_texture->get_height());
    }
    if (progress_texture.is_valid()) {
        new_size.width = MAX(new_size.width, progress_texture->get_width());
        new_size.height = MAX(new_size.height, progress_texture->get_height());
    }
    if (over_texture.is_valid()) {
        new_size.width = MAX(new_size.width, over_texture->get_width());
        new_size.height = MAX(new_size.height, over_texture->get_height());
    }
    size = new_size;
}


Size2 GraphicProgress::get_size() const {
    return size;
}

void GraphicProgress::set_max_value(float value) {
    max_value = value;
    update();
}

float GraphicProgress::get_max_value() const {
    return max_value;
}

void GraphicProgress::set_value(float value) {
    current_value = CLAMP(value, min_value, max_value);
    update();
}

float GraphicProgress::get_value() const {
    return current_value;
}

void GraphicProgress::set_progress_color(const Color& color) {
    progress_color = color;
    update();
}

Color GraphicProgress::get_progress_color() const {
    return progress_color;
}

void GraphicProgress::set_under_color(const Color& color) {
    under_color = color;
    update();
}

Color GraphicProgress::get_under_color() const {
    return under_color;
}

void GraphicProgress::set_progress_direction(ProgressDirection direction) {
    progress_direction = direction;
    update();
}

GraphicProgress::ProgressDirection GraphicProgress::get_progress_direction() const {
    return progress_direction;
}

void GraphicProgress::_notification(int p_what) {
    if (p_what == NOTIFICATION_DRAW) {
        _draw();
    }
}

void GraphicProgress::_draw() {
    Vector2 center = -Vector2(size.width / 2, size.height / 2);
    if (under_texture.is_valid()) {
        draw_texture_rect(under_texture, Rect2(center, size), false, under_color);
    }

    if (progress_texture.is_valid()) {
        Size2 texture_size = progress_texture->get_size();
        float progress_fraction = current_value / max_value;
        float width = texture_size.width * progress_fraction;
        float height = texture_size.height * progress_fraction;

        Rect2 src_rect;
        Rect2 dest_rect;

        switch (progress_direction) {
            case LEFT_TO_RIGHT:
                src_rect = Rect2(center, Size2(width, texture_size.height));
                dest_rect = Rect2(Point2(), Size2(width, size.height));
                break;
            case RIGHT_TO_LEFT:
                src_rect = Rect2(Point2(texture_size.width - width + center.x, center.y), Size2(width, texture_size.height));
                dest_rect = Rect2(Point2(size.width - width, 0), Size2(width, size.height));
                break;
            case BOTTOM_TO_TOP:
                src_rect = Rect2(Point2(center.x, texture_size.height - height + center.y), Size2(texture_size.width, height));
                dest_rect = Rect2(Point2(0, size.height - height), Size2(size.width, height));
                break;
            case TOP_TO_BOTTOM:
                src_rect = Rect2(center, Size2(texture_size.width, height));
                dest_rect = Rect2(Point2(), Size2(size.width, height));
                break;
        }

        draw_texture_rect_region(progress_texture, src_rect, dest_rect, progress_color);
    }

    if (over_texture.is_valid()) {
        draw_texture_rect(over_texture, Rect2(center, size), false);
    }
}



void GraphicProgress::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_under_texture", "texture"), &GraphicProgress::set_under_texture);
    ClassDB::bind_method(D_METHOD("get_under_texture"), &GraphicProgress::get_under_texture);
    ClassDB::bind_method(D_METHOD("set_progress_texture", "texture"), &GraphicProgress::set_progress_texture);
    ClassDB::bind_method(D_METHOD("get_progress_texture"), &GraphicProgress::get_progress_texture);
    ClassDB::bind_method(D_METHOD("set_over_texture", "texture"), &GraphicProgress::set_over_texture);
    ClassDB::bind_method(D_METHOD("get_over_texture"), &GraphicProgress::get_over_texture);

    ClassDB::bind_method(D_METHOD("set_max_value", "value"), &GraphicProgress::set_max_value);
    ClassDB::bind_method(D_METHOD("get_max_value"), &GraphicProgress::get_max_value);
    ClassDB::bind_method(D_METHOD("set_value", "value"), &GraphicProgress::set_value);
    ClassDB::bind_method(D_METHOD("get_value"), &GraphicProgress::get_value);

    ClassDB::bind_method(D_METHOD("set_progress_color", "color"), &GraphicProgress::set_progress_color);
    ClassDB::bind_method(D_METHOD("get_progress_color"), &GraphicProgress::get_progress_color);
    ClassDB::bind_method(D_METHOD("set_under_color", "color"), &GraphicProgress::set_under_color);
    ClassDB::bind_method(D_METHOD("get_under_color"), &GraphicProgress::get_under_color);
    ClassDB::bind_method(D_METHOD("set_progress_direction", "direction"), &GraphicProgress::set_progress_direction);
    ClassDB::bind_method(D_METHOD("get_progress_direction"), &GraphicProgress::get_progress_direction);



    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "under_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_under_texture", "get_under_texture");
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "under_color"), "set_under_color", "get_under_color");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "progress_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_progress_texture", "get_progress_texture");
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "progress_color"), "set_progress_color", "get_progress_color");

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "over_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_over_texture", "get_over_texture");

    ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_value", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_max_value", "get_max_value");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "current_value", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_value", "get_value");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "progress_direction", PROPERTY_HINT_ENUM, "Left to Right,Right to Left,Bottom to Top,Top to Bottom"), "set_progress_direction", "get_progress_direction");
}

GraphicProgress::GraphicProgress() {
    size = Size2(100, 20); // Default size
    min_value = 0.0;
    max_value = 100.0;
    current_value = 0.0;
    progress_color = Color(1, 1, 1, 1);
    under_color = Color(1, 1, 1, 1);
    stretch = true;
    progress_direction = LEFT_TO_RIGHT;
}

GraphicProgress::~GraphicProgress() {
    // Clean-up code here if needed
}