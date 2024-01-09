
// #include "sfgMainWindow.h"
#include <stdio.h>  //No hace falta si se pone el include de arriba
#include <stdlib.h> //Esta igual
#include <math.h>
#include "sfgSimulador.h"


//Datos que se guardan entre iteraciones 
float *masas = NULL;
float **posiciones;
float **velocidades;



// Estas dos variables no son necesarias ahora mismo, pero puede que cuando apliquemos las BLAS las necesitemos
// float **distancias; //medidos en metros
// float *vectores; //medidos en radianes


//Posiciones de memoria utilizadas durante la ejecución que combiene utilizar
float **aceleraciones; // medida en metros/segundos^2
float ***fuerzas;


int numCuerposSimulados = 0;


//Variable que evita que se intente inicializar el simulador otra vez, una vez ya inicializado
int sfg_simulador_inicializado = 0; 





// Añade cuerpos a la simulacion
int sfg_simulador_addCuerpos(int numero, Cuerpo *cuerpos)
{

    int i, posicionEnCuerpos, coordenada;
    float *tempPointerVectores = NULL;  // Puntero temporal para no perder antiguos punteros en caso de fallos con realloc
    float **tempPointerMatrices = NULL; // Como fuerzas necesita llenar matrices, se necesita un tempPointer con tipo float **

    int numCuerposNuevo = numCuerposSimulados + numero;

    // Aumenta el tamaño del array que contiene las masas
    if ((tempPointerVectores = (float *)realloc(masas, sizeof(float) * numCuerposNuevo)) == NULL)
    {
        return -1;
    }
    masas = tempPointerVectores;


    // Para aquellas variables que tengan valores distintos segun la dirección,
    // se debe obtener espacio tanto en el eje X como en el Y
    for (coordenada = 0; coordenada < 2; ++coordenada)
    {

        // Aumenta el tamaño del array que contiene las velocidades en el eje correspondiente
        if ((tempPointerVectores = (float *)realloc(velocidades[coordenada], sizeof(float) * numCuerposNuevo)) == NULL)
        {
            perror("Error al asignar la memoria en el simulador al anadir velocidades");
            return -1;
        }

        velocidades[coordenada] = tempPointerVectores;



        // Aumenta el tamaño del array que contiene las posiciones del eje correspondiente
        if ((tempPointerVectores = (float *)realloc(posiciones[coordenada], sizeof(float) * numCuerposNuevo)) == NULL)
        {
            perror("Error al asignar la memoria en el simulador al anadir posiciones");
            return -1;
        }
        posiciones[coordenada] = tempPointerVectores;


        // Aumenta el tamaño del array que contiene las aceleraciones en el eje X durante la ejecución
        if ((tempPointerVectores = (float *)realloc(aceleraciones[coordenada], sizeof(float) * numCuerposNuevo)) == NULL)
        {
            perror("Error al asignar la memoria en el simulador al anadir aceleraciones");
            return -1;
        }
        aceleraciones[coordenada] = tempPointerVectores;


        // Se aumenta la matriz de fuerzas

        // Aumenta el numero de columnas de la matriz
        if ((tempPointerMatrices = (float **)realloc(fuerzas[coordenada], sizeof(float *) * numCuerposNuevo)) == NULL)
        {
            perror("Error al asignar la memoria en el simulador al anadir fuerzas");
            return -1;
        }
        fuerzas[coordenada] = tempPointerMatrices;

        //Esto permite eliminar cuerpos en caso de que numero sea negativo
        if(numCuerposNuevo<numCuerposSimulados){
            numCuerposSimulados = numCuerposNuevo;
        }

        // Este for irá actualizando las antiguas filas para que tengan el tamaño nuevo
        for (i = 0; i < numCuerposSimulados; ++i)
        {

            if ((tempPointerVectores = (float *)realloc(fuerzas[coordenada][i], sizeof(float) * numCuerposNuevo)) == NULL)
            {
                perror("Error al asignar la memoria en el simulador al rehacer los arrays de fuerzas");
                return -1;
            }
            fuerzas[coordenada][i] = tempPointerVectores;
        }

        //En los nuevos se debe hacer malloc ya que no estan inicializados a NULL
        for(i = numCuerposSimulados; i<numCuerposNuevo; ++i){
            
            if ((fuerzas[coordenada][i] = (float *)malloc(sizeof(float) * numCuerposNuevo)) == NULL)
            {
                perror("Error al asignar la memoria en el simulador al crear los arrays de fuerzas");
                return -1;
            }

        }
    }


    posicionEnCuerpos = 0;

    for (i = numCuerposSimulados; i < numCuerposNuevo; i++, ++posicionEnCuerpos)
    {
        masas[i] = cuerpos[posicionEnCuerpos].masa;
        velocidades[0][i] = cuerpos[posicionEnCuerpos].velocidadX;
        velocidades[1][i] = cuerpos[posicionEnCuerpos].velocidadY;
        posiciones[0][i] = cuerpos[posicionEnCuerpos].posicionX;
        posiciones[1][i] = cuerpos[posicionEnCuerpos].posicionY;
    }

    numCuerposSimulados = numCuerposNuevo;

    return 0;
}







