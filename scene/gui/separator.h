#ifndef ADVANCED_GUI_DISABLED
    /**************************************************************************/
    /*  separator.h                                                           */
    /**************************************************************************/


    #ifndef SEPARATOR_H
    #define SEPARATOR_H

    #include "scene/gui/control.h"
    class Separator : public Control {
        GDCLASS(Separator, Control);

    protected:
        Orientation orientation;
        void _notification(int p_what);

    public:
        virtual Size2 get_minimum_size() const;

        Separator();
        ~Separator();
    };

    class VSeparator : public Separator {
        GDCLASS(VSeparator, Separator);

    public:
        VSeparator();
    };

    class HSeparator : public Separator {
        GDCLASS(HSeparator, Separator);

    public:
        HSeparator();
    };

    #endif // SEPARATOR_H
#endif