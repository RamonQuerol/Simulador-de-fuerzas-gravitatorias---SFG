
//#include "sfgMainWindow.h"
#include <stdio.h> //No hace falta si se pone el include de arriba
#include <stdlib.h> //Esta igual
#include "sfgSimulador.h"

//struct _SfgSimulador
//{
//};

//G_DEFINE_TYPE(SfgSimulador, sfg_simulador, G_TYPE_OBJECT);

float *masas = NULL;
float **posiciones;
float **velocidades;

//Estas dos variables no son necesarias ahora mismo, pero puede que cuando apliquemos las BLAS las necesitemos
//float **distancias; //medidos en metros
//float *vectores; //medidos en radianes
float **aceleraciones; //medida en metros/segundos^2

float ***fuerzas;

int numCuerposSimulados = 0;

//Añade cuerpos a la simulacion
int sfg_simulador_addCuerpos(int numero, Cuerpo *cuerpos){
    
    int i, coordenada;
    float *tempPointerVectores = NULL; //Puntero temporal para no perder antiguos punteros en caso de fallos con realloc
    float ** tempPointerMatrices = NULL; // Como fuerzas necesita llenar matrices, se necesita un tempPointer con tipo float **

    int numCuerposNuevo = numCuerposSimulados + numero;

    //Aumenta el tamaño del array que contiene las masas
    if((tempPointerVectores = (float *)realloc(masas, sizeof(float)*numCuerposNuevo))==NULL){
        return -1;   
    }
    masas = tempPointerVectores;

    

    //Para aquellas variables que tengan valores distintos segun la dirección, 
    //se debe obtener espacio tanto en el eje X como en el Y
    for(coordenada = 0; coordenada<2; ++coordenada){

        //Aumenta el tamaño del array que contiene las velocidades en el eje correspondiente
        if((tempPointerVectores = (float *)realloc(velocidades[coordenada], sizeof(float)*numCuerposNuevo))==NULL){
            return -1;   
        }
        velocidades[coordenada] = tempPointerVectores;


        //Aumenta el tamaño del array que contiene las posiciones del eje correspondiente 
        if((tempPointerVectores = (float *)realloc(posiciones[coordenada], sizeof(float)*numCuerposNuevo))==NULL){
            return -1;   
        }
        posiciones[coordenada] = tempPointerVectores;

        //Aumenta el tamaño del array que contiene las aceleraciones en el eje X durante la ejecución
        if((tempPointerVectores = (float *)realloc(aceleraciones[coordenada], sizeof(float)*numCuerposNuevo))==NULL){
            return -1;   
        }
        aceleraciones[coordenada] = tempPointerVectores;

        //Se aumenta la matriz de fuerzas

        //Aumenta el numero de columnas de la matriz
        if((tempPointerMatrices = (float **)realloc(fuerzas[coordenada], sizeof(float *)*numCuerposNuevo))==NULL){
            return -1;   
        }
        fuerzas[coordenada] = tempPointerMatrices;

        //Este for irá actualizando las antiguas filas para que tengan el tamaño nuevo
        for(i=0; i<numCuerposNuevo; ++i){
        
            if((tempPointerVectores = (float *)realloc(fuerzas[coordenada][i], sizeof(float)*numCuerposNuevo))==NULL){
                return -1;   
            }
            fuerzas[coordenada][i] = tempPointerVectores;
        
        }

    }


    for(i=numCuerposSimulados; i<numCuerposNuevo; i++){
        masas[i] = cuerpos[i].masa;
        velocidades[0][i] = cuerpos[i].velocidadX;
        velocidades[1][i] = cuerpos[i].velocidadY;
        posiciones[0][i] = cuerpos[i].posicionX;
        posiciones[1][i] = cuerpos[i].posicionY;
        printf("%f, %f, %f, %f, %f", masas[i], velocidades[0][i], velocidades[1][i], posiciones[0][i], posiciones[1][i]);
    }

    numCuerposSimulados = numCuerposNuevo;

    return 0;
}

//Libera toda la memoria del simulador
void
sfg_simulador_destroy(){
    if(numCuerposSimulados>0){
        int coordenada, i;
        
        free(masas);

        for(coordenada = 0; coordenada < 2; ++coordenada){
            free(posiciones[coordenada]);
            free(velocidades[coordenada]);
            free(aceleraciones[coordenada]);
            
            for(i = 0; i<numCuerposSimulados; ++i){
                free(fuerzas[coordenada][i]);
            }

            free(fuerzas[coordenada]);
        }

        free(posiciones);
        free(velocidades);
        free(aceleraciones);
        free(fuerzas);
    }
}

//Realiza un ciclo de la simulacion
void sfg_simular(int tiempo){

}

int sfg_simulador_init(){
    if(!(posiciones = malloc(sizeof(float **)*2))){
        perror("No se pudo inicializar la simulación debido a no tener memoria para guardar los punteros a posiciones");
        return -1;
    }

    if(!(velocidades = malloc(sizeof(float **)*2))){
        perror("No se pudo inicializar la simulación debido a no tener memoria para guardar los punteros a velocidades");
        return -1;
    }

    if(!(aceleraciones = malloc(sizeof(float **)*2))){
        perror("No se pudo inicializar la simulación debido a no tener memoria para guardar los punteros a las aceleraciones");
        return -1;
    }

    if(!(fuerzas = malloc(sizeof(float ***)*2))){
        perror("No se pudo inicializar la simulación debido a no tener memoria para guardar los punteros a las aceleraciones");
        return -1;
    }

    return 0;
}