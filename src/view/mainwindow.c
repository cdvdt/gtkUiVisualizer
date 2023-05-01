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

static void main_window_init(MainWindow *self) {
	self->viewed_window = NULL;
}

void main_window_show(MainWindow *self) {
	gtk_widget_show (GTK_WIDGET (self->window));
}

void set_file(MainWindow *self, GFile *file) {
	if (file) {
		ViewedWindow *current = self->viewed_window;

		char *path = self->filename;
		if (path)
			g_free(path);

		path = g_file_get_path(file);
		self->filename = path;

		if (current)
			g_object_unref(current);

		gtk_editable_set_text(GTK_EDITABLE(self->entryFilename), path);

		current = viewed_window_new(file);
		gtk_widget_show(GTK_WIDGET(viewed_window_get_window(current)));

		self->viewed_window = current;
	}
}

void set_file_from_filename (MainWindow *self, char *filename) {
	if (filename)
		set_file(self, g_file_new_for_path(filename));
}

static void onSetFile(GtkWidget *widget, gint response_id, gpointer data) {
	if(response_id == GTK_RESPONSE_ACCEPT) {
		MainWindow *self = (MainWindow *) data;

		g_autoptr(GFile) file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(widget));
		set_file(self, file);
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
	set_file_from_filename(window, filename);

	return window;
}

static void main_window_dispose(GObject *gobject) {
	MainWindow *self = MAIN_WINDOW(gobject);
	ViewedWindow *viewed_window = self->viewed_window;

	if (viewed_window)
		g_object_unref(viewed_window);
	g_object_unref(self->window);

	G_OBJECT_CLASS (main_window_parent_class)->dispose (gobject);
}

static void main_window_finalize(GObject *gobject) {
	char *filename = (MAIN_WINDOW(gobject))->filename;
	if (filename)
		g_free(filename);
	G_OBJECT_CLASS (main_window_parent_class)->finalize (gobject);
}


static void main_window_class_init(MainWindowClass *klass) {
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = main_window_dispose;
	object_class->finalize = main_window_finalize;
}