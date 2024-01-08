#include <gtk/gtk.h>

#include "sfgApp.h"
#include "sfgMainWindow.h"
#include "sfgAmcWindow.h"
#include "sfgSettingsWindow.h"

struct _SfgApp
{
  GtkApplication parent;
};

G_DEFINE_TYPE(SfgApp, sfg_app, GTK_TYPE_APPLICATION);

static void
sfg_app_init(SfgApp *app)
{
}

static void
anadir_multiples_activated(GSimpleAction *action,
                           GVariant *parameter,
                           gpointer app)
{
  GtkWindow *win;

  win = gtk_application_get_active_window(GTK_APPLICATION(app));
  SfgAmcWindow *amc = sfg_amc_window_new(SFG_MAIN_WINDOW(win));
  gtk_window_set_default_size(GTK_WINDOW(amc), 400, 125);
  gtk_window_present(GTK_WINDOW(amc));
}

static void
ver_lista_activated(GSimpleAction *action,
                    GVariant *parameter,
                    gpointer app)
{
}

static void
ajustes_activated(GSimpleAction *action,
                  GVariant *parameter,
                  gpointer app)
{
  GtkWindow *win;
  
  win = gtk_application_get_active_window(GTK_APPLICATION(app));
  SfgSettingsWindow *settings = sfg_settings_window_new(SFG_MAIN_WINDOW(win));
  gtk_window_set_default_size(GTK_WINDOW(settings), 500, 125);
  gtk_window_present(GTK_WINDOW(settings));
}

static void
salir_activated(GSimpleAction *action,
                GVariant *parameter,
                gpointer app)
{
  g_application_quit(G_APPLICATION(app));
}

static GActionEntry app_entries[] =
    {
        {"anadirmultiples", anadir_multiples_activated, NULL, NULL, NULL},
        {"ajustes", ajustes_activated, NULL, NULL, NULL},
        {"lista", ver_lista_activated, NULL, NULL, NULL},
        {"salir", salir_activated, NULL, NULL, NULL}};

static void
sfg_app_startup(GApplication *app)
{
  G_APPLICATION_CLASS(sfg_app_parent_class)->startup(app);

  g_action_map_add_action_entries(G_ACTION_MAP(app),
                                  app_entries, G_N_ELEMENTS(app_entries),
                                  app);
}

static void
sfg_app_activate(GApplication *app)
{
  SfgMainWindow *win;

  win = sfg_main_window_new(SFG_APP(app));
  gtk_window_present(GTK_WINDOW(win));
}

static void
sfg_app_open(GApplication *app,
             GFile **files,
             int n_files,
             const char *hint)
{
  GList *windows;
  int i;
  SfgMainWindow *win;
  windows = gtk_application_get_windows(GTK_APPLICATION(app));
  if (windows)
    win = SFG_MAIN_WINDOW(windows->data);
  else
    win = sfg_main_window_new(SFG_APP(app));

  for (i = 0; i < n_files; i++)
    sfg_main_window_open(win, files[i]);

  gtk_window_present(GTK_WINDOW(win));
}

static void
sfg_app_class_init(SfgAppClass *class)
{
  G_APPLICATION_CLASS(class)->activate = sfg_app_activate;
  G_APPLICATION_CLASS(class)->open = sfg_app_open;
  G_APPLICATION_CLASS(class)->startup = sfg_app_startup;
}

SfgApp *
sfg_app_new(void)
{
  return g_object_new(SFG_APP_TYPE,
                      "application-id", "com.git.sfg",
                      "flags", G_APPLICATION_HANDLES_OPEN,
                      NULL);
}