#ifndef __PTMONITORCONTROL_H__
#define __PTMONITORCONTROL_H__

#include "PtMonitorView.h"
#include "PtMonitorModel.h"
#include "MessageQueue.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>

// SINGLETON
class PtMonitorControl {

    public:
        
        //SINGLETON
        static PtMonitorControl* getInstance(PtMonitorView *_view, PtMonitorModel *_model);
        ~PtMonitorControl();

        // DISABLE OTHER CONSTRUCOTRS
        PtMonitorControl(const PtMonitorControl&) = delete;
        const PtMonitorControl& operator=(const PtMonitorControl&) = delete;

        // CALLBACK FUNCTIONS
        static void shutdownHandler(void);
        static void swipeHandler(gdouble v_x, gdouble v_y);
        static void usbReloadHandler(void);
        static void usbReleaseHandler(void);
        static void usbHandler(std::string usbLabel);

        // CONNECTION
        void setConnection(std::string recIP, uint16_t recPort);

    protected:
        PtMonitorControl(PtMonitorView *view, PtMonitorModel *model);

    private:

        //INTERNAL CONSTANT
        static const int XACT = 500;
        static const int YLIMIT = 300;
        
        static PtMonitorView *view;
        static PtMonitorModel *model;

        //Queue Array
        static MessageQueue **queues;

        static void periodicGetData();
};

#endif // __PTMONITORCONTROL_H__