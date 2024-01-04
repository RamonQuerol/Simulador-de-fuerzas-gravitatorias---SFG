#include <gtk/gtk.h>

#include "sfgApp.h"
#include "sfgMainWindow.h"

struct _SfgMainWindow
{
  GtkApplicationWindow parent;

  GtkWidget *comenzarButton;
  GtkWidget *pararButton;
  GtkWidget *anadirButton;
};

G_DEFINE_TYPE(SfgMainWindow, sfg_main_window, GTK_TYPE_APPLICATION_WINDOW);

static void
comenzar_simulacion(){
  printf("Simulacion comenzada");
} 

static void
finalizar_simulacion(){
  printf("Simulacion finalizada");
}

static void
anadir_cuerpo(){
  printf("Se añadio un cuerpo");
}

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

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgMainWindow, comenzarButton);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgMainWindow, pararButton);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgMainWindow, anadirButton);

  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (class), comenzar_simulacion);
  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (class), finalizar_simulacion);
  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (class), anadir_cuerpo);
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
