
#include <gtk/gtk.h>
#include "sfgMainWindow.h"
#include "sfgSimulador.h"

struct _SfgSimulador
{
    float *masas;
    float *velocidades;
    float **posiciones;

};

G_DEFINE_TYPE(SfgSimulador, sfg_simulador, G_TYPE_OBJECT);

float *distancias; //medidos en metros
float *vectores; //medidos en radianes
float *aceleraciones; //medida en metros/segundos^2

float *fuerzas;

//Añade cuerpos a la simulacion
int sfg_simulador_addCuerpos(int numero, struct Cuerpo *cuerpos){
    
}

//Realiza un ciclo de la simulacion
void sfg_simular(int tiempo){

}