#ifndef ADVANCED_GUI_DISABLED
    /**************************************************************************/
    /*  panel_container.h                                                     */
    /**************************************************************************/


    #ifndef PANEL_CONTAINER_H
    #define PANEL_CONTAINER_H

    #include "scene/gui/container.h"

    class PanelContainer : public Container {
        GDCLASS(PanelContainer, Container);

    protected:
        void _notification(int p_what);

    public:
        virtual Size2 get_minimum_size() const;

        PanelContainer();
    };

    #endif // PANEL_CONTAINER_H
#endif
