#pragma once

#include <gtk/gtk.h>
#include "sfgApp.h"
#include "sfgSimulador.h"


#define SFG_MAIN_WINDOW_TYPE (sfg_main_window_get_type ())
G_DECLARE_FINAL_TYPE (SfgMainWindow, sfg_main_window, SFG, MAIN_WINDOW, GtkApplicationWindow)


SfgMainWindow       *sfg_main_window_new          (SfgApp *app);
void                    sfg_main_window_open         (SfgMainWindow *win,
                                                         GFile            *file);    
void                    add_cuerpos               (int num, int masaMin, int masaMax);
void                    add_cuerpo                (float masa, float posX, float posY, float velX, float velY, gchar *cadenaTam, gchar *cadenaColor);
void                    cambiarAjustes            (float tiempo, float distancia, gchar *unidadadesTiempo, gchar *unidadesDistancia, int nuevoCentroX, int nuevoCentroY);
