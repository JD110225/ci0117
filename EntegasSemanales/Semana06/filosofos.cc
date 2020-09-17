/*
 * Resuelve el problema de los filósofos utilizando un monitor "Mesa"
 * 
 * Author: Programacion Concurrente (Francisco Arroyo)
 * Version: 2020/Set/03
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <iostream>
using namespace std;
#include "Mesa.h"
struct FiloMesa {
   Mesa redonda;
};
Mesa * mesa = new Mesa();

void* Filosofo( void* cual) {
   
   int i;
   int eat, think;
   srand( getpid() );
   for ( i = 0; i < 10; i++ ) {	
      think = rand() & 0xfffff;
      usleep( think );
      mesa->pickup( *(int*)(&cual) );
      printf( "El filósofo %d está comiendo\n", cual );
      eat = rand() & 0xfffff;
      usleep( eat );
      mesa->putdown( *(int*)(&cual));
      printf( "El filósofo %d está pensando\n", cual );
      think = rand()  & 0xfffff;
      usleep( think );
   }
   pthread_exit(0);
}

int main( int argc, char ** argv ) {
   long hilos,hilo;
   hilos=5;
   pthread_t * pthilos;
   pthilos = (pthread_t *) calloc( hilos, sizeof( pthread_t ) );
   for ( hilo = 0; hilo < hilos; hilo++ ) {
      pthread_create( & pthilos[ hilo ], NULL, Filosofo, (void *) hilo );
   }   
   for ( hilo = 0; hilo < hilos; hilo++ ) {
      pthread_join( pthilos[ hilo ], NULL );
   }
   free(pthilos);
}

