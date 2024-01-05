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
  double r;
  double g;
  double b;
};

struct Cuerpo
{
  float masa;
  float velocidad;
  float posicionX;
  float posicionY;
};

G_DEFINE_TYPE(SfgMainWindow, sfg_main_window, GTK_TYPE_APPLICATION_WINDOW);

int numCuerpos=0;
int ancho;
int alto;
static cairo_surface_t *surface = NULL;
SfgMainWindow *win;
struct Circulo *miCirculo;  // puntero al array de circulos
int anadido=0;


static void
sfg_main_window_init(SfgMainWindow *win)
{
  GtkBuilder *builder;
  GMenuModel *menu;

  gtk_widget_init_template(GTK_WIDGET(win));

  builder = gtk_builder_new_from_resource("/com/git/sfg/menuButtonOpciones.ui");
  menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));
  gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(win->menuButton), menu);
  g_object_unref(builder);
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
  clear_surface();
  cairo_t *cr = cairo_create(surface);
  cairo_set_line_width(cr, 2.0);
  
  for (int i = 0; i < numCuerpos; i++) {
    cairo_set_source_rgb(cr, miCirculo[i].r, miCirculo[i].g, miCirculo[i].b); 
    cairo_arc(cr, miCirculo[i].x*ancho, miCirculo[i].y*alto, miCirculo[i].masa, 0, 2 * G_PI);
    cairo_fill(cr);  
  }
  
  cairo_destroy(cr);
  gtk_widget_queue_draw(win->area);
}

static void draw_cb(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data)
{
 // printf("drawcb");
  ancho = width;
  alto = height;
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

      pintar_cuerpos();
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
  if(numCuerpos){
    free(miCirculo);
  }
  printf("Se anade conjunto de cuerpos");
  srand((unsigned int)time(NULL));

  //Creacion de array de circulos
  numCuerpos=5;
  miCirculo = malloc(numCuerpos * sizeof(struct Circulo));
  for (int i = 0; i < numCuerpos; i++) {
    miCirculo[i].masa=50;  
    miCirculo[i].x=((double)rand() / RAND_MAX);  
    miCirculo[i].y=((double)rand() / RAND_MAX); 

    miCirculo[i].r=(double)rand() / RAND_MAX;
    miCirculo[i].g=(double)rand() / RAND_MAX; 
    miCirculo[i].b=(double)rand() / RAND_MAX;    
  }
  anadido=1;
  pintar_cuerpos();
}

static void
sfg_main_window_class_init(SfgMainWindowClass *class)
{
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                              "/com/git/sfg/mainWindow.ui");

  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, comenzarButton);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, pararButton);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, anadirButton);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, menuButton);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), SfgMainWindow, area);

  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), comenzar_simulacion);
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), finalizar_simulacion);
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), anadir_cuerpo);
  // gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), pintar_cuerpo);
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
