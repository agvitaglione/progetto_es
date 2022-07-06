#include "PtMonitorView.h"
#include <string>
#include "PtConfig.h"

/*
* TODO
* Variabili da prendere da file
*/
static int numberOfAxis;
static int numberOfTyrePerAxis;

// X AXIS FOR PLOT
#define NLABEL 4
static const SlopeSample slope_sampler_array[] = {
    {-60, (char*) "60s"},
    {-40, (char*) "40s"},
    {-20, (char*) "20s"},
    {0  , (char*) "0s" },
};

//----------------------------------- CALLBACKS
// SWIPE HANDLER
void changePageHandler(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y) {
    PtMonitorView* view = PtMonitorView::getInstance();
    view->swipeHandler(v_x, v_y);
}

// CONFIRM SHUTDOWN BUTTON HANDLER
gboolean shutdownRequestHandler (GtkWidget *shutdown_button_box) {

	PtMonitorView* view = PtMonitorView::getInstance();

	int risultato = gtk_dialog_run(GTK_DIALOG(view->request_dialog));

	if(risultato == 1 /* SHUTDOWN */) {
		view->shutdownHandler();
	}
	else if (risultato == 0 /* CANCEL */) {
		gtk_widget_hide(view->request_dialog);
	}

    return TRUE;
}


//----------------------------------- GENERATE GRID

void generateGrid(GtkWidget* grid, GtkWidget ***interface_labels_temperature, GtkWidget ***interface_labels_pressure) {

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), true);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);


    for(int i = 0; i < numberOfAxis; i++) {

        int ntyre = 0;

        for(int j = 0; j < 2; j++) {

            GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12); // BOX INTO GRID CELL
            gtk_grid_attach(GTK_GRID(grid), box, j, i, 1, 1);


            std::string label_box_str = "ASSE " + std::to_string(i + 1);
            label_box_str += j == 0 ? " L" : " R";
            GtkWidget* label_box = gtk_label_new(label_box_str.c_str());
            gtk_widget_set_halign(label_box, GTK_ALIGN_START);
            gtk_box_pack_start(GTK_BOX(box), label_box, false, false, 0);


            GtkWidget* grid_tp = gtk_grid_new();
            gtk_box_pack_start(GTK_BOX(box), grid_tp, false, false, 0);
            gtk_grid_set_column_homogeneous(GTK_GRID(grid_tp), true);

            // --------------------------- SET IMAGE TEMPERATURE AND PRESURE

            GtkWidget* image_thermometer = gtk_image_new_from_file("../img/thermometer.png");
            gtk_grid_attach(GTK_GRID(grid_tp), image_thermometer, 0, 0, 1, 1);

            GtkWidget* image_tyre = gtk_image_new_from_file("../img/tyre.png");
            gtk_grid_attach(GTK_GRID(grid_tp), image_tyre, 2, 0, 1, 1);

            // ---------------------------


            // --------------------------- GENERATE LABEL T AND P FOR EACH TYRE


            GtkWidget* box_temperature = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_set_homogeneous(GTK_BOX(box_temperature), true);
            gtk_grid_attach(GTK_GRID(grid_tp), box_temperature, 1, 0, 1, 1);

            GtkWidget* box_pressure = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_set_homogeneous(GTK_BOX(box_pressure), true);
            gtk_grid_attach(GTK_GRID(grid_tp), box_pressure, 3, 0, 1, 1);

            for(int k = 0; k < numberOfTyrePerAxis / 2; k++) {
                interface_labels_temperature[i][ntyre] = gtk_label_new("0°C");
                interface_labels_pressure[i][ntyre] = gtk_label_new("0 bar");
                gtk_box_pack_start(GTK_BOX(box_temperature), interface_labels_temperature[i][ntyre], false, false, 0);
                gtk_box_pack_start(GTK_BOX(box_pressure), interface_labels_pressure[i][ntyre], false, false, 0);
                ntyre++;
            }
            
            // ---------------------------

        }
    }
}



//---------------------------------- CLASS FUNCTIONS

PtMonitorView* PtMonitorView::getInstance() {
    static PtMonitorView instance;
    return &instance;
}

