#include "viewedwindow.h"
#include "../ui/generated/resources.h"

struct _ViewedWindow {
	GObject parent_instance;

	GFile *file;
	GFileMonitor *monitor;
	GtkWidget *design_root;
	GtkWindow *viewed_ui_window;
};

G_DEFINE_TYPE(ViewedWindow, viewed_window, G_TYPE_OBJECT)

static void viewed_window_init(ViewedWindow *self) {
	self->viewed_ui_window = NULL;
}

void viewed_window_show(ViewedWindow *self) {
	gtk_widget_show(GTK_WIDGET(self->viewed_ui_window));
}

GtkWindow *viewed_window_get_window(ViewedWindow *self) {
	return self->viewed_ui_window;
}

GtkBox *free_and_get_parent_design_root(ViewedWindow *self) {
	GtkWidget *design_root = self->design_root;
	GtkWidget *parent = gtk_widget_get_parent(design_root);
	gtk_widget_unparent(self->design_root);
	return GTK_BOX(parent);
}

void dispose_viewed_ui_window(ViewedWindow *self) {
	g_object_unref(self->viewed_ui_window);
}

GtkWidget *find_root_widget(GtkBuilder *builder) {
	GSList *objects = gtk_builder_get_objects(builder);
	GtkWidget *selected = NULL;

	for (GSList *node = objects; node != NULL; node = node->next) {
		GObject *object = G_OBJECT(node->data);
		if (GTK_IS_WIDGET(object)) {
			GtkWidget *current = GTK_WIDGET(object);
			GtkWidget *parent = gtk_widget_get_parent(current);
			if (parent == NULL) {
				selected = current;
				break;
			}
		}
	}

	g_slist_free(objects);
	return selected;
}

GtkBox *build_container_window(ViewedWindow *self) {
	GtkBuilder *container_builder = gtk_builder_new_from_resource("/org/cdvdt/gtkUiVisualizer/src/ui/defaultContainer.ui");

	GtkWindow *window = GTK_WINDOW(gtk_builder_get_object (container_builder, "window"));
	self->viewed_ui_window = GTK_WINDOW(window);
	GtkBox *container = GTK_BOX(gtk_builder_get_object (container_builder, "container"));
	g_object_unref(container_builder);

	return container;
}

void viewed_window_builder(ViewedWindow *self) {
	char *filename = g_file_get_path(self->file);
	GtkBuilder *builder = gtk_builder_new_from_file(filename);
	g_free(filename);

	int has_window = self->viewed_ui_window != NULL;

	/* Connect signal handlers to the constructed widgets. */
	GtkWidget *design_root = find_root_widget(builder);
	if(GTK_IS_WINDOW(design_root)) {
		if (has_window)
			dispose_viewed_ui_window(self);

		GtkWindow *window = GTK_WINDOW(design_root);
		self->viewed_ui_window = GTK_WINDOW(window);

		gtk_widget_show(GTK_WIDGET(window));
	} else {
		GtkBox *container;
		if (has_window)
			if(GTK_WIDGET(self->viewed_ui_window) == self->design_root)     {
				dispose_viewed_ui_window(self);

				container = build_container_window(self);
			}
			else
				container = free_and_get_parent_design_root(self);
		else
			container = build_container_window(self);

		gtk_box_append(container, design_root);

		gtk_widget_show(GTK_WIDGET(self->viewed_ui_window));
	}
	self->design_root = design_root;

	g_object_unref(builder);
}

void on_file_updated(GFileMonitor *monitor, GFile *file, GFile *other_file, GFileMonitorEvent event_type, gpointer user_data) {
	if (event_type == G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT) {
		ViewedWindow *self = VIEWED_WINDOW(user_data);
		g_print("updated, event %d\n", (int)event_type);
		self->file = file;
		viewed_window_builder(self);
	}
}

ViewedWindow *viewed_window_new(GFile *file) {
	ViewedWindow *window = g_object_new(VIEWED_TYPE_WINDOW, NULL);

	window->file = file;
	GFileMonitor *monitor = g_file_monitor_file(file, G_FILE_MONITOR_NONE, NULL, NULL);
	window->monitor = monitor;
	g_signal_connect(monitor, "changed", G_CALLBACK(on_file_updated), window);

	window->viewed_ui_window = NULL;

	viewed_window_builder(window);

	return window;
}

static void viewed_window_dispose(GObject *gobject) {
	ViewedWindow *self = VIEWED_WINDOW(gobject);
	dispose_viewed_ui_window(self);
	g_object_unref(self->monitor);
	G_OBJECT_CLASS (viewed_window_parent_class)->dispose (gobject);
}

static void viewed_window_finalize(GObject *gobject) {
	G_OBJECT_CLASS (viewed_window_parent_class)->finalize (gobject);
}

static void viewed_window_class_init(ViewedWindowClass *klass) {
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = viewed_window_dispose;
	object_class->finalize = viewed_window_finalize;
}