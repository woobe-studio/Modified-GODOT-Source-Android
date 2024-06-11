/**************************************************************************/
/*  text_label.h                                                          */
/**************************************************************************/

#ifndef TEXT_LABEL_H
#define TEXT_LABEL_H

#include "scene/2d/node_2d.h"
#include "scene/resources/font.h"

class TextLabel : public Node2D {
GDCLASS(TextLabel, Node2D);

public:
    enum Align {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT
    };

    void set_text(const String& p_text);
    String get_text() const;

    void set_font(const Ref<Font>& p_font);
    Ref<Font> get_font() const;

    void set_font_color(const Color& p_color);
    Color get_font_color() const;

    Vector2 get_size() const;

    void set_align(Align p_align);
    Align get_align() const;

    void set_max_lines_visible(int max_lines);
    int get_max_lines_visible() const;

    void set_percent_visible(float percent);
    float get_percent_visible() const;

    void set_max_chars_per_line(int max_chars);
    int get_max_chars_per_line() const;

    TextLabel();
    ~TextLabel();

private:
    String text;
    Ref<Font> font;
    Align align;
    Color font_color;
    Vector2 size;
    int max_lines_visible;
    float percent_visible;
    int max_chars_per_line;

protected:
    void _notification(int p_what);
    static void _bind_methods();
    void draw();

private:
    void update_text();
};

VARIANT_ENUM_CAST(TextLabel::Align);

#endif // TEXT_LABEL_H
