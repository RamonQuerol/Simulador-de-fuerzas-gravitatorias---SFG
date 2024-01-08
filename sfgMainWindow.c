#include <gtk/gtk.h>

#include "sfgApp.h"
#include "sfgMainWindow.h"
#include "sfgAucWindow.h"
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
struct Circulo *miCirculo; // puntero al array de circulos

int simulacionActivada = 0; //Variable global que activa o desactiva la simulacion

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
    //printf("He pintado en %f %f\n", miCirculo[i].x, miCirculo[i].y);
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

void add_cuerpo(float masa, float posX, float posY, float velX, float velY, gchar *cadenaTam)
{
  printf("Anade un cuerpo  \n");

  if (g_strcmp0(cadenaTam, "Pequenio") == 0) {

    } else if((g_strcmp0(cadenaTam, "Normal") == 0)) {

    }else if((g_strcmp0(cadenaTam, "Grande") == 0)) {

    }else{

    }

  // Creacion de cuerpo
  struct Circulo *tempPointer = NULL; // Puntero temporal para no perder antiguos punteros en caso de fallos con realloc
  int numCuerposNuevo = numCuerpos + 1;
  if ((tempPointer = (struct Circulo *)realloc(miCirculo, sizeof(struct Circulo) * numCuerposNuevo)) == NULL)
  {
    perror("error al hacer realloc");
    return;
  }

  struct Cuerpo *cuerpoSimulacion = NULL; // Es el cuerpo que se mandará al simulador para guardar sus caracteristicas

  if((cuerpoSimulacion = (struct Cuerpo *)malloc(sizeof(struct Cuerpo))) == NULL)
  {
    perror("error al hacer malloc durante la creación de la variable temporal de cuerpo");
    return;
  }

  cuerpoSimulacion->masa = masa;
  cuerpoSimulacion->posicionX = posX;
  cuerpoSimulacion->posicionY = posY;
  cuerpoSimulacion->velocidadX = velX;
  cuerpoSimulacion->velocidadY = velY;

  sfg_simulador_addCuerpos(1, cuerpoSimulacion);

  miCirculo = tempPointer;

  // los valores los introduce el usuario
  miCirculo[numCuerpos].masa = cuerpoSimulacion->masa;
  miCirculo[numCuerpos].x = cuerpoSimulacion->posicionX / 1000;
  miCirculo[numCuerpos].y = cuerpoSimulacion->posicionY / 1000;

  miCirculo[numCuerpos].r = 0;
  miCirculo[numCuerpos].g = 0;
  miCirculo[numCuerpos].b = 0;

  numCuerpos++;

  free(cuerpoSimulacion);
  pintar_cuerpos();
}

void add_cuerpos(int numCuerposAdd)
{

  printf("Se anade conjunto de cuerpos \n");
  struct Circulo *tempPointer = NULL; // Puntero temporal para no perder antiguos punteros en caso de fallos con realloc
  srand((unsigned int)time(NULL));
  int numCuerposNuevo = numCuerpos + numCuerposAdd;

  // Creacion de array de circulos
  if ((tempPointer = (struct Circulo *)realloc(miCirculo, sizeof(struct Circulo) * numCuerposNuevo)) == NULL)
  {
    perror("error al hacer realloc");
    return;
  }

  miCirculo = tempPointer;
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

// Funciones que se llaman al hacer click en botones
static void
comenzar_simulacion()
{
    //anadirGordo();

  printf("Simulacion comenzada");

  Cuerpo *cuerposSimulacion;
  int i;

  if ((cuerposSimulacion = malloc(sizeof(Cuerpo) * numCuerpos))==NULL)
  {
    perror("No se pudo iniciar la simulacion ya que no hay suficiente memoria para guardar los resultados");
    return;
  }
  simulacionActivada = 1;
  
  int cuentaCosas = 0;
  int segundoCuentaCosas = 0;


  while(cuentaCosas<100){

    

    sfg_simular(1000, cuerposSimulacion);


    //if(cuentaCosas >100){
      ++segundoCuentaCosas;
      //printf("%d ", segundoCuentaCosas);
    for(i = 0; i<numCuerpos; ++i){
      //miCirculo[i].masa = cuerposSimulacion[i].masa;
      miCirculo[i].x = cuerposSimulacion[i].posicionX/1000;
      miCirculo[i].y = cuerposSimulacion[i].posicionY/1000; 
    }

    pintar_cuerpos();
    //cuentaCosas = 0;
    //}

    cuentaCosas++;
  }
}


static void
finalizar_simulacion()
{
  printf("Simulacion finalizada");
}

static void
anadir_cuerpo()
{
  SfgAucWindow *auc = sfg_auc_window_new(SFG_MAIN_WINDOW(win));
  gtk_window_set_default_size(GTK_WINDOW(auc), 400,300);
  gtk_window_present(GTK_WINDOW(auc));
}

// Libera la memoria contenida en la diversas parte del programa
static void sfg_main_window_destroy()
{

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
