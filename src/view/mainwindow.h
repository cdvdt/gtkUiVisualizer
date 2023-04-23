#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MAIN_TYPE_WINDOW main_window_get_type()
G_DECLARE_FINAL_TYPE(MainWindow, main_window, MAIN, WINDOW, GObject);

MainWindow *main_window_new(char *filename);

GtkWindow *main_window_get_window(MainWindow *self);

void main_window_show(MainWindow *self);

G_END_DECLS