/**
 * Problema del comedor estudiantil
 * 
 * Author: Programacion Concurrente (Francisco Arroyo)
 * Version: 2020/Oct/02
 *
 * Primer examen parcial
 * Grupo-2
**/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "Monitor.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
using namespace std;
struct Monit{
   // int estudiantesSentados;
   Monitor m=Monitor(100);
};
struct Sentados{
   int sentados;
};
/**
 *  Código para los estudiantes
**/
int Estudiante( int cual,Monitor* m,Sentados* s ) {
   srand( getpid() );
   printf( "Se produjo un estudiante %d\n", cual );
   cout<<"Cantidad estudiantes comiendo: "<<s->sentados++<<endl;
   // cout<<"Estudiantes en mesa: "<<monit->estudiantesSentados<<endl;
   // m->ComprarAlmuerzo(cual);
   // m->Almorzar(cual);
   // m->Salir(cual);
   exit( 0 );

}


/*
 *
 */
int main( int argc, char ** argv ) {
   Monit *monit;
   // monit->estudiantesSentados=0;
   Sentados* sent=new Sentados();
   sent->sentados=0;
   long workers;
   int worker, pid, memId;
   memId = shmget( IPC_PRIVATE, sizeof(struct Monit), IPC_CREAT | 0600 );
   memId = shmget( IPC_PRIVATE, sizeof(struct Sentados), IPC_CREAT | 0600 );
   if ( -1 == memId ) {
      perror( "Fail to create shared memory segment" );
      exit( 1 );
   }
   monit = (struct Monit *) shmat( memId, NULL, 0 );
   sent= (struct Sentados *) shmat( memId, NULL, 0 );
   workers = 10;
   if ( argc > 1 ) {
      workers = atol( argv[ 1 ] );
   }

// Create students
   for ( worker = 0; worker < workers; worker++ ) {
      pid = fork();
      if ( ! pid ) {
         Estudiante( worker, & monit->m,sent);
      }
   }

   for ( worker = 0; worker < workers; worker++ ) {
      int status;
      pid_t pid = wait( &status );
   }
   shmdt( monit );
   shmctl( memId, IPC_RMID, NULL );
   shmdt( sent );
   shmctl( memId, IPC_RMID, NULL );
}

