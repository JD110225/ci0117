#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "Monitor.h"


struct table {
   Monitor redonda;
};

int estudiante( int cual, Monitor * mesa ) {
   int espera;
   mesa->ComprarAlmuerzo(cual);
   espera = random() & 0x2;
   sleep( espera );
   mesa->Almorzar(cual);
   espera = random() & 0x2;
   sleep( espera );
   //Hacer que el estudiante 3(puede ser cualquiera) almuerce lento ,para que haya que esperarlo
   //(solo un caso de prueba)
   if(cual==3){
      sleep(espera);
   }
   mesa->Salir(cual);
   exit( 0 );
}



/*
 *
 */
int main( int argc, char ** argv ) {
   long workers;
   int worker, pid, memId;
   table * mesaEstudiantes;


   workers = 5;
   if ( argc > 1 ) {
      workers = atol( argv[ 1 ] );
   }
   Monitor * mesa = new Monitor(workers);

// Create shared memory area and copy object image to it
   memId = shmget( IPC_PRIVATE, sizeof( struct table ), IPC_CREAT | 0600 );
   if ( -1 == memId ) {
      perror( "Fail to create shared memory segment" );
      exit( 1 );
   }

   mesaEstudiantes = (struct table *) shmat( memId, NULL, 0 );
   memcpy( mesaEstudiantes, mesa, sizeof( Monitor ) );	

   for ( worker = 0; worker < workers; worker++ ) {
      pid = fork();
      if ( ! pid ) {
         estudiante( worker,  &mesaEstudiantes->redonda );
      }
   }

   for ( worker = 0; worker < workers; worker++ ) {
      int status;
      pid_t pid = wait( &status );
   }

   shmdt( mesaEstudiantes );
   shmctl( memId, IPC_RMID, NULL );

}