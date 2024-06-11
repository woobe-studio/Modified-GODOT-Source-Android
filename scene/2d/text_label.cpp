/**************************************************************************/
/*  text_label.cpp                                                          */
/**************************************************************************/

#include "text_label.h"

#include "core/print_string.h"
#include "core/project_settings.h"
#include "core/translation.h"


void TextLabel::set_text(const String& p_text) {
    text = p_text;
    update_text();
    update();
}

String TextLabel::get_text() const {
    return text;
}

void TextLabel::set_align(Align p_align) {
    align = p_align;
    update();
}

TextLabel::Align TextLabel::get_align() const {
    return align;
}

void TextLabel::set_max_lines_visible(int max_lines) {
    if (max_lines < -1) {
        max_lines = -1;
    }
    max_lines_visible = max_lines;
    update();
}

int TextLabel::get_max_lines_visible() const {
    return max_lines_visible;
}

void TextLabel::set_percent_visible(float percent) {
    percent_visible = CLAMP(percent, 0.0f, 1.0f);
    update();
}

float TextLabel::get_percent_visible() const {
    return percent_visible;
}

void TextLabel::set_max_chars_per_line(int max_chars) {
    if (max_chars < -1) {
        max_chars = -1;
    }
    max_chars_per_line = max_chars;
    update_text();
    update();
}

int TextLabel::get_max_chars_per_line() const {
    return max_chars_per_line;
}

void TextLabel::set_font(const Ref<Font>& p_font) {
    font = p_font;
    update_text();
    update();
}

Ref<Font> TextLabel::get_font() const {
    return font;
}

void TextLabel::set_font_color(const Color& p_color) {
    font_color = p_color;
    update();
}

Color TextLabel::get_font_color() const {
    return font_color;
}

void TextLabel::update_text() {
    if (font.is_valid() && !text.empty()) {
        if (max_chars_per_line == 0) {
            size.x = 0;
            size.y = 0;
            return;
        }

        size.x = 0;
        size.y = 0;
        Vector<String> initial_lines = text.split("\n");
        Vector<String> lines;

        for (int i = 0; i < initial_lines.size(); i++) {
            String line = initial_lines[i];
            while (line.length() > max_chars_per_line && max_chars_per_line > 0) {
                String part = line.substr(0, max_chars_per_line);
                lines.push_back(part);
                line = line.substr(max_chars_per_line);
            }
            if (!line.empty()) {
                lines.push_back(line);
            }
        }

        int lines_count = max_lines_visible > 0 ? MIN(lines.size(), max_lines_visible) : lines.size();
        for (int i = 0; i < lines_count; i++) {
            float line_width = font->get_string_size(lines[i]).width;
            size.x = MAX(size.x, line_width);
            size.y += font->get_height();
        }
    }
}

Vector2 TextLabel::get_size() const {
    return size;
}

void TextLabel::draw() {
    if (!is_visible_in_tree() || text.empty() || font.is_null() || max_lines_visible == 0 || max_chars_per_line == 0) {
        return;
    }

    Vector<String> lines = text.split("\n");
    float y_offset = 0;

    int total_chars = 0;
    int max_effective_lines = max_lines_visible > 0 ? MIN(lines.size(), max_lines_visible) : lines.size();
    for (int i = 0; i < max_effective_lines; i++) {
        int effective_length = (max_chars_per_line > 0 && lines[i].length() > max_chars_per_line) ? max_chars_per_line : lines[i].length();
        total_chars += effective_length;
    }

    int chars_to_show = static_cast<int>(total_chars * percent_visible);
    int chars_drawn = 0;

    for (int i = 0; i < max_effective_lines && chars_drawn < chars_to_show; i++) {
        String line = lines[i];
        if (max_chars_per_line > 0 && line.length() > max_chars_per_line) {
            line = line.substr(0, max_chars_per_line);
        }

        int line_chars_to_show = MIN(line.length(), chars_to_show - chars_drawn);
        String visible_text = line.substr(0, line_chars_to_show);
        float line_width = font->get_string_size(visible_text).width;
        Point2 line_position;

        switch (align) {
            case ALIGN_CENTER:
                line_position.x = (size.x - line_width) / 2;
                break;
            case ALIGN_LEFT:
                line_position.x = 0;
                break;
            case ALIGN_RIGHT:
                line_position.x = size.x - line_width;
                break;
        }

        line_position.y = y_offset + font->get_ascent();
        draw_string(font, line_position, visible_text, font_color, -1);

        chars_drawn += line_chars_to_show;
        y_offset += font->get_height();

        if (chars_drawn >= chars_to_show) break;
    }
}


void TextLabel::_notification(int p_what) {
    if (p_what == NOTIFICATION_DRAW) {
        draw();
    }
}

void TextLabel::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_text", "text"), &TextLabel::set_text);
    ClassDB::bind_method(D_METHOD("get_text"), &TextLabel::get_text);

    ClassDB::bind_method(D_METHOD("set_font", "font"), &TextLabel::set_font);
    ClassDB::bind_method(D_METHOD("get_font"), &TextLabel::get_font);

    ClassDB::bind_method(D_METHOD("set_font_color", "color"), &TextLabel::set_font_color);
    ClassDB::bind_method(D_METHOD("get_font_color"), &TextLabel::get_font_color);
    ClassDB::bind_method(D_METHOD("set_align", "align"), &TextLabel::set_align);
    ClassDB::bind_method(D_METHOD("get_align"), &TextLabel::get_align);
    ClassDB::bind_method(D_METHOD("set_max_lines_visible", "max_lines"), &TextLabel::set_max_lines_visible);
    ClassDB::bind_method(D_METHOD("get_max_lines_visible"), &TextLabel::get_max_lines_visible);
    ClassDB::bind_method(D_METHOD("set_percent_visible", "percent"), &TextLabel::set_percent_visible);
    ClassDB::bind_method(D_METHOD("get_percent_visible"), &TextLabel::get_percent_visible);
    ClassDB::bind_method(D_METHOD("set_max_chars_per_line", "max_chars"), &TextLabel::set_max_chars_per_line);
    ClassDB::bind_method(D_METHOD("get_max_chars_per_line"), &TextLabel::get_max_chars_per_line);

    BIND_ENUM_CONSTANT(ALIGN_LEFT);
    BIND_ENUM_CONSTANT(ALIGN_CENTER);
    BIND_ENUM_CONSTANT(ALIGN_RIGHT);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "text", PROPERTY_HINT_MULTILINE_TEXT), "set_text", "get_text");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "font", PROPERTY_HINT_RESOURCE_TYPE, "Font"), "set_font", "get_font");
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "font_color"), "set_font_color", "get_font_color");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "align", PROPERTY_HINT_ENUM, "Left,Center,Right"), "set_align", "get_align");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_lines_visible"), "set_max_lines_visible", "get_max_lines_visible");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "percent_visible", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_percent_visible", "get_percent_visible");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_chars_per_line"), "set_max_chars_per_line", "get_max_chars_per_line");
}

TextLabel::TextLabel() {
    align = ALIGN_CENTER;
    font_color = Color(1, 1, 1);
    max_lines_visible = -1;
    percent_visible = 1.0f;
    max_chars_per_line = -1;
}

TextLabel::~TextLabel() {
    // Destructor
}