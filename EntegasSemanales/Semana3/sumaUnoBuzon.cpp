#include <sys/msg.h>
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
#include "Buzon.h"
#define LABEL_SIZE 64
using namespace std;

struct msgbuff1
{
    long mtype; // message type, must be > 0
    long times;  // Times that label appears
    char label[64];
};
Buzon b;
struct msgbuff1 a;
int st;

long AddOne( long * suma ) {
   int i;
   for ( i = 0; i < 1000; i++ ) {
      // usleep( 1 );
      (* suma)++;			// Suma uno
   }
   b.Enviar("valor",a.times,1000);
   return 0;

}


/*   
  Serial test
        int st=msgrcv(id2,&rcv,sizeof(rcv),2020,IPC_NOWAIT);
        printf(" %d\n",rcv.dato);
*/
void SerialTest( int procesos, long * total ) {
   int i, proceso;

   for ( proceso = 0; proceso < 1; proceso++ ) {

      for ( i = 0; i < 1000; i++ ) {
         (* total)++;			// Suma uno
      }

   }

}
int ForkTest( int procesos) {
   Semaforo s(0);
    int proceso, pid;
   b.Enviar("inicial",0,1000);
   for ( proceso = 0; proceso < procesos; proceso++ ) {
      pid = fork();
      if ( ! pid ) {
         st=b.Recibir(&a,1000);
         AddOne( &a.times);
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
    ForkTest( procesos );
    b.Recibir(&a,1000);
    used = getTimer( timerStart );
    printf( "Concurrent   version:      Valor acumulado por %ld procesos es \033[91m %ld \033[0m en %f ms\n", procesos, a.times, used );
    //VERSION SERIAL
    startTimer( & timerStart );
    t = 0;
    SerialTest( procesos, & t );
    used = getTimer( timerStart );
    printf( "Serial version:      Valor acumulado por %ld procesos es \033[91m %ld \033[0m en %f ms\n", procesos, t, used );
}
