#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <slope/slope.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


/* SWIPE ACT
* GESTURE SPEED LIMIT FOR HORIZONTAL SWIPE RECOGNITION
*/
#define XACT 500
#define YLIMIT 200

// MACRO
#define SWIPE_RIGHT(v_x, v_y) (v_x >= XACT && abs(v_y) < YLIMIT) // CHECK IT'S A RIGHT SWIPE
#define SWIPE_LEFT(v_x, v_y) (v_x <= -XACT && abs(v_y) < YLIMIT) // CHECK IT'S A LEFT SWIPE

// GLOBAL WIDGET INTERFACE VARIABLES
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *stack;
GtkWidget *togglebottomleft;
GtkWidget *togglebottomright;
GtkWidget *box;
SlopeFigure *figure;
GtkWidget *view;
SlopeScale *scale;
SlopeItem *series;

// SWIPE CALLBACK
void change_page(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y) {
	
	GtkStack* stack = GTK_STACK(::stack);
	GtkToggleButton* togglebottomleft = GTK_TOGGLE_BUTTON(::togglebottomleft);
	GtkToggleButton* togglebottomright = GTK_TOGGLE_BUTTON(::togglebottomright);
	
	const gchar *name = gtk_widget_get_name(gtk_stack_get_visible_child(stack));
	
	if(SWIPE_RIGHT(v_x, v_y)) {
		if(strcmp(name, "pagina2") == 0) {
			gtk_stack_set_visible_child_full(stack, "pagina1", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
			gtk_toggle_button_set_active(togglebottomleft, TRUE);
			gtk_toggle_button_set_active(togglebottomright, FALSE);
		}
	}
		
	if(SWIPE_LEFT(v_x, v_y)) {
		if(strcmp(name, "pagina1") == 0) {
			gtk_stack_set_visible_child_full(stack, "pagina2", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
			gtk_toggle_button_set_active(togglebottomleft, FALSE);
			gtk_toggle_button_set_active(togglebottomright, TRUE);
		}
	}
}
#include <iostream>

// CHART PLOT
void chart_plot() {
	figure = slope_figure_new();
	scale = slope_xyscale_new();
	view   = slope_view_new();
	gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(view), TRUE, TRUE, 0);
	slope_view_set_figure(SLOPE_VIEW(view), figure);
	slope_figure_add_scale(SLOPE_FIGURE(figure), scale);
	

	const long n = 100;
	double *x, *y;

	x = (double*) g_malloc(n * sizeof(double));
	y = (double*) g_malloc(n * sizeof(double));

	for(long i = 0; i < 100; i++) {
		x[i] = g_random_double();
		y[i] = g_random_double();
	}
	slope_view_redraw(SLOPE_VIEW(view));
	series = slope_xyseries_new_filled("Identical", x, y, 100, "kor");
	slope_scale_add_item(scale, series);

	// DELETE SIGNAL HANDLER MOUSE MOVING
	g_signal_handlers_disconnect_by_data(view, GINT_TO_POINTER(SLOPE_MOUSE_MOVE));

}


int main(int argc, char* argv[]) {
	
	gtk_init(&argc, &argv);
	//g_object_set(gtk_settings_get_default(), "gtk-enable-animations", TRUE, NULL);
	
	builder = gtk_builder_new_from_file("interface.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack"));
	togglebottomleft = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_1"));
	togglebottomright = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_2"));
	box = GTK_WIDGET(gtk_builder_get_object(builder, "pagina2"));
	// ADD GESTURE LISTENER
	GtkGesture *swipe = gtk_gesture_swipe_new(window);
	g_signal_connect(swipe, "swipe", G_CALLBACK(change_page), NULL);
	
	// ADD CHART
	chart_plot();

	// ADD DESTROY SIGNAL AT WINDOW CLOSING
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder, NULL);



	// SHOW MAIN WINDOW
	gtk_widget_show_all(window);


	// CURSOR (TO DELETE ON TOUCH SCREEN) 
	gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_ARROW));

	// START GTK MAIN ROUTINE
	gtk_main();

	return 0;
}



