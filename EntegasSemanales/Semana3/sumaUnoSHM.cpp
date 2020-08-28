#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <string.h>
#include "Semaforo.h"
#include <iostream>
using namespace std;
long AddOne( long * suma ) {
   int i;

   for ( i = 0; i < 1000; i++ ) {
      usleep( 1 );
      (* suma)++;			// Suma uno
   }
   return 0;
}


/*
  Serial test
*/
void SerialTest( int procesos, long * total ) {
   int i, proceso;

   for ( proceso = 0; proceso < procesos; proceso++ ) {

      for ( i = 0; i < 1000; i++ ) {
         (* total)++;			// Suma uno
      }

   }

}


/*
  Fork test
*/
int ForkTest( int procesos, long * total ) {
   Semaforo s(0);
   int proceso, pid;
   for ( proceso = 0; proceso < procesos; proceso++ ) {
      pid = fork();
      if ( ! pid ) {
         AddOne( total );
         s.signal();
         exit(0);

      }
      else{
         s.wait();
      }
   }

   for ( proceso = 0; proceso < procesos; proceso++ ) {
      int status;
      pid_t pid = wait( &status );
   }
   return pid;

}

/*
 *
 */
void startTimer( struct timeval * timerStart) {
   gettimeofday( timerStart, NULL );
}


/*
 *  time elapsed in ms
 */
double getTimer( struct timeval timerStart ) {
   struct timeval timerStop, timerElapsed;

   gettimeofday(&timerStop, NULL);
   timersub(&timerStop, &timerStart, &timerElapsed);
   return timerElapsed.tv_sec*1000.0+timerElapsed.tv_usec/1000.0;
}


int main( int argc, char ** argv ) {
    long t=0;
    long *tPointer=&t;
    int id= shmget( 123456, 1024, 0700 | IPC_CREAT );
    tPointer=(long*) shmat( id, NULL, 0 );	
    long procesos;
    int proceso;
    struct timeval timerStart;
    double used;
    procesos = 100;
    if ( argc > 1 ) {
        procesos = atol( argv[ 1 ] );
    }
    //VERSION CONCURRENTE
    startTimer( & timerStart );
    *tPointer = 0;
    ForkTest( procesos, tPointer );
    used = getTimer( timerStart );
    printf( "Concurrent   version:      Valor acumulado por %ld procesos es \033[91m %ld \033[0m en %f ms\n", procesos, *tPointer, used );
    shmdt( &tPointer );		// Se "despega" del area compartida
    shmctl( id, IPC_RMID, NULL );
    //VERSION SERIAL
    startTimer( & timerStart );
    t = 0;
    SerialTest( procesos, & t );
    used = getTimer( timerStart );
    printf( "Serial version:      Valor acumulado por %ld procesos es \033[91m %ld \033[0m en %f ms\n", procesos, t, used );



}
