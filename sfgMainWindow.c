#include <gtk/gtk.h>

#include "sfgApp.h"
#include "sfgMainWindow.h"
#include "sfgSimulador.h"

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


G_DEFINE_TYPE(SfgMainWindow, sfg_main_window, GTK_TYPE_APPLICATION_WINDOW);

int numCuerpos = 0;
int ancho;
int alto;
static cairo_surface_t *surface = NULL;
SfgMainWindow *win;
struct Circulo *miCirculo;          // puntero al array de circulos

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

  for (int i = 0; i < numCuerpos; i++)
  {
    cairo_set_source_rgb(cr, miCirculo[i].r, miCirculo[i].g, miCirculo[i].b);
    cairo_arc(cr, miCirculo[i].x * ancho, miCirculo[i].y * alto, miCirculo[i].masa, 0, 2 * G_PI);
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

  sfg_simulador_destroy();
  printf("Simulacion comenzada");
}

static void
finalizar_simulacion()
{
  // printf("Simulacion finalizada");
  // free(miCirculo);
  // miCirculo = NULL;
  // numCuerpos=0;
  // pintar_cuerpos();
  printf("Anade un cuerpo \n");
  // Creacion de cuerpo
  struct Circulo *tempPointer = NULL; // Puntero temporal para no perder antiguos punteros en caso de fallos con realloc
  int numCuerposNuevo = numCuerpos + 1;
  if ((tempPointer = (struct Circulo *)realloc(miCirculo, sizeof(struct Circulo)*numCuerposNuevo)) == NULL)
  {
    perror("error al hacer realloc");
    return;
  }
  
  struct Cuerpo *cuerpoSimulacion = NULL;//Es el cuerpo que se mandará al simulador para guardar sus caracteristicas
  
  if((cuerpoSimulacion = (struct Cuerpo *)malloc(sizeof(struct Cuerpo)))==NULL){
    perror("error al hacer malloc durante la creación de la variable temporal de cuerpo");
    return;
  }

  cuerpoSimulacion->masa = (float)(rand() % 20 + 5);
  cuerpoSimulacion->posicionX = (float)(rand() % 1000);
  cuerpoSimulacion->posicionY = (float)(rand() % 1000);
  cuerpoSimulacion->velocidadX = (float)(rand() % 500);
  cuerpoSimulacion->velocidadY = (float)(rand() % 500);

  sfg_simulador_addCuerpos(1, cuerpoSimulacion);

  miCirculo = tempPointer;

  //los valores los introduce el usuario
  miCirculo[numCuerpos].masa = cuerpoSimulacion->masa;
  miCirculo[numCuerpos].x = cuerpoSimulacion->posicionX/1000;
  miCirculo[numCuerpos].y = cuerpoSimulacion->posicionY/1000;

  miCirculo[numCuerpos].r = 0;
  miCirculo[numCuerpos].g = 0;
  miCirculo[numCuerpos].b = 0;

  numCuerpos++;

  free(cuerpoSimulacion);
  pintar_cuerpos();
}

static void
anadir_cuerpo()
{

  printf("Se anade conjunto de cuerpos \n");
  struct Circulo *tempPointer = NULL; // Puntero temporal para no perder antiguos punteros en caso de fallos con realloc
  srand((unsigned int)time(NULL));
  int numCuerposNuevo = numCuerpos + 5;

  // Creacion de array de circulos
  if ((tempPointer = (struct Circulo *)realloc(miCirculo, sizeof(struct Circulo) * numCuerposNuevo)) == NULL)
  {
    perror("error al hacer realloc");
    return;
  }

  miCirculo = tempPointer;
  // miCirculo = malloc(numCuerpos * sizeof(struct Circulo));
  for (int i = numCuerpos; i < numCuerposNuevo; i++)
  {
    miCirculo[i].masa = rand() % 20 + 5;
    miCirculo[i].x = ((double)rand() / RAND_MAX);
    miCirculo[i].y = ((double)rand() / RAND_MAX);

    miCirculo[i].r = (double)rand() / RAND_MAX;
    miCirculo[i].g = (double)rand() / RAND_MAX;
    miCirculo[i].b = (double)rand() / RAND_MAX;
  }
  numCuerpos = numCuerposNuevo;
  pintar_cuerpos();
}

//Libera la memoria contenida en la diversas parte del programa
static void sfg_main_window_destroy(){
  
  free(miCirculo);

  sfg_simulador_destroy();

  gtk_widget_dispose_template(win, SFG_MAIN_WINDOW_TYPE);
  gtk_window_destroy((GtkWindow *)win);
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
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), sfg_main_window_destroy);
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), resize_cb);
}

SfgMainWindow *
sfg_main_window_new(SfgApp *app)
{
  win = g_object_new(SFG_MAIN_WINDOW_TYPE, "application", app, NULL);
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(win->area), draw_cb, NULL, NULL);
  sfg_simulador_init();
  return win;
}

void sfg_main_window_open(SfgMainWindow *win,
                          GFile *file)
{
}
