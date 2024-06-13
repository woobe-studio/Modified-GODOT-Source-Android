#ifndef ADVANCED_GUI_DISABLED
    /**************************************************************************/
    /*  check_button.h                                                        */
    /**************************************************************************/


    #ifndef CHECK_BUTTON_H
    #define CHECK_BUTTON_H

    #include "scene/gui/button.h"
    /**
    @author Juan Linietsky <reduzio@gmail.com>
    */
    class CheckButton : public Button {
        GDCLASS(CheckButton, Button);

    protected:
        Size2 get_icon_size() const;
        virtual Size2 get_minimum_size() const;
        void _notification(int p_what);

    public:
        CheckButton();
        ~CheckButton();
    };

    #endif // CHECK_BUTTON_H
#endif