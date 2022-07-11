#ifndef __PTMONITORVIEW_H__
#define __PTMONITORVIEW_H__

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <slope/slope.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "TypeDefinitions.h"
#include <map>


// Singleton Class

class PtMonitorView {

    // --------------------------- FRIEND FUNCTIONS

    /**
     * @brief Handler function called by the interface when the user press the shutdown button. It invokes the function
     *  setted with setShoutdownHandler.
     * 
     * @param shutdown_button_box handler function for shutdown request.
     * @return gboolean always true. 
     */
    friend gboolean _shutdownRequestHandler (GtkWidget *shutdown_button_box);

    /**
     * @brief Handler function called by the interface when the user change the page through swiping. It invokes the function
     *  setted with setSwipeHandler.
     * 
     * @param swipe gtk object for swipe handling.
     * @param v_x swipe speed on x axis.
     * @param v_y swipe speed on y axis.
     */
    friend void _changePageHandler(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y);

    /**
     * @brief Handler function called by the interface when the user chooses an USB where to store data.
     * 
     * @param button pressed. The button label is the USB name. 
     */
    friend void _usbButtonHandler(GtkWidget* button);

    /**
     * @brief Handler function called by the interface when the user requests to reload the usb list.  It invokes the function
     *  setted with setUsbReloadButtonHandler.
     * 
     * @param eventBox gtk object for the event box where the user has clicked.
     * @return gboolean always true.
     */
    friend gboolean _usbReloadButtonHandler(GtkWidget *eventBox); 

    /**
     * @brief Handler function called by the interface when the user requests to stop logging on USB. It invokes the function
     *  setted with setUsbReloadButtonHandler.
     * 
     * @param eventBox gtk object for the event box where the user has clicked.
     * @return gboolean always true.
     */
    friend gboolean _usbReleaseButtonHandler(GtkWidget *eventBox);
   
    // --------------------------- 

    public:

        /**
         * @brief Get the Instance object.
         * 
         * @return PtMonitorView*. 
         */
        static PtMonitorView* getInstance();

        /**
         * @brief Destroy the Pt Monitor View object.
         * 
         */
        ~PtMonitorView();

        /**
         * @brief The copy constructor is disabled.
         * 
         */
        PtMonitorView(const PtMonitorView&) = delete;

        /**
         * @brief The assignment operator is disabled.
         * 
         * 
         */
        const PtMonitorView& operator=(const PtMonitorView&) = delete; 
        
        /**
         * @brief Get the Current Page Number. 
         * 
         * @return 0 if the user is in the first page.
         * @return 1 if the user is in the second page.
         */
        int getCurrentPageNumber(void) const;

        /**
         * @brief Get the Number Of Pages object
         * 
         * @return int. In this version, always 2.
         */
        static int getNumberOfPages(void);


        /**
         * @brief Set the page showed to the user.
         * 
         * @param page_number It can be or 0 or 1.
         * @param transition Set the transition type (left to right, right to left...).
         */
        void setPage(const int page_number, const GtkStackTransitionType transition);

        // --------------------------- SET HANDLERS

        /**
         * @brief Set the Shoutdown Handler callback function, which is called when a shoutdown request is raised.
         * 
         * @param callback 
         */
        void setShoutdownHandler(void (*callback)(void));

        /**
         * @brief Set the Swipe Handler callback function, which is called when the user trys to change the page through swiping.
         * 
         * @param callback 
         *      - v_x[in] swipe speed on x axis.
         *      - v_y[in] swipe speed on y axis.
         */
        void setSwipeHandler(void (*callback)(gdouble v_x, gdouble v_y));

        /**
         * @brief Set the Usb Button Handler callback function, which is called when the user chooses an USB.
         * 
         * @param callback 
         *      - usb_name[in] Name of the chosen USB.
         */
        void setUsbButtonHandler(void (*callback)(std::string usb_name));

