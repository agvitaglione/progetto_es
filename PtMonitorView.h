#ifndef __PTMONITORVIEW_H__
#define __PTMONITORVIEW_H__

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <slope/slope.h>
#include <string>

class PtMonitorView {

    friend gboolean shutdown_request_handler (GtkWidget *shutdown_button_box);

    private:
        static const int NUMBER_OF_PAGES = 3;
        inline static const std::string builder_file_name = "interface.glade";


        // GLOBAL WIDGET INTERFACE VARIABLES
        GtkBuilder *builder;
        GtkWidget *window;
        GtkWidget *stack;
        GtkWidget *togglebottom1;
        GtkWidget *togglebottom2;
        GtkWidget *togglebottom3;
        GtkWidget *request_dialog;
        GtkWidget* shutdown_button_box;

        // PLOT
        GtkWidget *box_temperature;
        SlopeFigure *figure_temperature;
        GtkWidget *view_temperature;
        SlopeScale *scale_temperature;
        SlopeItem *series_temperature;
        GtkWidget *box_pressure;
        SlopeFigure *figure_pressure;
        GtkWidget *view_pressure;
        SlopeScale *scale_pressure;
        SlopeItem *series_pressure;

        // Callback functions
        void (*shutdownHandler)(void);
        void (*swipeHandler)(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y);


    public:

        PtMonitorView(void);
        PtMonitorView(const PtMonitorView&) = delete;
        const PtMonitorView& operator=(const PtMonitorView&) = delete; 
        ~PtMonitorView();

        void setPage(const int page_number, const GtkStackTransitionType transition);
        void setShoutdownHandler(void (*callback)(void));
        void setSwipeHandler(void (*callback)(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y));
        void startRoutine(void) const;

        int getCurrentPageNumber(void) const;
        static int getNumberOfPages(void);

};

#endif //__PTMONITORVIEW_H__
