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
#define NLABEL 3
static const SlopeSample slope_sampler_array[NLABEL] = {
    {-10, (char*) "10"},
    {-5, (char*) "5"},
    {0  , (char*) "0" },
};

//----------------------------------- CALLBACKS
// SWIPE HANDLER
void _changePageHandler(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y) {
    PtMonitorView* view = PtMonitorView::getInstance();
    view->swipeHandler(v_x, v_y);
}

// CONFIRM SHUTDOWN BUTTON HANDLER
gboolean _shutdownRequestHandler (GtkWidget *shutdown_button_box) {
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

void _usbButtonHandler(GtkWidget* button) {
    PtMonitorView* view = PtMonitorView::getInstance();
    view->usbButtonHandler(std::string(gtk_button_get_label(GTK_BUTTON(button))));
}

gboolean _usbReloadButtonHandler (GtkWidget *eventBox) {
    PtMonitorView* view = PtMonitorView::getInstance();
    view->usbReloadButtonHandler();
    return TRUE;
}


gboolean _usbReleaseButtonHandler (GtkWidget *eventBox) {
    PtMonitorView* view = PtMonitorView::getInstance();
    view->usbReleaseButtonHandler();
    return TRUE;
}

//-----------------------------------




//----------------------------------- GENERATE GRID

void generateGrid(GtkWidget* grid, GtkWidget ***interface_labels_temperature, GtkWidget ***interface_labels_pressure, GtkCssProvider *provider) {

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), true);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    GtkStyleContext *context;

    for(int i = 0; i < numberOfAxis; i++) {

        int ntyre = 0;

        for(int j = 0; j < 2; j++) {

            GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12); // BOX INTO GRID CELL
            gtk_grid_attach(GTK_GRID(grid), box, j, i, 1, 1);


            std::string label_box_str = "AXIS " + std::to_string(i + 1);
            label_box_str += j == 0 ? " L" : " R";
            GtkWidget* label_box = gtk_label_new(label_box_str.c_str());

            gtk_widget_set_name(label_box, "labelAxisFirstPage");
            context = gtk_widget_get_style_context(label_box);
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            gtk_widget_set_halign(label_box, GTK_ALIGN_CENTER);
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

            GtkWidget* lt;
            GtkWidget* lp;

            for(int k = 0; k < numberOfTyrePerAxis / 2; k++) {

                lt = gtk_label_new("0°C");;
                lp = gtk_label_new("0 mbar");

                interface_labels_temperature[i][ntyre] = lt;
                interface_labels_pressure[i][ntyre] = lp;

                gtk_widget_set_name(lt, "labelValue");
                gtk_widget_set_name(lp , "labelValue");

                context = gtk_widget_get_style_context(lt);
                gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
                context = gtk_widget_get_style_context(lp);
                gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

                gtk_box_pack_start(GTK_BOX(box_temperature), lt, false, false, 0);
                gtk_box_pack_start(GTK_BOX(box_pressure), lp, false, false, 0);

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
    SlopeScale ***scale_pressure,
    GtkCssProvider *provider)
{

    for(int axis = 0; axis < numberOfAxis; axis++) {
        
        for(int tyre = 0; tyre < numberOfTyrePerAxis; tyre++){

            // Create a grid for each axis
            GtkWidget* grid = gtk_grid_new();
            gtk_grid_set_column_homogeneous(GTK_GRID(grid), true);
            gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 10);
            
            // Title for the tyre
            std::string label_string = "Axis " + std::to_string(axis + 1) + " Tyre " + std::to_string(tyre + 1);
            GtkWidget* label = gtk_label_new(label_string.c_str());
            gtk_widget_set_name(label, "labelAxis");
            GtkStyleContext *context = gtk_widget_get_style_context(label);
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            

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
            slope_xyscale_set_x_range(SLOPE_XYSCALE(_scale_temperature), -10, 0);
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
            slope_xyscale_set_x_range(SLOPE_XYSCALE(_scale_pressure), -10, 0);
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

    // Lock for thread safe
    g_thread_init(NULL);
    gdk_threads_init();
    gdk_threads_enter();

    gtk_init(NULL, NULL);
	
	builder = gtk_builder_new_from_file(builder_file_name.c_str());
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack"));
	togglebottom1 = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_1"));
	togglebottom2 = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_2"));
    box_plot = GTK_WIDGET(gtk_builder_get_object(builder, "pagina2"));
    request_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "request_dialog"));
    shutdown_button_box = GTK_WIDGET(gtk_builder_get_object(builder, "shutdown_button_box"));
    box_popoverusb = GTK_WIDGET(gtk_builder_get_object(builder, "boxPopoverUSB"));

    realease_button = GTK_WIDGET(gtk_builder_get_object(builder, "releaseButton"));
    reload_button = GTK_WIDGET(gtk_builder_get_object(builder, "reloadButton"));


    // ------------------------ Style provider 

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "../style.css", NULL);
    
    // ------------------------


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
    
    generateGrid(box_first_page, interface_labels_temperature, interface_labels_pressure, provider);


    // ------------------------



    // ------------------------ CALLBACKS

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(shutdown_button_box, "button-release-event", G_CALLBACK(_shutdownRequestHandler), NULL);

    g_signal_connect(reload_button, "button-release-event", G_CALLBACK(_usbReloadButtonHandler), NULL);
    g_signal_connect(realease_button, "button-release-event", G_CALLBACK(_usbReleaseButtonHandler), NULL);

    // ------------------------
    

  	// ------------------------ ADD GESTURE LISTENER

	swipe = gtk_gesture_swipe_new(window);
	g_signal_connect(swipe, "swipe", G_CALLBACK(_changePageHandler), NULL);

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

    generatePlotDiagram(box_plot, view_temperature, scale_temperature, view_pressure, scale_pressure, provider);
    
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
    for(int i = 0; i < numberOfAxis; i++) {
        delete[] interface_labels_temperature[i];
        delete[]interface_labels_pressure[i];
        delete[] view_temperature[i];
        delete[] scale_temperature[i];
        delete[] series_temperature[i];
        delete[] view_pressure[i];
        delete[] scale_pressure[i];
        delete[] series_pressure[i];
    }

    delete[] interface_labels_temperature;
    delete[] interface_labels_pressure;
    delete[] view_temperature;
    delete[] scale_temperature; 
    delete[] series_temperature; 
    delete[] view_pressure;
    delete[] scale_pressure;
    delete[] series_pressure;
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

