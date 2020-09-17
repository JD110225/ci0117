/**
 *  PThreads
 *  Esta clase encapsula las funciones para la utilizacion de semaforos
 *  Se provee las estructuras para realizar la sincronizacion de trabajadores
 *  a traves de los metodos tradicionales Signal (V), Wait (P)
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Set/09
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "Semaforo.h"

/*
 *  sem_init
 */
Semaforo::Semaforo( int inicial ) {
   int resultado;
   // semId=(sem_t*)malloc(sizeof(sem_t));
   semId=new sem_t();  //el new si funciona...
   resultado=sem_init(semId,0,1);
   if(resultado==-1){
      perror("Error en semaforo");
      exit(0);
   }
}


/*
 *  sem_destroy
 */
Semaforo::~Semaforo() {
   int resultado;
   resultado=sem_destroy(semId);
   if(resultado==-1){
      perror("Destructor error");
      exit(0);
   }
}


/*
 *  sem_post
 */
int Semaforo::Signal() {
   int resultado = 0;
   resultado=sem_post(semId);
   if(resultado==-1){
      perror("Signal Destructorerror");
      exit(0);
   }
   return resultado;
}


/*
 *  sem_wait
 */
int Semaforo::Wait() {
   int resultado = 0;
   resultado=sem_wait(semId);
   if(resultado==-1){
      perror("Wait error");
      exit(0);
   }
   return resultado;
}


/*
 *  sem_trywait
 */
int Semaforo::tryWait() {
   int resultado = 0;
   resultado=sem_trywait(semId);
   return resultado;

}


/*
 *  sem_timedwait
 */
int Semaforo::timedWait( long sec, long nsec ) {
   int resultado = 0;
   struct timespec lapso;
   lapso.tv_nsec = nsec;
   lapso.tv_sec = sec;
   resultado=sem_timedwait(semId,&lapso);
   return resultado;

}

