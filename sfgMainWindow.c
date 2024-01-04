#include <gtk/gtk.h>

#include "sfgApp.h"
#include "sfgMainWindow.h"

struct _SfgMainWindow
{
  GtkApplicationWindow parent;

  GtkWidget *comenzarButton;
  GtkWidget *pararButton;
  GtkWidget *anadirButton;
  GtkWidget *area;
};

G_DEFINE_TYPE(SfgMainWindow, sfg_main_window, GTK_TYPE_APPLICATION_WINDOW);

static void
comenzar_simulacion()
{
  printf("Simulacion comenzada");
}

static void
finalizar_simulacion()
{
  printf("Simulacion finalizada");
}

static void
anadir_cuerpo()
{
  printf("Se añadio un cuerpo");
}

static void
sfg_main_window_init(SfgMainWindow *win)
{
  gtk_widget_init_template(GTK_WIDGET(win));
}

static void draw_cb(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {

    double radius = 50.0;
    double center_x = width / 2.0;
    double center_y = height / 2.0;

    cairo_set_source_rgb(cr, 0.100, 0.50, 0.32);  // Color negro
    cairo_set_line_width(cr, 2.0);

    cairo_arc(cr, center_x, center_y, radius, 0, 2 * G_PI);
    cairo_fill(cr);
    }

static void
sfg_main_window_class_init(SfgMainWindowClass *class)
{
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                              "/com/git/sfg/mainWindow.ui");

  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, comenzarButton);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, pararButton);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, anadirButton);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, area);

  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), comenzar_simulacion);
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), finalizar_simulacion);
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), anadir_cuerpo);
}

SfgMainWindow *
sfg_main_window_new(SfgApp *app)
{
  SfgMainWindow *win = g_object_new(SFG_MAIN_WINDOW_TYPE, "application", app, NULL);
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(win->area), draw_cb, NULL, NULL);

  return win;
}

void sfg_main_window_open(SfgMainWindow *win,
                          GFile *file)
{
}
