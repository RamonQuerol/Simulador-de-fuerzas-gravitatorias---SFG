#include <gtk/gtk.h>

#include "sfgApp.h"
#include "sfgMainWindow.h"
#include "sfgAucWindow.h"
#include "sfgSimulador.h"

//Objeto representativo de la ventana
struct _SfgMainWindow
{
  GtkApplicationWindow parent;

  GtkWidget *comenzarButton;
  GtkWidget *pararButton;
  GtkWidget *anadirButton;
  GtkWidget *menuButton;
  GtkWidget *area;
};


//Tipo utilizado para guardar datos necesarios para pintar en pantalla los cuerpos
//En ningún caso se utiliza para el calculo de la posición en proximos ciclos
struct Circulo
{
  int tam;
  double x;
  double y;
  double r;
  double g;
  double b;
};


//Definición del objeto representante de la ventana
G_DEFINE_TYPE(SfgMainWindow, sfg_main_window, GTK_TYPE_APPLICATION_WINDOW);



//Variables de proposito variado
int ancho;
int alto;
SfgMainWindow *win;

//Variables para el dibujado de cuerpos
static cairo_surface_t *surface = NULL;
int numCuerpos = 0;
struct Circulo *miCirculo; // puntero al array de circulos

//Variables para el mantenimiento de la simulacion
int simulacionActivada = 0;  // Variable global que activa o desactiva la simulacion
GMainContext *context;
static GMutex mutexCirculos;


//Variables con los ajustes personalizables de la simulacion
float tiempoPorCiclo = 1;
float distanciaRealPantalla = 1000;

int centroX = 0;
int centroY = 0;
int contadorCiclos = 0;


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



/////////////// OPERACIONES DE PINTADO DE VENTANA ///////////////


// Borra el contenido de la ventana
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

  g_mutex_lock(&mutexCirculos);
  for (int i = 0; i < numCuerpos; i++)
  {
    cairo_set_source_rgb(cr, miCirculo[i].r, miCirculo[i].g, miCirculo[i].b);
    cairo_arc(cr, miCirculo[i].x * ancho, miCirculo[i].y * alto, miCirculo[i].tam, 0, 2 * G_PI);
    cairo_fill(cr);
  }
  g_mutex_unlock(&mutexCirculos);
  cairo_destroy(cr);
  gtk_widget_queue_draw(win->area);
}