        /**
         * @brief Set the Usb Reload Button Handler callback function, which is called when the user requests to reload
         *  the usb detected, usually when a new usb is plugged in.
         * 
         * @param callback 
         */
        void setUsbReloadButtonHandler(void (*callback)(void));

        /**
         * @brief Set the Usb Release Button Handler callback function, which is called when the user requests to stop logging on USB.
         * 
         * @param callback 
         */
        void setUsbReleaseButtonHandler(void (*callback)(void));

        // ---------------------------

        /**
         * @brief Set the Measure Values shown on the first page. 
         * 
         * @param value to be shown.
         * @param measure Type of the measured value which can be either TEMPERATURE or PRESSURE. 
         * @param axis car axis where the tyre is.
         * @param tyre identification number of the tyre. 
         */
        void setMeasureValues(int value, MeasureType measure, const int axis, const int tyre);

        /**
         * @brief Plot the data on one of the box plot in the second page. 
         * 
         * @param data to be plotted.
         * @param nelem number of the points to be plotted.
         * @param graph type of the value to be plotted (temperature, pressure).
         * @param axis car axis where the tyre is.
         * @param tyre identification number of the tyre.
         */
        void plotData(const DataType& data, const int nelem, const MeasureType graph, const int axis, const int tyre);

        /**
         * @brief Starts the gtk main function. 
         *  It should be called at the end, when all the initialization phase in concluded.
         * 
         */
        void startRoutine(void) const;

        /**
         * @brief Add USB on the interface. If the USB is already in the list, the function won't do anything. 
         * 
         * @param usb_name name of the USB to add to the list.
         */
        void addUSB(std::string usb_name);

        /**
         * @brief Remove USB from the interface. If the USB is not in the list, the function won't do anything
         * 
         * @param usb_name name of the USB to remove from the list.
         */
        void removeUSB(std::string usb_name);
        
        /**
         * @brief Remove all USB from the interface.
         * 
         */
        void removeAllUSB();


    protected:

        PtMonitorView(void);

    private:

        /// Number of pages in the application.
        static const int NUMBER_OF_PAGES = 2;

        /// Name of the glade file.
        /// If the file cannot be found, the application will crash.
        inline static const std::string builder_file_name =  "../interface_scalable.glade";


        // -------------------------- WIDGET INTERFACE VARIABLES

        GtkBuilder *builder;
        GtkWidget *window;
        GtkWidget *stack;
        GtkWidget *togglebottom1;
        GtkWidget *togglebottom2;
        GtkWidget *togglebottom3;
        GtkWidget *request_dialog;
        GtkWidget *shutdown_button_box;
        GtkWidget *realease_button;
        GtkWidget *reload_button;
        
        // ---------------------------

        // --------------------------- LABELS

        /// Example:
        /// interface_labels_temperature[axis][tyre] is GtkWidget pointer which
        /// is refered to the label on the first page that shows the temperature 
        /// of the tyre [axis][tyre].
        GtkWidget ***interface_labels_temperature;
        GtkWidget ***interface_labels_pressure;

        ---------------------------

        // GESTURE
        GtkGesture *swipe;

        // --------------------------- PLOT WIDGET VARIABLES 

        GtkWidget *box_plot;
        GtkWidget ***view_temperature;
        SlopeScale ***scale_temperature;
        SlopeItem ***series_temperature;
        GtkWidget ***view_pressure;
        SlopeScale ***scale_pressure;
        SlopeItem ***series_pressure;

        // ---------------------------

        // --------------------------- USB UTILITIES VARIABLES

        GtkWidget* box_popoverusb;
        std::map<std::string, GtkWidget*> usb_list;

        ---------------------------


        // ---------------------------  CALLBACK FUNCTIONS

        void (*shutdownHandler)(void);
        void (*swipeHandler)(gdouble v_x, gdouble v_y);
        void (*usbButtonHandler)(std::string usb_name);
        void (*usbReloadButtonHandler)(void);
        void (*usbReleaseButtonHandler)(void);

         // --------------------------- 

};

#endif //__PTMONITORVIEW_H__
