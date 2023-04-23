#include "mainwindow.h"
#include "viewedwindow.h"
#include "../ui/generated/resources.h"

struct _MainWindow {
	GObject parent_instance;

	char *filename;
	GtkWindow *window;
	ViewedWindow *viewed_window;
	GtkEntry *entryFilename;
};

G_DEFINE_TYPE(MainWindow, main_window, G_TYPE_OBJECT)

static void main_window_class_init(MainWindowClass *klass) {
}

static void main_window_init(MainWindow *self) {
}

void main_window_show(MainWindow *self) {
	gtk_widget_show (GTK_WIDGET (self->window));
}

void set_file(MainWindow *self, char *filename) {
	if (filename) {
		ViewedWindow *current = self->viewed_window;
		self->filename = filename;

		if (current)
			g_object_unref(current);
		
		gtk_editable_set_text(GTK_EDITABLE(self->entryFilename), filename);

		current = viewed_window_new(filename);
		gtk_widget_show(GTK_WIDGET(viewed_window_get_window(current)));

		self->viewed_window = current;
	}
}

static void onSetFile(GtkWidget *widget, gint response_id, gpointer data) {
	if(response_id == GTK_RESPONSE_ACCEPT) {
		MainWindow *self = (MainWindow *) data;

		g_autoptr(GFile) file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(widget));
		char *filename = g_file_get_path(file);
		set_file(self, filename);
		g_object_unref(file);
	}

	gtk_window_destroy(GTK_WINDOW(widget));
}

static void openFile(GtkWidget *widget, gpointer data) {
	MainWindow *self = (MainWindow *) data;

	GtkWidget *dialog = gtk_file_chooser_dialog_new("Open file", self->window, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
	GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.ui");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
	if (self->filename)
		gtk_file_chooser_set_file(GTK_FILE_CHOOSER(dialog), g_file_new_for_path(self->filename), NULL);
	g_signal_connect (dialog, "response", G_CALLBACK (onSetFile), self);
	gtk_window_present(GTK_WINDOW(dialog));
}

GtkWindow *main_window_get_window(MainWindow *self) {
	return self->window;
}

void main_window_builder(MainWindow *self) {
	GResource *resource = resouces_get_resource();
	GtkBuilder *builder = gtk_builder_new ();
	gtk_builder_add_from_resource(builder, "/org/cdvdt/gtkUiVisualizer/src/ui/main.ui", NULL);

	/* Connect signal handlers to the constructed widgets. */
	GObject *window = gtk_builder_get_object (builder, "window");

	GObject *button = gtk_builder_get_object (builder, "openFile");
	g_signal_connect (button, "clicked", G_CALLBACK (openFile), self);

	GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object (builder, "entryFilename"));

	self->entryFilename = entry;
	self->window = GTK_WINDOW(window);

	g_object_unref (builder);
}

MainWindow *main_window_new(char *filename) {
	MainWindow *window = g_object_new(MAIN_TYPE_WINDOW, NULL);

	main_window_builder(window);
	set_file(window, filename);

	return window;
}