static void draw_cb(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data)
{
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


/////////////// OPERACIONES DE AJUSTES ///////////////



void cambiarAjustes(float tiempo, float distancia, gchar *unidadesTiempo, gchar *unidadesDistancia, int nuevoCentroX, int nuevoCentroY){

  //Ajustes de tiempo
  if(g_strcmp0(unidadesTiempo, "segundos") == 0){
    tiempoPorCiclo = tiempo;
  }else if(g_strcmp0(unidadesTiempo, "minutos") == 0){
    tiempoPorCiclo = tiempo * 60;
  }else if(g_strcmp0(unidadesTiempo, "horas") == 0){
    tiempoPorCiclo = tiempo * 3600;
  }else if(g_strcmp0(unidadesTiempo, "dias") == 0){
    tiempoPorCiclo = tiempo * 86400;
  }else if(g_strcmp0(unidadesTiempo, "semanas") == 0){
    tiempoPorCiclo = tiempo * 604800;
  }else if(g_strcmp0(unidadesTiempo, "meses") == 0){
    tiempoPorCiclo = tiempo * 2.628E6;
  }else if(g_strcmp0(unidadesTiempo, "anio") == 0){
    tiempoPorCiclo = tiempo * 3.154E7;
  }


  //Ajustes de distancia
  if(g_strcmp0(unidadesDistancia, "metros")==0){
    distanciaRealPantalla = distancia;
  }else if(g_strcmp0(unidadesDistancia, "kilometros") == 0){
    distanciaRealPantalla = distancia*1000;
  }

  //Ajustes de centro
  centroX = nuevoCentroX;
  centroY = nuevoCentroY;
  
  printf("------------------------------------------ \n");
  printf("AJUSTES DE SIMULACION: \n");
  printf("Tiempo por ciclo %.1f segundos \n",tiempoPorCiclo);
  printf("Distancia Real %.1f metros \n",distanciaRealPantalla);
  printf("CentroX: %d \n",centroX);
  printf("CentroY: %d \n",centroY);
  printf("------------------------------------------ \n");

  pintar_cuerpos();
}



/////////////// OPERACIONES DE AÑADIDO DE DATOS ///////////////


void add_cuerpo(float masa, float posX, float posY, float velX, float velY, gchar *cadenaTam, gchar *cadenaColor)
{
  printf("Anade un cuerpo  \n");

  // Creacion de cuerpo
  struct Circulo *tempPointer = NULL; // Puntero temporal para no perder antiguos punteros en caso de fallos con realloc
  int numCuerposNuevo = numCuerpos + 1;
  if ((tempPointer = (struct Circulo *)realloc(miCirculo, sizeof(struct Circulo) * numCuerposNuevo)) == NULL)
  {
    perror("error al hacer realloc");
    return;
  }

  struct Cuerpo *cuerpoSimulacion = NULL; // Es el cuerpo que se mandará al simulador para guardar sus caracteristicas

  if ((cuerpoSimulacion = (struct Cuerpo *)malloc(sizeof(struct Cuerpo))) == NULL)
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

  if (g_strcmp0(cadenaTam, "Muy Pequenio") == 0)
  {
    miCirculo[numCuerpos].tam = 2;
  }
  else if (g_strcmp0(cadenaTam, "Pequenio") == 0)
  {
    miCirculo[numCuerpos].tam = 5;
  }
  else if ((g_strcmp0(cadenaTam, "Normal") == 0))
  {
    miCirculo[numCuerpos].tam = 10;
  }
  else if ((g_strcmp0(cadenaTam, "Grande") == 0))
  {
    miCirculo[numCuerpos].tam = 15;
  }
  else
  {
    miCirculo[numCuerpos].tam = 20;
  }

  if (g_strcmp0(cadenaColor, "Verde") == 0)
  {
    miCirculo[numCuerpos].r = 0;
    miCirculo[numCuerpos].g = 1;
    miCirculo[numCuerpos].b = 0;
  }
  else if (g_strcmp0(cadenaColor, "Rojo") == 0)
  {
    miCirculo[numCuerpos].r = 1;
    miCirculo[numCuerpos].g = 0;
    miCirculo[numCuerpos].b = 0;
  }
  else if ((g_strcmp0(cadenaColor, "Amarillo") == 0))
  {
    miCirculo[numCuerpos].r = 1;
    miCirculo[numCuerpos].g = 1;
    miCirculo[numCuerpos].b = 0;
  }
  else if ((g_strcmp0(cadenaColor, "Azul") == 0))
  {
    miCirculo[numCuerpos].r = 0;
    miCirculo[numCuerpos].g = 0;
    miCirculo[numCuerpos].b = 1;
  }
  else if (g_strcmp0(cadenaColor, "Naranja") == 0)
  {
    miCirculo[numCuerpos].r = 1;
    miCirculo[numCuerpos].g = 0.5;
    miCirculo[numCuerpos].b = 0;
  }
  else if ((g_strcmp0(cadenaColor, "Morado") == 0))
  {
    miCirculo[numCuerpos].r = 0.5;
    miCirculo[numCuerpos].g = 0;
    miCirculo[numCuerpos].b = 1;
  }
  else if ((g_strcmp0(cadenaColor, "Rosa") == 0))
  {
    miCirculo[numCuerpos].r = 1;
    miCirculo[numCuerpos].g = 0.5;
    miCirculo[numCuerpos].b = 0.5;
  }
  else if (g_strcmp0(cadenaColor, "Gris") == 0)
  {
    miCirculo[numCuerpos].r = 0.5;
    miCirculo[numCuerpos].g = 0.5;
    miCirculo[numCuerpos].b = 0.5;
  }
  else if ((g_strcmp0(cadenaColor, "Marron") == 0))
  {
    miCirculo[numCuerpos].r = 0.6;
    miCirculo[numCuerpos].g = 0.4;
    miCirculo[numCuerpos].b = 0.2;
  }
  else
  {
    miCirculo[numCuerpos].r = 0;
    miCirculo[numCuerpos].g = 0;
    miCirculo[numCuerpos].b = 0;
  }

  // los valores los introduce el usuario
  miCirculo[numCuerpos].x = (cuerpoSimulacion->posicionX - centroX) / distanciaRealPantalla;
  miCirculo[numCuerpos].y = (cuerpoSimulacion->posicionY - centroY) / distanciaRealPantalla;

  numCuerpos++;

  free(cuerpoSimulacion);
  pintar_cuerpos();
}







void add_cuerpos(int numCuerposAdd, int masaMin, int masaMax)
{

  printf("Se anade conjunto de cuerpos \n");
  
  
  struct Circulo *tempPointer = NULL; // Puntero temporal para no perder antiguos punteros en caso de fallos con realloc
  srand((unsigned int)time(NULL));

  int i, j;  
  int numCuerposNuevo = numCuerpos + numCuerposAdd;


  // Creacion de array de circulos
  if ((tempPointer = (struct Circulo *)realloc(miCirculo, sizeof(struct Circulo) * numCuerposNuevo)) == NULL)
  {
    perror("error al hacer realloc");
    return;
  }

  Cuerpo *cuerpos;

  if ((cuerpos = (Cuerpo *)malloc(sizeof(Cuerpo) * numCuerposAdd)) == NULL)
  {
    perror("Error al obtener la memoria para las variables temporales de add_cuerpos");
    return;
  }

  

  int pseudoMax;
  
  if(masaMax == masaMin){
    pseudoMax = 1;
  } else{
    pseudoMax = masaMax - masaMin;
  }

  miCirculo = tempPointer;
  for (i = numCuerpos, j = 0; i < numCuerposNuevo; ++i, ++j)
  {
    
    cuerpos[j].masa = rand() % pseudoMax + masaMin;
    cuerpos[j].posicionX = (float)(rand() %1000);
    cuerpos[j].posicionY = (float)(rand() %1000);
    cuerpos[j].velocidadX = 0;
    cuerpos[j].velocidadY = 0;

    miCirculo[i].tam = (cuerpos[j].masa/masaMax) * 20 + 1; 
    miCirculo[i].x = (double)((cuerpos[j].posicionX - centroX)/distanciaRealPantalla);
    miCirculo[i].y = (double)((cuerpos[j].posicionY - centroY)/distanciaRealPantalla);

    miCirculo[i].r = (double)rand() / RAND_MAX;
    miCirculo[i].g = (double)rand() / RAND_MAX;
    miCirculo[i].b = (double)rand() / RAND_MAX;
  }
  sfg_simulador_addCuerpos(numCuerposAdd, cuerpos);
  numCuerpos = numCuerposNuevo;
  pintar_cuerpos();

  free(cuerpos);
}






static void
anadir_cuerpo()
{
  //Si la simulación está activa no se pueden añadir cuerpos
  if(simulacionActivada){
    return;
  }

  SfgAucWindow *auc = sfg_auc_window_new(SFG_MAIN_WINDOW(win));
  gtk_window_set_default_size(GTK_WINDOW(auc), 400, 00);
  gtk_window_present(GTK_WINDOW(auc));
}




/////////////// OPERACIONES MONITORIZACION DE SIMULACION ///////////////




static void printCiclosPorMinuto(){
  if(simulacionActivada){
    printf("La simulacion realiza %d por minuto\n", contadorCiclos*2);
    contadorCiclos = 0;
  }
}


static gpointer
comenzar_simulacion_thread(gpointer datos)
{

  Cuerpo *cuerposSimulacion = datos;
  int i;
  GSource *source = g_idle_source_new();

  // En caso de que al simular haya un choque, la última posicon del array miCirculo deberá ir a esta posicion
  int numMasaACambiar;

  contadorCiclos = 0;

  g_source_set_callback(source, pintar_cuerpos, NULL, NULL);

  g_source_attach(source, context);

  while (simulacionActivada)
  {

    if ((numMasaACambiar = sfg_simular(tiempoPorCiclo, cuerposSimulacion)))
    {
      g_mutex_lock(&mutexCirculos);

      miCirculo[numMasaACambiar].tam = miCirculo[numCuerpos-1].tam;
      miCirculo[numMasaACambiar].x = miCirculo[numCuerpos-1].x;
      miCirculo[numMasaACambiar].y = miCirculo[numCuerpos-1].y;
      miCirculo[numMasaACambiar].r = miCirculo[numCuerpos-1].r;
      miCirculo[numMasaACambiar].b = miCirculo[numCuerpos-1].b;
      miCirculo[numMasaACambiar].g = miCirculo[numCuerpos-1].g;

      --numCuerpos;

      // Creacion de array de circulos
      if ((miCirculo = (struct Circulo *)realloc(miCirculo, sizeof(struct Circulo) * numCuerpos)) == NULL)
      {
        perror("error al hacer realloc");
        return NULL;
      }

      g_mutex_unlock(&mutexCirculos);
    }
    else
    {
      for (i = 0; i < numCuerpos; ++i)
      {
        miCirculo[i].x = (cuerposSimulacion[i].posicionX - centroX) / distanciaRealPantalla;
        miCirculo[i].y = (cuerposSimulacion[i].posicionY - centroY) / distanciaRealPantalla;
      }
    }

    ++contadorCiclos;
  }

  free(cuerposSimulacion);
  return NULL;
}






// Funciones que se llaman al hacer click en botones
static void
comenzar_simulacion()
{

  printf("Simulacion comenzada\n");

  GThread *thread;

  Cuerpo *cuerposSimulacion;

  if ((cuerposSimulacion = malloc(sizeof(Cuerpo) * numCuerpos)) == NULL)
  {
    perror("No se pudo iniciar la simulacion ya que no hay suficiente memoria para guardar los resultados");
    return;
  }
  simulacionActivada = 1;

  thread = g_thread_new("simulacion", comenzar_simulacion_thread, cuerposSimulacion);

  g_thread_unref(thread);
}






static void
finalizar_simulacion()
{
  printf("Simulacion parada\n");
  simulacionActivada = 0;
}











/////////////// OPERACIONES DE CREACION Y DESTRUCCION DE LA VENTANA ///////////////



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
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), sfg_main_window_destroy);
  gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(class), resize_cb);
}






SfgMainWindow *
sfg_main_window_new(SfgApp *app)
{
  win = g_object_new(SFG_MAIN_WINDOW_TYPE, "application", app, NULL);
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(win->area), draw_cb, NULL, NULL);
  sfg_simulador_init();

  context = g_main_context_get_thread_default();
  g_mutex_init(&mutexCirculos);

  GSource *source = g_timeout_source_new(30000);
  g_source_set_callback(source, printCiclosPorMinuto, NULL, NULL);
  g_source_attach(source, context);

  return win;
}






void sfg_main_window_open(SfgMainWindow *win,
                          GFile *file)
{
}