// Libera toda la memoria del simulador
void sfg_simulador_destroy()
{
    if (numCuerposSimulados > 0)
    {
        int coordenada, i;

        free(masas);

        for (coordenada = 0; coordenada < 2; ++coordenada)
        {
            free(posiciones[coordenada]);
            free(velocidades[coordenada]);
            free(aceleraciones[coordenada]);

            for (i = 0; i < numCuerposSimulados; ++i)
            {
                free(fuerzas[coordenada][i]);
            }

            free(fuerzas[coordenada]);
        }

        free(posiciones);
        free(velocidades);
        free(aceleraciones);
        free(fuerzas);

        printf("Simulador destruido");
    }
}








int sfg_simulador_fusionEntreCuerpos(int numCuerpo1, int numCuerpo2){
    
    int numFusion; //contiene la posición en los array que tendrá la fusion de los objetos
    int numNoEscogido; //contiene la posición en los array del que no será la posicion de la fusion

    float masaFusion;

    //La posicion de la fusion sera la de aquel con menor indice
    if(numCuerpo1<numCuerpo2){
        numFusion = numCuerpo1;
        numNoEscogido = numCuerpo2;
    }else{
        numFusion = numCuerpo2;
        numNoEscogido = numCuerpo1;
    }
    
    masaFusion = masas[numCuerpo1] + masas[numCuerpo2];
    //velocidades[0][numFusion] = masaFusion * (velocidades[0][numCuerpo1]/masas[numCuerpo1] + velocidades[0][numCuerpo2]/masas[numCuerpo2]);
    //velocidades[1][numFusion] = masaFusion * (velocidades[1][numCuerpo1]/masas[numCuerpo1] + velocidades[1][numCuerpo2]/masas[numCuerpo2]);
    velocidades[0][numFusion] = 0;
    velocidades[1][numFusion] = 0;
    
    masas[numFusion] = masaFusion;


    //Se moverá el último cuerpo a la posicion no escogida
    masas[numNoEscogido] = masas[numCuerposSimulados-1];
    velocidades[0][numNoEscogido] = velocidades[0][numCuerposSimulados-1];
    velocidades[1][numNoEscogido] = velocidades[1][numCuerposSimulados-1];
    posiciones[0][numNoEscogido] = posiciones[0][numCuerposSimulados-1];
    posiciones[1][numNoEscogido] = posiciones[1][numCuerposSimulados-1];

    sfg_simulador_addCuerpos(-1, NULL);
    return numNoEscogido;
}









