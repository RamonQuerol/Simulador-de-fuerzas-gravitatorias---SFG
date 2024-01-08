#pragma once


//#include "sfgMainWindow.h"

typedef struct Cuerpo
{
  float masa;
  float velocidadX;
  float velocidadY;
  float posicionX;
  float posicionY;
} Cuerpo;


//#define SFG_SIMULADOR_TYPE (sfg_simulador_get_type ())
//G_DECLARE_FINAL_TYPE (SfgSimulador, sfg_simulador, SFG, SIMULADOR, GObject)


int sfg_simulador_init();

int sfg_simulador_addCuerpos(int numero, struct Cuerpo *cuerpos);

void sfg_simulador_destroy();

int sfg_simular(double tiempo, Cuerpo *cuerpos);