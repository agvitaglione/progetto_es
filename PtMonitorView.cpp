#include "PtMonitorView.h"

PtMonitorView* view;


// HANDLER
// HANDLER SHUTDOWN BUTTON
gboolean shutdown_request_handler (GtkWidget *shutdown_button_box) {
	
	int risultato = gtk_dialog_run(GTK_DIALOG(view->request_dialog));

	if(risultato == 1 /* SHUTDOWN */) {
		view->shutdownHandler();
	}
	else if (risultato == 0 /* CANCEL */) {
		gtk_widget_hide(view->request_dialog);
	}

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

    view = this;
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(shutdown_button_box, "button-release-event", G_CALLBACK(shutdown_request_handler), NULL);
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

    gtk_stack_set_visible_child_full(GTK_STACK(stack), PAGE[page_number].c_str(), GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);

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

void PtMonitorView::setSwipeHandler(void (*callback)(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y)) {
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
}

int PtMonitorView::getNumberOfPages(void) {
    return NUMBER_OF_PAGES;
}