PtMonitorView::PtMonitorView(void) {

    gtk_init(NULL, NULL);
	
	builder = gtk_builder_new_from_file(builder_file_name.c_str());
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack"));
	togglebottom1 = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_1"));
	togglebottom2 = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_2"));
	togglebottom3 = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_3"));
	box_temperature = GTK_WIDGET(gtk_builder_get_object(builder, "pagina2"));
	box_pressure = GTK_WIDGET(gtk_builder_get_object(builder, "pagina3"));
    request_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "request_dialog"));
    shutdown_button_box = GTK_WIDGET(gtk_builder_get_object(builder, "shutdown_button_box"));

    // ------------------------ GENERATE GRID

    // Get number of axis and tyre for each axis
    PtConfig *ptConfig = PtConfig::getInstance(); 
    ::numberOfAxis = ptConfig->getNumberOfAxis();
    ::numberOfTyrePerAxis = ptConfig->getNumberOfTyrePerAxis();

    GtkWidget* box_first_page = GTK_WIDGET(gtk_builder_get_object(builder, "pagina1"));

    interface_labels_temperature = new GtkWidget**[numberOfAxis];
    interface_labels_pressure = new GtkWidget**[numberOfAxis];
    for(int i = 0; i < numberOfAxis; i++) {
        interface_labels_temperature[i] = new GtkWidget*[numberOfTyrePerAxis];
        interface_labels_pressure[i] = new GtkWidget*[numberOfTyrePerAxis];
    }
    generateGrid(box_first_page, interface_labels_temperature, interface_labels_pressure);


    // ------------------------

    // ------------------------ CALLBACKS

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(shutdown_button_box, "button-release-event", G_CALLBACK(shutdownRequestHandler), NULL);

    // ------------------------
    

  	// ------------------------ ADD GESTURE LISTENER

	swipe = gtk_gesture_swipe_new(window);
	g_signal_connect(swipe, "swipe", G_CALLBACK(changePageHandler), NULL);

    //------------------------


    // ------------------------ CREATE DIAGRAM BOXES

    figure_temperature = slope_figure_new();
	scale_temperature = slope_xyscale_new_axis("Time [s]", "Value [°C]", "Temperature");
	view_temperature   = slope_view_new();
	gtk_box_pack_start(GTK_BOX(box_temperature), GTK_WIDGET(view_temperature), TRUE, TRUE, 0);
	slope_view_set_figure(SLOPE_VIEW(view_temperature), figure_temperature);
	slope_figure_add_scale(SLOPE_FIGURE(figure_temperature), scale_temperature);
    axis_temperature = slope_xyscale_get_axis(SLOPE_XYSCALE(scale_temperature), SLOPE_XYSCALE_AXIS_BOTTOM);
	sampler_temperature = slope_xyaxis_get_sampler(SLOPE_XYAXIS(axis_temperature));
    slope_sampler_set_samples(sampler_temperature, slope_sampler_array, NLABEL);
    slope_xyscale_set_x_range(SLOPE_XYSCALE(scale_temperature), -60, 0);
    g_signal_handlers_disconnect_by_data(view_temperature, GINT_TO_POINTER(SLOPE_MOUSE_MOVE)); // REMOVE DIAGRAM MOVEMENT ON SWIPE


    figure_pressure = slope_figure_new();
	scale_pressure = slope_xyscale_new_axis("Time [s]", "Value [bar]", "Pressure");
	view_pressure   = slope_view_new();
	gtk_box_pack_start(GTK_BOX(box_pressure), GTK_WIDGET(view_pressure), TRUE, TRUE, 0);
	slope_view_set_figure(SLOPE_VIEW(view_pressure), figure_pressure);
	slope_figure_add_scale(SLOPE_FIGURE(figure_pressure), scale_pressure);
    axis_pressure = slope_xyscale_get_axis(SLOPE_XYSCALE(scale_pressure), SLOPE_XYSCALE_AXIS_BOTTOM);
	sampler_pressure = slope_xyaxis_get_sampler(SLOPE_XYAXIS(axis_pressure));
    slope_sampler_set_samples(sampler_pressure, slope_sampler_array, NLABEL);
    slope_xyscale_set_x_range(SLOPE_XYSCALE(scale_pressure), -60, 0);
    g_signal_handlers_disconnect_by_data(view_pressure, GINT_TO_POINTER(SLOPE_MOUSE_MOVE)); // REMOVE DIAGRAM MOVEMENT ON SWIPE
    
    // ------------------------ 
	

	// ------------------------ DELETE SIGNAL HANDLER MOUSE MOVING
	
    g_signal_handlers_disconnect_by_data(view_temperature, GINT_TO_POINTER(SLOPE_MOUSE_MOVE));

    // ------------------------


}

PtMonitorView::~PtMonitorView() {

    gtk_widget_destroy(window);
    gtk_widget_destroy(stack);
    gtk_widget_destroy(togglebottom1);
    gtk_widget_destroy(togglebottom2);
    gtk_widget_destroy(togglebottom3);
    gtk_widget_destroy(box_temperature);
    gtk_widget_destroy(box_pressure);
    g_object_unref(G_OBJECT(builder));
    

}

