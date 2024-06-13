#ifndef ADVANCED_GUI_DISABLED
    /**************************************************************************/
    /*  tool_button.h                                                         */
    /**************************************************************************/


    #ifndef TOOL_BUTTON_H
    #define TOOL_BUTTON_H

    #include "scene/gui/button.h"

    class ToolButton : public Button {
        GDCLASS(ToolButton, Button);

    public:
        ToolButton();
    };

    #endif // TOOL_BUTTON_H
#endif