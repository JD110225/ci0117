/*  PThreads
 *
/*  Esta clase encapsula las funciones para la utilizacion de barreras
 *  Una barrera detiene una cantidad definida de hilos, una vez alcanzado
 *  ese limite, todos los hilos continuan a partir del punto de la barrera
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Set/09
 */

#include "Barrier.h"


/*
 *  pthread_barrier_init
 */
Barrier::Barrier( int limit ) {
   int resultado;
   barrier=new pthread_barrier_t();
   pthread_mutexattr_t * atributos;
   resultado=pthread_barrier_init(barrier,NULL,limit);
   if(resultado==-1){
      perror("Constructor error");
      exit(0);
   }
}



/*
 * pthread_barrier_destroy
 */
Barrier::~Barrier() {
   int resultado;
   resultado=pthread_barrier_destroy(barrier);
   if(resultado==-1){
      perror("Constructor error");
      exit(0);
   }
}


/*
 * pthread_barrier_wait
 */
void Barrier::Wait() {
    int resultado;
    resultado=pthread_barrier_wait(barrier);
    if(resultado==-1){
        perror("Constructor error");
        exit(0);
    }
}

