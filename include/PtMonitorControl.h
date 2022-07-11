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
        
        /**
         * @brief Get the Instance object.
         * 
         * @see PtMonitorControl
         * 
         * @param[in] _view 
         * @param[in] _model 
         * @return PtMonitorControl* The instance ot object.
         */
        static PtMonitorControl* getInstance(PtMonitorView *_view, PtMonitorModel *_model);

        /**
         * @brief Destroy the Pt Monitor Control object.
         * It stops the periodicGetData thread. 
         */
        ~PtMonitorControl();


        // -------------------------------------  CALLBACK FUNCTIONS

        /**
         * @brief Shut down the device.
         * 
         */
        static void shutdownHandler(void);

        /**
         * @brief Check the swipe direction and change the active page.
         * 
         * @param v_x swipe speed along x direction
         * @param v_y swipe speed along y direction
         */
        static void swipeHandler(gdouble v_x, gdouble v_y);

        /**
         * @brief Refresh to view to display the currently connected usb storage devices.
         * 
         */
        static void usbReloadHandler(void);

        /**
         * @brief Turns off recording activity on the previously selected USB storage device.
         * 
         */
        static void usbReleaseHandler(void);


        static void usbHandler(std::string usbLabel);

        //------------------------------------- 


        // CONNECTION
        void setConnection(std::string recIP, uint16_t recPort);


        //------------------------------------- DISABLED METHODS

        /**
         * @brief The copy constructor is disabled.
         * 
         */
        PtMonitorControl(const PtMonitorControl&) = delete;

        /**
         * @brief The assignement operator is disabled.
         * 
         */
        const PtMonitorControl& operator=(const PtMonitorControl&) = delete;

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