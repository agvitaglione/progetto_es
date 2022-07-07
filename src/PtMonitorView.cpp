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
    {-60, (char*) "60"},
    {-40, (char*) "40"},
    {-20, (char*) "20"},
    {0  , (char*) "0" },
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
                interface_labels_pressure[i][ntyre] = gtk_label_new("0 mbar");
                gtk_box_pack_start(GTK_BOX(box_temperature), interface_labels_temperature[i][ntyre], false, false, 0);
                gtk_box_pack_start(GTK_BOX(box_pressure), interface_labels_pressure[i][ntyre], false, false, 0);
                ntyre++;
            }
            
            // ---------------------------

        }
    }
}

void generatePlotDiagram (
    GtkWidget *box,
    GtkWidget ***view_temperature,
    SlopeScale ***scale_temperature,
    GtkWidget ***view_pressure,
    SlopeScale ***scale_pressure)
{

    for(int axis = 0; axis < numberOfAxis; axis++) {
        
        for(int tyre = 0; tyre < numberOfTyrePerAxis; tyre++){

            // Create a grid for each axis
            GtkWidget* grid = gtk_grid_new();
            gtk_grid_set_column_homogeneous(GTK_GRID(grid), true);
            gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 10);
            
            // Title for the tyre
            std::string label_string = "Axis " + std::to_string(axis) + " Tyre " + std::to_string(tyre);
            GtkWidget* label = gtk_label_new(label_string.c_str());
            gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

            // ------------ Generate Graph Temperature

            SlopeFigure *figure_temperature = slope_figure_new();
            scale_temperature[axis][tyre] = slope_xyscale_new_axis("Previous samples", "Value [°C]", "Temperature");
            view_temperature[axis][tyre]   = slope_view_new();
            
            // Get value to semplify
            SlopeScale *_scale_temperature = scale_temperature[axis][tyre];
            GtkWidget *_view_temperature = view_temperature[axis][tyre];

            gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(_view_temperature), 0, 1, 1, 1);
            slope_view_set_figure(SLOPE_VIEW(_view_temperature), figure_temperature);
            slope_figure_add_scale(SLOPE_FIGURE(figure_temperature), _scale_temperature);
            SlopeItem *axis_temperature = slope_xyscale_get_axis(SLOPE_XYSCALE(_scale_temperature), SLOPE_XYSCALE_AXIS_BOTTOM);
            SlopeSampler *sampler_temperature = slope_xyaxis_get_sampler(SLOPE_XYAXIS(axis_temperature));
            slope_sampler_set_samples(sampler_temperature, slope_sampler_array, NLABEL);
            slope_xyscale_set_x_range(SLOPE_XYSCALE(_scale_temperature), -60, 0);
            g_signal_handlers_disconnect_by_data(_view_temperature, GINT_TO_POINTER(SLOPE_MOUSE_MOVE)); // REMOVE DIAGRAM MOVEMENT ON SWIPE

            //------------

            // ------------ Generate Graph Pressure

            SlopeFigure *figure_pressure = slope_figure_new();
            scale_pressure[axis][tyre] = slope_xyscale_new_axis("Previous samples", "Value [mbar]", "Pressure");
            view_pressure[axis][tyre]   = slope_view_new();
            
            // Get value to semplify
            SlopeScale *_scale_pressure = scale_pressure[axis][tyre];
            GtkWidget *_view_pressure = view_pressure[axis][tyre];

            gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(_view_pressure), 1, 1, 1, 1);
            slope_view_set_figure(SLOPE_VIEW(_view_pressure), figure_pressure);
            slope_figure_add_scale(SLOPE_FIGURE(figure_pressure), _scale_pressure);
            SlopeItem *axis_pressure = slope_xyscale_get_axis(SLOPE_XYSCALE(_scale_pressure), SLOPE_XYSCALE_AXIS_BOTTOM);
            SlopeSampler *sampler_pressure = slope_xyaxis_get_sampler(SLOPE_XYAXIS(axis_pressure));
            slope_sampler_set_samples(sampler_pressure, slope_sampler_array, NLABEL);
            slope_xyscale_set_x_range(SLOPE_XYSCALE(_scale_pressure), -60, 0);
            g_signal_handlers_disconnect_by_data(_view_pressure, GINT_TO_POINTER(SLOPE_MOUSE_MOVE)); // REMOVE DIAGRAM MOVEMENT ON SWIPE

            //------------
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
    box_plot = GTK_WIDGET(gtk_builder_get_object(builder, "pagina2"));
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

    view_temperature = new GtkWidget**[numberOfAxis];
    scale_temperature = new SlopeScale**[numberOfAxis];
    series_temperature = new SlopeItem**[numberOfAxis];
    view_pressure = new GtkWidget**[numberOfAxis];
    scale_pressure = new SlopeScale**[numberOfAxis];
    series_pressure = new SlopeItem**[numberOfAxis];

    for(int i = 0; i < numberOfAxis; i++) {
        view_temperature[i] = new GtkWidget*[numberOfAxis];
        scale_temperature[i] = new SlopeScale*[numberOfAxis];
        series_temperature[i] = new SlopeItem*[numberOfAxis];
        view_pressure[i] = new GtkWidget*[numberOfAxis];
        scale_pressure[i] = new SlopeScale*[numberOfAxis];
        series_pressure[i] = new SlopeItem*[numberOfAxis];
    }

    generatePlotDiagram(box_plot, view_temperature, scale_temperature, view_pressure, scale_pressure);
    
    // ------------------------ 
	

	// ------------------------ DELETE SIGNAL HANDLER MOUSE MOVING

    for(int axis = 0; axis < numberOfAxis; axis++) {
        for(int tyre = 0; tyre < numberOfTyrePerAxis; tyre++) {
            g_signal_handlers_disconnect_by_data(view_temperature[axis][tyre], GINT_TO_POINTER(SLOPE_MOUSE_MOVE));
            g_signal_handlers_disconnect_by_data(view_pressure[axis][tyre], GINT_TO_POINTER(SLOPE_MOUSE_MOVE));
        }
    }

    // ------------------------


}