void PtMonitorView::setUsbButtonHandler(void (*callback)(std::string usb_name)) {
    usbButtonHandler = callback;
}

void PtMonitorView::setUsbReloadButtonHandler(void (*callback)(void)) {
    usbReloadButtonHandler = callback;
}

void PtMonitorView::setUsbReleaseButtonHandler(void (*callback)(void)) {
    usbReleaseButtonHandler = callback;
}


void PtMonitorView::startRoutine(void) const {

    // SHOW MAIN WINDOW
    // gtk_window_fullscreen(GTK_WINDOW(window));
	gtk_widget_show_all(window);

	// CURSOR (TO COMMENT FOR TOUCH SCREEN) 
	//gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_ARROW));

	// START GTK MAIN ROUTINE
	gtk_main();

    // Unlock
    gdk_threads_leave();
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
    SlopeItem** series;
    GtkWidget* view;
    double ymin;
    double ymax;

    if(graph == TEMPERATURE) {
        scale = scale_temperature[axis][tyre];
        series = &series_temperature[axis][tyre];
        view = view_temperature[axis][tyre];
        ymin = 0.0;
        ymax = 150.0;
    }
    else if(graph == PRESSURE) {
        scale = scale_pressure[axis][tyre];
        series = &series_pressure[axis][tyre];
        view = view_pressure[axis][tyre];
        ymin = 0.0;
        ymax = 2000.0;
    }
    
    gdk_threads_enter();

    if(nelem == 1) {
        *series = slope_xyseries_new_filled("Data", data.x, data.y, nelem, "b-");
        slope_scale_add_item(scale, *series);
        slope_xyscale_set_x_range(SLOPE_XYSCALE(scale), -10, 0);
        slope_xyscale_set_y_range(SLOPE_XYSCALE(scale), ymin, ymax);
    }
    else {
        slope_xyseries_set_data(SLOPE_XYSERIES(*series), data.x, data.y, nelem);
        slope_view_redraw(SLOPE_VIEW(view));
    }

    gdk_threads_leave();

}

void PtMonitorView::setMeasureValues(int value, MeasureType measure, const int axis, const int tyre) {

    GtkWidget *tyreLabel;

    if(measure == MeasureType::TEMPERATURE) {
        tyreLabel = interface_labels_temperature[axis][tyre];
    }

    else if(measure == MeasureType::PRESSURE) {
        tyreLabel = interface_labels_pressure[axis][tyre];
    }

    gdk_threads_enter();
    gtk_label_set_label(GTK_LABEL(tyreLabel), (std::to_string(value) + toUnit(measure)).c_str());
    gdk_threads_leave();
}

void PtMonitorView::addUSB(std::string usb_name) {

    // If the usb_name is already in the list, don't do anything
    if(usb_list.count(usb_name) > 0) {
        return;
    }


    GtkWidget* button = gtk_button_new();
    usb_list.insert(std::pair<std::string, GtkWidget*>(usb_name, button));
    gtk_button_set_label(GTK_BUTTON(button), usb_name.c_str());
    gtk_widget_set_name(button, "usbLabel");
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(_usbButtonHandler), NULL);

    gtk_widget_show(button);
    
    gtk_box_pack_start(GTK_BOX(box_popoverusb), button, TRUE, TRUE, 0);

}

void PtMonitorView::removeUSB(std::string usb_name) {

    // IF the usb_name is not in the list, don't do anything
    if(usb_list.count(usb_name) == 0) {
        return;
    }


    GtkWidget* button = usb_list[usb_name];
    usb_list.erase(usb_name);
    gtk_container_remove(GTK_CONTAINER(box_popoverusb), button);
    gtk_widget_destroy(button);

}

void PtMonitorView::removeAllUSB() {

    for(auto usb : usb_list) {

        GtkWidget* button = usb.second;
        gtk_container_remove(GTK_CONTAINER(box_popoverusb), button);
    }


    usb_list.clear();
}