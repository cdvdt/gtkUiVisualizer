#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define VIEWED_TYPE_WINDOW viewed_window_get_type()
G_DECLARE_FINAL_TYPE(ViewedWindow, viewed_window, VIEWED, WINDOW, GObject);

ViewedWindow *viewed_window_new(char *filename);

GtkWindow *viewed_window_get_window(ViewedWindow *self);

void viewed_window_show(ViewedWindow *self);

G_END_DECLS