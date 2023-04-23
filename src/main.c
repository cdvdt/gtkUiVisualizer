#include <gtk/gtk.h>
#include "view/mainwindow.h"

static void activate (GtkApplication *app, gpointer user_data) {
	MainWindow *main_window = main_window_new(NULL);

	gtk_window_set_application (main_window_get_window(main_window), app);
	main_window_show(main_window);
}

int main (int argc, char **argv) {
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.cdvdt.gtkUiVisualizer", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}

