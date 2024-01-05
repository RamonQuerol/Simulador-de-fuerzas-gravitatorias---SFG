#include <gtk/gtk.h>

#include "sfgApp.h"
#include "sfgMainWindow.h"

struct _SfgMainWindow
{
  GtkApplicationWindow parent;

  GtkWidget *comenzarButton;
  GtkWidget *pararButton;
  GtkWidget *anadirButton;
  GtkWidget *menuButton;
  GtkWidget *area;
};

struct Circulo
{
  double masa;
  double x;
  double y;
};

struct Cuerpo{
  float masa;
  float velocidad;
  float posicionX;
  float posicionY;
};

G_DEFINE_TYPE(SfgMainWindow, sfg_main_window, GTK_TYPE_APPLICATION_WINDOW);

static cairo_surface_t *surface = NULL;
SfgMainWindow *win;



static void
sfg_main_window_init(SfgMainWindow *win)
{
  GtkBuilder *builder;
  GMenuModel *menu;

  gtk_widget_init_template(GTK_WIDGET(win));

  builder = gtk_builder_new_from_resource ("/com/git/sfg/menuButtonOpciones.ui");
  menu = G_MENU_MODEL (gtk_builder_get_object (builder, "menu"));
  gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (win->menuButton), menu);
  g_object_unref (builder);
}

// borra el contenido de la ventana
static void
clear_surface(void)
{
  cairo_t *cr;

  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  cairo_destroy(cr);
}

static void pintar_cuerpos()
{
  printf("dibujando cuerpo \n");
  cairo_t *cr = cairo_create(surface);

  // De momento aqui, pero cambiar a futuro
  struct Circulo miCirculo;
  
  miCirculo.masa = 50 ;
  miCirculo.x = 0;
  miCirculo.y = 0;

  cairo_set_line_width(cr, 2.0);

  cairo_set_source_rgb(cr, 0.100, 0.50, 0.32); // Color verde raro -> cambiar
  
  cairo_arc(cr, miCirculo.x, miCirculo.y, miCirculo.masa, 0, 2 * G_PI);
  cairo_fill(cr);

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_arc(cr, 100, miCirculo.y, miCirculo.masa, 0, 2 * G_PI);
  cairo_fill(cr);
  
  cairo_destroy(cr);
  gtk_widget_queue_draw(win->area);
}

static void draw_cb(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data)
{
  printf("drawcb");
  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_paint(cr);
}

// al modificar tamano de la ventana eliminando el contenido de esta
static void
resize_cb(GtkWidget *widget,
          int width,
          int height,
          gpointer data)
{
  if (surface)
  {
    cairo_surface_destroy(surface);
    surface = NULL;
  }

  if (gtk_native_get_surface(gtk_widget_get_native(widget)))
  {
    surface = gdk_surface_create_similar_surface(gtk_native_get_surface(gtk_widget_get_native(widget)),
                                                 CAIRO_CONTENT_COLOR,
                                                 gtk_widget_get_width(widget),
                                                 gtk_widget_get_height(widget));

    /* Initialize the surface to white */
    clear_surface();

  }
}



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
sfg_main_window_class_init(SfgMainWindowClass *class)
{
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                              "/com/git/sfg/mainWindow.ui");

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgMainWindow, comenzarButton);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgMainWindow, pararButton);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgMainWindow, anadirButton);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (class), SfgMainWindow, menuButton);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, area);

  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), comenzar_simulacion);
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), finalizar_simulacion);
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), anadir_cuerpo);
  //gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), pintar_cuerpo);
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), resize_cb);
}

SfgMainWindow *
sfg_main_window_new(SfgApp *app)
{
  win = g_object_new(SFG_MAIN_WINDOW_TYPE, "application", app, NULL);
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(win->area), draw_cb, NULL, NULL);
  return win;
}

void sfg_main_window_open(SfgMainWindow *win,
                          GFile *file)
{
}