PtMonitorView::~PtMonitorView() {

    gtk_widget_destroy(window);
    gtk_widget_destroy(stack);
    gtk_widget_destroy(togglebottom1);
    gtk_widget_destroy(togglebottom2);
    gtk_widget_destroy(togglebottom3);
    gtk_widget_destroy(box_plot);
    g_object_unref(G_OBJECT(builder));

    // TODO: Distruggere tutti i puntatori allocati
    

}

void PtMonitorView::setPage(const int page_number, const GtkStackTransitionType transition) {

    GtkToggleButton* togglebottom1 = GTK_TOGGLE_BUTTON(this->togglebottom1);
	GtkToggleButton* togglebottom2 = GTK_TOGGLE_BUTTON(this->togglebottom2);
    static std::string PAGE[] = {"pagina1", "pagina2"};

    gtk_stack_set_visible_child_full(GTK_STACK(stack), PAGE[page_number - 1].c_str(), transition);

    switch(page_number) {

        case 1:
			gtk_toggle_button_set_active(togglebottom1, TRUE);
			gtk_toggle_button_set_active(togglebottom2, FALSE);
            break;

        case 2:
            gtk_toggle_button_set_active(togglebottom1, FALSE);
			gtk_toggle_button_set_active(togglebottom2, TRUE);
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
    else                                    return -1; // CURRENT PAGE IS 1 IF ERROR
}

int PtMonitorView::getNumberOfPages(void) {
    return NUMBER_OF_PAGES;
}

void PtMonitorView::plotData(const DataType& data, const int nelem, const MeasureType graph, const int axis, const int tyre) {

    SlopeScale* scale;
    SlopeItem* series;
    GtkWidget* view;
    static std::string colors[] = {"b-", "r-", "y-", "g-"};
    double ymin;
    double ymax;

    if(graph == TEMPERATURE) {
        scale = scale_temperature[axis][tyre];
        series = series_temperature[axis][tyre];
        view = view_temperature[axis][tyre];
        ymin = 0.0;
        ymax = 150.0;
    }
    else if(graph == PRESSURE) {
        scale = scale_pressure[axis][tyre];
        series = series_pressure[axis][tyre];
        view = view_pressure[axis][tyre];
        ymin = 0.0;
        ymax = 5.0;
    }

    slope_scale_remove_item_by_name(SLOPE_SCALE(scale), "prova");
    series = slope_xyseries_new_filled("prova", data.x, data.y, nelem, "b-");
    slope_scale_add_item(scale, series);
    slope_xyscale_set_x_range(SLOPE_XYSCALE(scale), -60, 0);
    slope_xyscale_set_y_range(SLOPE_XYSCALE(scale), ymin, ymax);
    slope_view_redraw(SLOPE_VIEW(view));
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