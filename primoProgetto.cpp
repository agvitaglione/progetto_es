#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <slope/slope.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "time.h"
#include <thread>
#include <chrono>


/* SWIPE ACT
* GESTURE SPEED LIMIT FOR HORIZONTAL SWIPE RECOGNITION
*/
#define XACT 500
#define YLIMIT 300

// MACRO
#define SWIPE_RIGHT(v_x, v_y) (v_x >= XACT && abs(v_y) < YLIMIT) // CHECK IT'S A RIGHT SWIPE
#define SWIPE_LEFT(v_x, v_y) (v_x <= -XACT && abs(v_y) < YLIMIT) // CHECK IT'S A LEFT SWIPE

// GLOBAL WIDGET INTERFACE VARIABLES
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *stack;
GtkWidget *togglebottom1;
GtkWidget *togglebottom2;
GtkWidget *togglebottom3;
GtkWidget *request_dialog;


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

// HANDLER SHUTDOWN BUTTON
extern "C" gboolean shutdown_request_handler (GtkWidget *shutdown_button_box) {
	
	request_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "request_dialog"));
	int risultato = gtk_dialog_run(GTK_DIALOG(request_dialog));

	if(risultato == 1 /* SHUTDOWN */) {
		// SHUTDOWN
		// system("shutdown now");
		gtk_main_quit();
	}
	else if (risultato == 0 /* CANCEL */) {
		gtk_widget_hide(request_dialog);
	}
}

