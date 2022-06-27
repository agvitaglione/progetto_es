#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// SWIPE ACT
#define XACT 500
#define YLIMIT 200

// MACRO
#define SWIPE_RIGHT(v_x, v_y) (v_x >= XACT && abs(v_y) < YLIMIT)
#define SWIPE_LEFT(v_x, v_y) (v_x <= -XACT && abs(v_y) < YLIMIT)


GtkBuilder *builder;
GtkWidget *window;
GtkWidget *stack;
GtkWidget *togglebottomleft;
GtkWidget *togglebottomright;


extern "C" G_MODULE_EXPORT void on_button1_clicked (GtkButton *button1, GtkStack *stack) {
	//gtk_main_quit();
	const gchar *name = gtk_widget_get_name(gtk_stack_get_visible_child(stack));
	if(strcmp(name, "pagina2") == 0)
		gtk_stack_set_visible_child_name(stack, "pagina1");
	else
		gtk_stack_set_visible_child_name(stack, "pagina2");
}

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

int main(int argc, char* argv[]) {
	
	gtk_init(&argc, &argv);
	//g_object_set(gtk_settings_get_default(), "gtk-enable-animations", TRUE, NULL);
	
	builder = gtk_builder_new_from_file("interface.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack"));
	togglebottomleft = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_1"));
	togglebottomright = GTK_WIDGET(gtk_builder_get_object(builder, "circle_page_2"));
	
	// GESTURE
	GtkGesture *swipe = gtk_gesture_swipe_new(window);
	g_signal_connect(swipe, "swipe", G_CALLBACK(change_page), NULL);
	
	// CHIUSURA
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder, NULL);

	// FINESTRA
	gtk_widget_show_all(window);


	// CURSORE
	gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_ARROW));

	gtk_main();

	return 0;
}



