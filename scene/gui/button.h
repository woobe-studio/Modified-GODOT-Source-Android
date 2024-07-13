#ifndef ADVANCED_GUI_DISABLED
    /**************************************************************************/
    /*  button.h                                                              */
    /**************************************************************************/


    #ifndef BUTTON_H
    #define BUTTON_H

    #include "scene/gui/base_button.h"

    class Button : public BaseButton {
        GDCLASS(Button, BaseButton);

    public:
        enum TextAlign {
            ALIGN_LEFT,
            ALIGN_CENTER,
            ALIGN_RIGHT
        };

    private:
        bool flat;
        String text;
        String xl_text;
        Ref<Texture> icon;
        bool expand_icon;
        bool clip_text;
        TextAlign align;
        TextAlign icon_align;
        float _internal_margin[4];

        void _texture_changed();

    protected:
        void _set_internal_margin(Margin p_margin, float p_value);
        void _notification(int p_what);
        static void _bind_methods();

    public:
        virtual Size2 get_minimum_size() const;

        void set_text(const String &p_text);
        String get_text() const;

        void set_icon(const Ref<Texture> &p_icon);
        Ref<Texture> get_icon() const;

        void set_expand_icon(bool p_expand_icon);
        bool is_expand_icon() const;

        void set_flat(bool p_flat);
        bool is_flat() const;

        void set_clip_text(bool p_clip_text);
        bool get_clip_text() const;

        void set_text_align(TextAlign p_align);
        TextAlign get_text_align() const;

        void set_icon_align(TextAlign p_align);
        TextAlign get_icon_align() const;

        Button(const String &p_text = String());
        ~Button();
    };

    VARIANT_ENUM_CAST(Button::TextAlign);

    #endif // BUTTON_H
#endif