// SWIPE CALLBACK
void change_page(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y) {
	
	GtkStack* stack = GTK_STACK(::stack);
	GtkToggleButton* togglebottom1 = GTK_TOGGLE_BUTTON(::togglebottom1);
	GtkToggleButton* togglebottom2 = GTK_TOGGLE_BUTTON(::togglebottom2);
	GtkToggleButton* togglebottom3 = GTK_TOGGLE_BUTTON(::togglebottom3);
	
	const gchar *name = gtk_widget_get_name(gtk_stack_get_visible_child(stack));
	
	if(SWIPE_RIGHT(v_x, v_y)) {
		if(strcmp(name, "pagina2") == 0) {
			gtk_stack_set_visible_child_full(stack, "pagina1", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
			gtk_toggle_button_set_active(togglebottom1, TRUE);
			gtk_toggle_button_set_active(togglebottom2, FALSE);
			gtk_toggle_button_set_active(togglebottom3, FALSE);
		}
		if(strcmp(name, "pagina3") == 0) {
			gtk_stack_set_visible_child_full(stack, "pagina2", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
			gtk_toggle_button_set_active(togglebottom1, FALSE);
			gtk_toggle_button_set_active(togglebottom2, TRUE);
			gtk_toggle_button_set_active(togglebottom3, FALSE);
		}
	}
		
	if(SWIPE_LEFT(v_x, v_y)) {
		if(strcmp(name, "pagina1") == 0) {
			gtk_stack_set_visible_child_full(stack, "pagina2", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
			gtk_toggle_button_set_active(togglebottom1, FALSE);
			gtk_toggle_button_set_active(togglebottom2, TRUE);
			gtk_toggle_button_set_active(togglebottom3, FALSE);
		}
		if(strcmp(name, "pagina2") == 0) {
			gtk_stack_set_visible_child_full(stack, "pagina3", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
			gtk_toggle_button_set_active(togglebottom1, FALSE);
			gtk_toggle_button_set_active(togglebottom2, FALSE);
			gtk_toggle_button_set_active(togglebottom3, TRUE);
		}
	}
}


// CHART PLOT TEMPERATURE
void chart_plot_temperature() {
	figure_temperature = slope_figure_new();
	scale_temperature = slope_xyscale_new();
	view_temperature   = slope_view_new();
	gtk_box_pack_start(GTK_BOX(box_temperature), GTK_WIDGET(view_temperature), TRUE, TRUE, 0);
	slope_view_set_figure(SLOPE_VIEW(view_temperature), figure_temperature);
	slope_figure_add_scale(SLOPE_FIGURE(figure_temperature), scale_temperature);
	

	const long n = 100;
	double *x, *y;

	x = (double*) g_malloc(n * sizeof(double));
	y = (double*) g_malloc(n * sizeof(double));

	for(long i = 0; i < 100; i++) {
		x[i] = g_random_double();
		y[i] = g_random_double();
	}
	slope_view_redraw(SLOPE_VIEW(view_temperature));
	series_temperature = slope_xyseries_new_filled("Identical", x, y, 100, "kor");
	slope_scale_add_item(scale_temperature, series_temperature);

	// DELETE SIGNAL HANDLER MOUSE MOVING
	g_signal_handlers_disconnect_by_data(view_temperature, GINT_TO_POINTER(SLOPE_MOUSE_MOVE));

}


// thread
void task() {

	size_t MAX = 61;
	int pos_new_value = 0;
	int n = 0;
	double x_samples[MAX];
	double y_samples[MAX];
	double x[MAX];
	double y[MAX];

	int currentTime;
	while(true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		currentTime = (int)time(NULL);
		x_samples[pos_new_value] = (int)time(NULL);
		y_samples[pos_new_value] = (double) g_random_double() * 10;
		
		n = n == MAX ? MAX :  n + 1;

		int i = pos_new_value;
		for (int k = 0; k < n; k++) {
			x[k] = x_samples[i] - currentTime;
			y[k] = y_samples[i];
			i = i == 0 ? i = MAX - 1 : i - 1;
		}

		pos_new_value = (pos_new_value + 1) % MAX ;

		// Disegnare grafico
		slope_scale_remove_item_by_name(SLOPE_SCALE(scale_pressure), "P1");
		series_pressure = slope_xyseries_new_filled("P1", x, y, n, "b-");
		slope_scale_add_item(scale_pressure, series_pressure);
		slope_xyscale_set_x_range(SLOPE_XYSCALE(scale_pressure), -60, 0);
		slope_view_redraw(SLOPE_VIEW(view_pressure));
	}
}

// CHART PLOT pressure
void chart_plot_pressure() {
	figure_pressure = slope_figure_new();
	// scale_pressure = slope_xyscale_new();
	view_pressure   = slope_view_new();
	gtk_box_pack_start(GTK_BOX(box_pressure), GTK_WIDGET(view_pressure), TRUE, TRUE, 0);
	slope_view_set_figure(SLOPE_VIEW(view_pressure), figure_pressure);
	
	// slope_figure_add_scale(SLOPE_FIGURE(figure_pressure), scale_pressure);
	scale_pressure = slope_xyscale_new_axis("tempo", "pressione", "Valori di pressione");
	
	slope_figure_add_scale(SLOPE_FIGURE(figure_pressure), scale_pressure);
	SlopeItem *axis = slope_xyscale_get_axis(SLOPE_XYSCALE(scale_pressure), SLOPE_XYSCALE_AXIS_BOTTOM);
	SlopeSampler *sampler = slope_xyaxis_get_sampler(SLOPE_XYAXIS(axis));
	
	
	static const SlopeSample slope_sampler_array[] = {
		{-60, "60s"},
		{-40, "40s"},
		{-20, "20s"},
		{0, "0s"},
	};

	slope_sampler_set_samples(sampler, slope_sampler_array, 4);
	
	
	
	// DELETE SIGNAL HANDLER MOUSE MOVING
	g_signal_handlers_disconnect_by_data(view_pressure, GINT_TO_POINTER(SLOPE_MOUSE_MOVE));

}


int main(int argc, char* argv[]) {
	
	gtk_init(&argc, &argv);
	//g_object_set(gtk_settings_get_default(), "gtk-enable-animations", TRUE, NULL);
	
	builder = gtk_builder_new_from_file("interface.glade");
	GtkCssProvider *css = gtk_css_provider_new();
	gtk_css_provider_load_from_path(css, "style.css", NULL);
	GtkStyleContext *context;

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack"));
	togglebottom1 = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_1"));
	togglebottom2 = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_2"));
	togglebottom3 = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_3"));
	box_temperature = GTK_WIDGET(gtk_builder_get_object(builder, "pagina2"));
	box_pressure = GTK_WIDGET(gtk_builder_get_object(builder, "pagina3"));

	// context = gtk_widget_get_style_context(window);
	//gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_USER);
	//gtk_style_context_add_class(context, "dialog");

	// ADD GESTURE LISTENER
	GtkGesture *swipe = gtk_gesture_swipe_new(window);
	g_signal_connect(swipe, "swipe", G_CALLBACK(change_page), NULL);
	
	// ADD CHART
	chart_plot_temperature();
	chart_plot_pressure();

	// ADD DESTROY SIGNAL AT WINDOW CLOSING
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder, NULL);



	// SHOW MAIN WINDOW
	gtk_widget_show_all(window);


	// CURSOR (TO DELETE ON TOUCH SCREEN) 
	gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_ARROW));

	std::thread t(task);

	// START GTK MAIN ROUTINE
	gtk_main();

	return 0;
}



