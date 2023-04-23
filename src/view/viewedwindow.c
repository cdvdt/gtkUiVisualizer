#include "viewedwindow.h"
#include "../ui/generated/resources.h"

struct _ViewedWindow {
	GObject parent_instance;

	char *filename;
    GFileMonitor *monitor;
	GtkWindow *viewed_ui_window;
};

G_DEFINE_TYPE(ViewedWindow, viewed_window, G_TYPE_OBJECT)

static void viewed_window_class_init(ViewedWindowClass *klass) {
}

static void viewed_window_init(ViewedWindow *self) {
	self->viewed_ui_window = NULL;
}

void viewed_window_show(ViewedWindow *self) {
	gtk_widget_show (GTK_WIDGET (self->viewed_ui_window));
}

GtkWindow *viewed_window_get_window(ViewedWindow *self){
    return self->viewed_ui_window;
}

void viewed_window_builder(ViewedWindow *self) {
	GtkBuilder *builder = gtk_builder_new ();
	gtk_builder_add_from_file(builder, self->filename, NULL);

	/* Connect signal handlers to the constructed widgets. */
	GObject *window = gtk_builder_get_object (builder, "window");
	self->viewed_ui_window = GTK_WINDOW (window);

	gtk_widget_show (GTK_WIDGET (window));

	g_object_unref (builder);
}

ViewedWindow *viewed_window_new(char *filename){
    ViewedWindow *window = g_object_new(VIEWED_TYPE_WINDOW, NULL);

    window->filename = filename;

	viewed_window_builder(window);

	return window;
}

static void viewed_window_dispose (GObject *gobject) {
    g_object_unref(VIEWED_WINDOW(gobject)->viewed_ui_window);
}

static void viewed_window_finalize (GObject *gobject) {
    g_free(VIEWED_WINDOW(gobject)->filename);
}