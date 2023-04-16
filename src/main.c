#include <gtk/gtk.h>

static void openFile(GtkWidget *widget, gpointer data) {

}

static void activate (GtkApplication *app, gpointer user_data) {
	/* Construct a GtkBuilder instance and load our UI description */
	GtkBuilder *builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "ui/main.ui", NULL);

	/* Connect signal handlers to the constructed widgets. */
	GObject *window = gtk_builder_get_object (builder, "window");
	gtk_window_set_application (GTK_WINDOW (window), app);

	GObject *button = gtk_builder_get_object (builder, "openFile");
	g_signal_connect (button, "clicked", G_CALLBACK (openFile), NULL);

	/*   button = gtk_builder_get_object (builder, "button2");
	   g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

	   button = gtk_builder_get_object (builder, "quit");
	   g_signal_connect_swapped (button, "clicked", G_CALLBACK (quit_cb), window);*/

	gtk_widget_show (GTK_WIDGET (window));

	/* We do not need the builder any more */
	g_object_unref (builder);
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