void PtMonitorView::setPage(const int page_number, const GtkStackTransitionType transition) {

    GtkToggleButton* togglebottom1 = GTK_TOGGLE_BUTTON(this->togglebottom1);
	GtkToggleButton* togglebottom2 = GTK_TOGGLE_BUTTON(this->togglebottom2);
	GtkToggleButton* togglebottom3 = GTK_TOGGLE_BUTTON(this->togglebottom3);
    static std::string PAGE[] = {"pagina1", "pagina2", "pagina3"};

    gtk_stack_set_visible_child_full(GTK_STACK(stack), PAGE[page_number - 1].c_str(), transition);

    switch(page_number) {

        case 1:
			gtk_toggle_button_set_active(togglebottom1, TRUE);
			gtk_toggle_button_set_active(togglebottom2, FALSE);
			gtk_toggle_button_set_active(togglebottom3, FALSE);
            break;

        case 2:
            gtk_toggle_button_set_active(togglebottom1, FALSE);
			gtk_toggle_button_set_active(togglebottom2, TRUE);
			gtk_toggle_button_set_active(togglebottom3, FALSE);
            break;

        case 3:
			gtk_toggle_button_set_active(togglebottom1, FALSE);
			gtk_toggle_button_set_active(togglebottom2, FALSE);
			gtk_toggle_button_set_active(togglebottom3, TRUE);
            break;
    }
}

void PtMonitorView::setShoutdownHandler(void (*callback)(void)) { 
    shutdownHandler = callback; 
} 

void PtMonitorView::setSwipeHandler(void (*callback)(gdouble v_x, gdouble v_y)) {
    swipeHandler = callback; 
}



void PtMonitorView::startRoutine(void) const {

    // SHOW MAIN WINDOW
	gtk_widget_show_all(window);

	// CURSOR (TO DELETE ON TOUCH SCREEN) 
	gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_ARROW));

	// START GTK MAIN ROUTINE
	gtk_main();
}

int PtMonitorView::getCurrentPageNumber(void) const {

    const gchar *name = gtk_widget_get_name(gtk_stack_get_visible_child(GTK_STACK(stack)));

    if(strcmp(name, "pagina1") == 0)        return 1;
    else if(strcmp(name, "pagina2") == 0)   return 2;
    else if(strcmp(name, "pagina3") == 0)   return 3;
    else                                    return 1; // CURRENT PAGE IS 1 IF ERROR
}

int PtMonitorView::getNumberOfPages(void) {
    return NUMBER_OF_PAGES;
}

void PtMonitorView::plotData(const DataType* data, const MeasureType graph, const int nelem) {

    SlopeScale* scale;
    SlopeItem* series;
    GtkWidget* view;
    static std::string colors[] = {"b-", "r-", "y-", "g-"};
    double ymin;
    double ymax;

    if(graph == TEMPERATURE) {
        scale = scale_temperature;
        series = series_temperature;
        view = view_temperature;
        ymin = 0.0;
        ymax = 150.0;
    }
    else if(graph == PRESSURE) {
        scale = scale_pressure;
        series = series_pressure;
        view = view_pressure;
        ymin = 0.0;
        ymax = 5.0;
    }

    // GRAPH PLOT FOR ALL WHEELS
    for(size_t i = 0; i < 4; i++) {
		slope_scale_remove_item_by_name(SLOPE_SCALE(scale), toString(data[i].tyre).c_str());
		series = slope_xyseries_new_filled(toString(data[i].tyre).c_str(), data[i].x, data[i].y, nelem, colors[i].c_str());
		slope_scale_add_item(scale, series);
		slope_xyscale_set_x_range(SLOPE_XYSCALE(scale), -60, 0);
        slope_xyscale_set_y_range(SLOPE_XYSCALE(scale), ymin, ymax);
		slope_view_redraw(SLOPE_VIEW(view));
    }
}

void PtMonitorView::setMeasureValues(float value, MeasureType measure, const int axis, const int tyre) {

    GtkWidget *tyreLabel;

    if(measure == MeasureType::TEMPERATURE) {
        tyreLabel = interface_labels_temperature[axis][tyre];
    }

    else if(measure == MeasureType::PRESSURE) {
       tyreLabel = interface_labels_pressure[axis][tyre];
    }

    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << value << toUnit(measure);
    gtk_label_set_label(GTK_LABEL(tyreLabel), stream.str().c_str());
    
}