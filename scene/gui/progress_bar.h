#ifndef ADVANCED_GUI_DISABLED
    /**************************************************************************/
    /*  progress_bar.h                                                        */
    /**************************************************************************/


    #ifndef PROGRESS_BAR_H
    #define PROGRESS_BAR_H

    #include "scene/gui/range.h"

    class ProgressBar : public Range {
        GDCLASS(ProgressBar, Range);

        bool percent_visible;

    protected:
        void _notification(int p_what);
        static void _bind_methods();

    public:
        void set_percent_visible(bool p_visible);
        bool is_percent_visible() const;

        Size2 get_minimum_size() const;
        ProgressBar();
    };

    #endif // PROGRESS_BAR_H
#endif
