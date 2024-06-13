#ifndef GRAPHIC_PROGRESS_H
#define GRAPHIC_PROGRESS_H

#include "scene/2d/node_2d.h"
#include "core/reference.h"

class GraphicProgress : public Node2D {
GDCLASS(GraphicProgress, Node2D);

protected:
    static void _bind_methods();

    Ref<Texture> under_texture;
    Ref<Texture> progress_texture;
    Ref<Texture> over_texture;

    float min_value;
    float max_value;
    float current_value;

    Color progress_color;
    Color under_color;

    Size2 size;
    bool stretch;

    void update_size();
    virtual void _notification(int p_what);
    virtual void _draw();

public:
    enum ProgressDirection {
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
        BOTTOM_TO_TOP,
        TOP_TO_BOTTOM,
    };


    void set_under_texture(const Ref<Texture>& texture);
    Ref<Texture> get_under_texture() const;

    void set_progress_texture(const Ref<Texture>& texture);
    Ref<Texture> get_progress_texture() const;

    void set_over_texture(const Ref<Texture>& texture);
    Ref<Texture> get_over_texture() const;

    void set_size(const Size2& p_size);
    Size2 get_size() const;

    void set_max_value(float value);
    float get_max_value() const;

    void set_value(float value);
    float get_value() const;

    void set_progress_color(const Color& color);
    Color get_progress_color() const;
    void set_under_color(const Color& color);
    Color get_under_color() const;

    void set_progress_direction(ProgressDirection direction);
    ProgressDirection get_progress_direction() const;

    GraphicProgress();
    ~GraphicProgress();

private:
    ProgressDirection progress_direction;
};

VARIANT_ENUM_CAST(GraphicProgress::ProgressDirection);

#endif // GRAPHIC_PROGRESS_H
