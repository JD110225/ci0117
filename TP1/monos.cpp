/*
 *  Ejemplo base para el problema de los monos
 *
 *  CI-0117 Programacion concurrente y paralela
 *  Fecha: 2020/Ago/24
 *
 */

/*
#include <string.h>
#include <sys/sem.h>
#include <time.h>
#include <stdbool.h>
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "SemaforoArray.h"
#define MONOS 4		// Cantidad de monos a crear en la manada
#define MaxEnCuerda 2		// Capacidad máxima de la cuerda
#define J 3	// Cantidad permitada antes de cambiar la dirección
#define DirIzqADer 1		// El mono cruza de izquierda a derecha
#define DirDerAIzq 2		// El mono cruza de derecha a izquierda
enum Direccion { IzqDer, DerIzq };
struct compartido {
    int dir;   //Direccion en la que estan cruzando los monos en un momento dado
    int cantidadEnCuerda=0;
    int cantidadPasados=0;
    bool empty=1;  //variable booleana que indica si la cuerda esta vacia
    int leftCounter=0; //Cantidad de monos que esperan para cruzar de izquierda a derecha
    int rightCounter=0; //Cantidad de monos que esperan para cruzar de derecha a izquierda
    int extraCounter=0; //Cantidad de monos que estan esperando porque la cuerda se encuentra llena
};
struct compartido * sharedData;
SemaforoArray s(3);
//Metodo usado para cambiar direccion en la que pueden cruzar la cuerda los monos
void changeDirections(){ 
   if(sharedData->dir==0){
      sharedData->dir=1;
   }
   else{
      sharedData->dir=0;
   }
}
//Metodo auxiliar que revisa si hay monos esperando en el sentido contrario del barranco
bool esperandoOtroSentido(){
   int d=(sharedData->dir==0)?1:0;
   int counter=(d==0)?sharedData->leftCounter:sharedData->rightCounter;
   return counter>0;
}
//Metodo auxiliar para darle acceso a los monos que esten esperando en sentido contrario
void darAccesoOtroSentido(){
   int dirSignal=(sharedData->dir==0)?1:0;
   int counterType=(dirSignal==0)?sharedData->leftCounter:sharedData->rightCounter;
   printf("No hay nadie en el barranco, cambio de direccion... \n");
   changeDirections();
   for(int i=0;i<counterType;++i){
      s.Signal(dirSignal);
   }
   --counterType;   
}
/*
 *  Método para representar a cada mono
 *  Recibe como parámetro la dirección en la que cada mono quiere cruzar
 *  Cada mono debe respetar las reglas impuestas en la definición del problema
 *
 */
int mono( int id, int dir ) {
    if(sharedData->empty){  //Si la cuerda esta vacia...
        sharedData->dir=dir;
        sharedData->empty=0;
    }
   switch ( dir  ) {
      case IzqDer:
         printf( "Mono %2d quiere cruzar de izquierda a derecha\n", id );
         break;
      case DerIzq:
         printf( "Mono %2d quiere cruzar de derecha a izquierda\n", id );
         break;
   }
   bool puedeCruzar=false;
   while(!puedeCruzar){
      bool condicion1=sharedData->dir==dir;
      bool condicion2=sharedData->cantidadEnCuerda<MaxEnCuerda;
      if(condicion1 && condicion2){ 
         printf("\tAcceso del mono %2d%s",id," concedido\n");
         sharedData->cantidadEnCuerda++;     
         sharedData->cantidadPasados++;
         usleep(10);
         puedeCruzar=true;
      }
      else{
         if(!condicion1){
            puedeCruzar=false;
            printf("\tAcceso del mono %d denegado(direccion incorrecta)\n",id);
            if(dir==0){
               sharedData->leftCounter++;
            }
            else{
               sharedData->rightCounter++;
            }
            s.Wait(dir);
      }       
         else{
            printf("\tAcceso del mono %2d denegado(cuerda llena)\n",id);
            sharedData->extraCounter++;
            s.Wait(2);
         }
      }
   } 
   sharedData->cantidadEnCuerda--;
   printf("El mono %2d ha cruzado el barranco exitosamente!\n",id);       
   if(sharedData->cantidadEnCuerda<MaxEnCuerda && sharedData->extraCounter>0){
      s.Signal(2);
   }
   if(sharedData->cantidadEnCuerda==0 && esperandoOtroSentido()){     
      darAccesoOtroSentido();
   }
    exit( 0 );
}

int main( int argc, char ** argv ) {
   int m, monos, shmId, resultado;
   monos=0;

   shmId = shmget( IPC_PRIVATE, sizeof( struct compartido ), IPC_CREAT | 0600 );
   if ( -1 == shmId ) {
      perror( "main shared memory create" );
      exit( 1 );
   }

   sharedData = (struct compartido *) shmat( shmId, NULL, 0 );
   sharedData->empty=1;
   if ( (void *) -1 == sharedData ) {
      perror( "main Shared Memory attach" );
      exit( 1 );
   }

   if ( argc > 1 ) {
      monos = atoi( argv[ 1 ] );
   }
   if ( monos <= 0 ) {
      monos = MONOS;
   }

   printf( "Creando una manada de %d monos\n", monos );
   for ( m = 1; m <= monos; m++ ) {
      if ( ! fork()) {
         srandom( getpid() );
         if ( random() & 0x1 > 0 ) {	// Check for an odd or even number
            mono( m, IzqDer );
         } else {
            mono( m, DerIzq );
         }
         exit(0);
      }
   }
// Wait for all processes before destroy shared memory segment and finish
   for ( m = 0; m <= monos; m++ ) {
      wait( &resultado );
   }

   resultado = shmdt( sharedData );
   shmctl( shmId, IPC_RMID, 0 );

   return 0;

}
