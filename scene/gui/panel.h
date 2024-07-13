#ifndef ADVANCED_GUI_DISABLED
    /**************************************************************************/
    /*  panel.h                                                               */
    /**************************************************************************/


    #ifndef PANEL_H
    #define PANEL_H

    #include "scene/gui/control.h"

    class Panel : public Control {
        GDCLASS(Panel, Control);

    protected:
        void _notification(int p_what);

    public:
        Panel();
        ~Panel();
    };

    #endif // PANEL_H
#endif
