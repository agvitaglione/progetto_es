#ifndef __PTMONITORCONTROL_H__
#define __PTMONITORCONTROL_H__

#include <iostream>
#include "PtMonitorView.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>

// SINGLETON
class PtMonitorControl {

    public:
        
        //SINGLETON
        static PtMonitorControl* getInstance(PtMonitorView* _view);

        // DISABLE OTHER CONSTRUCOTRS
        PtMonitorControl(const PtMonitorControl&) = delete;
        const PtMonitorControl& operator=(const PtMonitorControl&) = delete;

        // CALLBACK FUNCTIONS
        static void shutdownHandler(void);
        static void swipeHandler(gdouble v_x, gdouble v_y);

    protected:
        PtMonitorControl(PtMonitorView* view);

    private:

        //INTERNAL CONSTANT
        static const int XACT = 500;
        static const int YLIMIT = 300;
        
        static PtMonitorView* view;

};

#endif // __PTMONITORCONTROL_H__