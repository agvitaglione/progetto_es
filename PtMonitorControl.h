#ifndef __PTMONITORCONTROL_H__
#define __PTMONITORCONTROL_H__

#include <iostream>
#include "PtMonitorView.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>

class PtMonitorControl {

    private:
        static const int XACT = 500;
        static const int YLIMIT = 300;
        PtMonitorView* view;

    public:
        
        PtMonitorControl(PtMonitorView* view);
        PtMonitorControl(const PtMonitorControl&) = delete;
        const PtMonitorControl& operator=(const PtMonitorControl&) = delete;

        void shutdownHandler(void);
        void swipeHandler(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y);
};

#endif // __PTMONITORCONTROL_H__