// Realiza un ciclo de la simulacion
int sfg_simular(double tiempo, Cuerpo *cuerpos)
{

    int i, j; 
    float distancia, fuerzaVector;

    float vectorX, vectorY, tiempoCuadrado;


    //Calculo de fuerzas

    for(i = 0; i<numCuerposSimulados; ++i){
        
        //Estas fuerzas se calculan fuera del bucle porque aunque necesitan un valor
        //La distancia entre un planeta y el mismo es 0, por lo que si se alica la formula de gravitacion
        //se daria un caso de division entre 0
        fuerzas[0][i][i] = 0;
        fuerzas[1][i][i] = 0;
        
        for(j = i+1; j<numCuerposSimulados; ++j){
            

            //Calculo de la distancia entre los cuerpos sqrt((x1-x2)^2 + (y1-y2)^2)
            vectorX = abs(posiciones[0][i]-posiciones[0][j]);
            vectorY = abs(posiciones[1][i]-posiciones[1][j]);

            distancia = (float)sqrt(pow(vectorX,2)+pow(vectorY,2));
            
            if(distancia < 1 ){
                return sfg_simulador_fusionEntreCuerpos(i, j);
            }

            float constante = 6.672E-11 * masas[i] * masas[j];            
            //Se aplica la formula de gravedad de Newton (G*m1*m2)/(distancia^2)
            fuerzaVector = (constante) / (pow(distancia, 2)); 

            //Dado que fuerzaVector es la fuerza lineal, hay que transformarlo a forma vectorial
            fuerzas[0][i][j] = (float)(fuerzaVector*(vectorX/distancia)); //Fuerza en coordenada X
            fuerzas[0][j][i] = fuerzas[0][i][j];
            

            fuerzas[1][i][j] =  (float)(fuerzaVector*(vectorY/distancia)); //Fuerza en coordenada Y
            fuerzas[1][j][i] = fuerzas[1][i][j];

            //Los vectores de fuerza son contrarios para ambos cuerpos, por lo que solo uno de ellos debe ser cambiado
            //Antes de esta operación fuerza posee el signo de la coordenada mayor y la coordenada mayor siempre
            //se moverá en dirección negativa, por tanto, cambia en signo si la coordenada mayor es positiva
            
            if(posiciones[0][i]>posiciones[0][j]){
                fuerzas[0][i][j] = -fuerzas[0][i][j];
            }else{
                fuerzas[0][j][i] = -fuerzas[0][j][i];
            }

            if(posiciones[1][i]>posiciones[1][j]){
                fuerzas[1][i][j] = -fuerzas[1][i][j];
            }else{
                fuerzas[1][j][i] = -fuerzas[1][j][i];
            }
        }
    }

    //Suma de fuerzas y guardado de resultados

    //Se calculan variables constantes del bloque
    tiempoCuadrado = pow(tiempo, 2);

    for(i = 0; i<numCuerposSimulados; ++i){
        

        vectorX = 0.0;
        vectorY = 0.0;

        //Se suman todas las fuerzas en la misma variable
        for(j=0; j<numCuerposSimulados; ++j){
            vectorX += fuerzas[0][i][j];
            vectorY += fuerzas[1][i][j];
        }

        //Se calcula la aceleracion
        aceleraciones[0][i]= vectorX/masas[i];
        aceleraciones[1][i]= vectorY/masas[i];

        //Y a partir de ella se calcula tanto la nueva posicion como la nueva velocidad
        //Se ha calculado primero la posicion ya que posicion es dependiente de velocidad pero 
        //la velocidad no es dependiente de la posicion
        //Se ha aplicado la formula de movimiento rectilineo uniformemente acelerado X = X0 + v0*t + 0.5*a*(t^2)
        posiciones[0][i] = posiciones[0][i] + velocidades[0][i]*tiempo + 0.5*aceleraciones[0][i]*tiempoCuadrado;
        posiciones[1][i] = posiciones[1][i] + velocidades[1][i]*tiempo + 0.5*aceleraciones[1][i]*tiempoCuadrado;


        cuerpos[i].posicionX = posiciones[0][i];
        cuerpos[i].posicionY = posiciones[1][i];

        //La formula para la velocidad es v = v0 + a*t
        velocidades[0][i] = velocidades[0][i] + aceleraciones[0][i]*tiempo;
        velocidades[1][i] = velocidades[1][i] + aceleraciones[1][i]*tiempo;

        cuerpos[i].velocidadX = velocidades[0][i];
        cuerpos[i].velocidadY = velocidades[1][i];

        //Se pasa la masa por si han ocurrido cambios en esta
        cuerpos[i].masa = masas[i];
    }

    return 0;
    

}







//Método que se debe llamar antes de comenzar para inicializar los punteros principales
int sfg_simulador_init()
{
     //El if está para evitar que se inicialicen los punteros dos veces
     //si se vuelve a llamar la función por error
    if (!sfg_simulador_inicializado){
        if (!(posiciones = malloc(sizeof(float *) * 2)))
        {
            perror("No se pudo inicializar la simulación debido a no tener memoria para guardar los punteros a posiciones");
            return -1;
        }

        posiciones[0]= NULL;
        posiciones[1]= NULL;

        if (!(velocidades = malloc(sizeof(float *) * 2)))
        {
            perror("No se pudo inicializar la simulación debido a no tener memoria para guardar los punteros a velocidades");
            return -1;
        }

        velocidades[0]= NULL;
        velocidades[1]= NULL;

        if (!(aceleraciones = malloc(sizeof(float *) * 2)))
        {
            perror("No se pudo inicializar la simulación debido a no tener memoria para guardar los punteros a las aceleraciones");
            return -1;
        }

        aceleraciones[0] = NULL;
        aceleraciones[1] = NULL;

        if (!(fuerzas = malloc(sizeof(float **) * 2)))
        {
            perror("No se pudo inicializar la simulación debido a no tener memoria para guardar los punteros a las aceleraciones");
            return -1;
        }

        fuerzas[0] = NULL;
        fuerzas[1] = NULL;

        sfg_simulador_inicializado = 1;
    }
    return 0;
}