#pragma once


typedef struct Cuerpo
{
  float masa;
  float velocidadX;
  float velocidadY;
  float posicionX;
  float posicionY;
} Cuerpo;

int sfg_simulador_init();

int sfg_simulador_addCuerpos(int numero, struct Cuerpo *cuerpos);

void sfg_simulador_destroy();

int sfg_simular(double tiempo, Cuerpo *cuerpos);