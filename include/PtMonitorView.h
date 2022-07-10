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

    friend gboolean _shutdownRequestHandler (GtkWidget *shutdown_button_box);
    friend void _changePageHandler(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y);
    friend void _usbButtonHandler(GtkWidget* button, gpointer data);
   
    // --------------------------- 

    public:

        // SINGLETON
        static PtMonitorView* getInstance();
        ~PtMonitorView();

        // DISABLE OTHER CONSTRUCTORS
        PtMonitorView(const PtMonitorView&) = delete;
        const PtMonitorView& operator=(const PtMonitorView&) = delete; 
        

        int getCurrentPageNumber(void) const;
        static int getNumberOfPages(void);


        void setPage(const int page_number, const GtkStackTransitionType transition);

        // SET HANDLERS
        void setShoutdownHandler(void (*callback)(void));
        void setSwipeHandler(void (*callback)(gdouble v_x, gdouble v_y));
        void setUsbButtonHandler(void (*callback)(std::string usb_name));

        // SET MEASURE LABELS
        void setMeasureValues(int value, MeasureType measure, const int axis, const int tyre);

        // DRAW DATA
        /*
        * Data must be a vector of 4 elements
        */
        void plotData(const DataType& data, const int nelem, const MeasureType graph, const int axis, const int tyre);

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

        // GLOBAL VIARIABLES
        static const int NUMBER_OF_PAGES = 2;
        inline static const std::string builder_file_name =  "../interface_scalable.glade";


        // GLOBAL WIDGET INTERFACE VARIABLES
        GtkBuilder *builder;
        GtkWidget *window;
        GtkWidget *stack;
        GtkWidget *togglebottom1;
        GtkWidget *togglebottom2;
        GtkWidget *togglebottom3;
        GtkWidget *request_dialog;
        GtkWidget *shutdown_button_box;
        
        //LABELS
        GtkWidget ***interface_labels_temperature;
        GtkWidget ***interface_labels_pressure;

        // GESTURE
        GtkGesture *swipe;

        // PLOT
        GtkWidget *box_plot;
        GtkWidget ***view_temperature;
        SlopeScale ***scale_temperature;
        SlopeItem ***series_temperature;
        GtkWidget ***view_pressure;
        SlopeScale ***scale_pressure;
        SlopeItem ***series_pressure;

        // USB UTILITIES
        GtkWidget* box_popoverusb;
        std::map<std::string, GtkWidget*> usb_list;


        // ---------------------------  CALLBACK FUNCTIONS

        void (*shutdownHandler)(void);
        void (*swipeHandler)(gdouble v_x, gdouble v_y);
        void (*usbButtonHandler)(std::string usb_name);

         // --------------------------- 

};

#endif //__PTMONITORVIEW_H__
