#include <gtk/gtk.h>

#include "sfgApp.h"
#include "sfgMainWindow.h"

struct _SfgMainWindow
{
  GtkApplicationWindow parent;
};

G_DEFINE_TYPE(SfgMainWindow, sfg_main_window, GTK_TYPE_APPLICATION_WINDOW);

static void
sfg_main_window_init (SfgMainWindow *win)
{
  gtk_widget_init_template (GTK_WIDGET (win));
}

static void
sfg_main_window_class_init (SfgMainWindowClass *class)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class),
                                               "/com/git/sfg/mainWindow.ui");
}

SfgMainWindow *
sfg_main_window_new (SfgApp *app)
{
  return g_object_new (SFG_MAIN_WINDOW_TYPE, "application", app, NULL);
}

void
sfg_main_window_open (SfgMainWindow *win,
                         GFile            *file)
{
}
