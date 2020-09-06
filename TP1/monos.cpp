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
#define MONOS 10		// Cantidad de monos a crear en la manada
#define MaxEnCuerda 3		// Capacidad máxima de la cuerda
#define J 5	// Cantidad permitada antes de cambiar la dirección
#define DirIzqADer 1		// El mono cruza de izquierda a derecha
#define DirDerAIzq 2		// El mono cruza de derecha a izquierda
enum Direccion { IzqDer, DerIzq };
struct compartido {
    int dir;
    int cantidadEnCuerda=0;
    int cantidadPasados=0;
    bool empty=1;
};
struct compartido * sharedData;
SemaforoArray s(2);
void changeDirections(){
   if(sharedData->dir==0){
      sharedData->dir=1;
   }
   else{
      sharedData->dir=0;
   }
}
/*
 *  Método para representar a cada mono
 *  Recibe como parámetro la dirección en la que cada mono quiere cruzar
 *  Cada mono debe respetar las reglas impuestas en la definición del problema
 *
 */
int mono( int id, int dir ) {
    if(sharedData->empty){  //Si la cuerda esta vacia, no problem
        sharedData->dir=dir;
        sharedData->empty=0;
    }
   printf("Proceso %d: monos caminando desde %s: %d\n",id,(sharedData->dir)==0?" Izq ":" Der ",sharedData->cantidadEnCuerda);
   switch ( dir  ) {
      case IzqDer:
         printf( "Mono %2d quiere cruzar de izquierda a derecha\n", id );
         break;
      case DerIzq:
         printf( "Mono %2d quiere cruzar de derecha a izquierda\n", id );
         break;
   }
   // printf("Dir shared: %d\n Dir mono: %d",sharedData->dir,dir);
   bool condicion1=sharedData->dir==dir;
   bool condicion2=sharedData->cantidadPasados<J;
   bool condicion3=sharedData->cantidadEnCuerda<MaxEnCuerda;
   if(condicion1 && condicion2 && condicion3){
        printf("\tAcceso del mono %2d%s",id," concedido\n");
        sharedData->cantidadEnCuerda++;     
        sharedData->cantidadPasados++;
        usleep(10);
        printf("El mono %2d ha cruzado el barranco exitosamente!\n",id);       
        sharedData->cantidadEnCuerda--;
    }
    else{
         printf("\tAcceso del mono %2d%s",id," denegado\n");
         if(dir==0){
            // s.Wait(0);
         }
         else{
            // s.Wait(1);
         }
    }  
   //  if(sharedData->cantidadPasados==J){
   //     changeDirections();
   //    //  sharedData->dir=(sharedData->dir==0) ? 1: 0;
   //  }
   //  if(sharedData->cantidadEnCuerda==0){
   //     sharedData->empty=1;
   //  }

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
   for ( m = 0; m <= monos; m++ ) {
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
