/*  PThreads
 *
 *  Esta clase encapsula las funciones para la utilizacion de semaforos
 *  Se provee las estructuras para realizar la sincronizacion de trabajadores
 *  a traves de los metodos tradicionales Signal y Wait 
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Set/09
 */

#ifndef _COND_H
#define _COND_H
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "Lock.h"

class Condition {

   public:
      Condition();
      ~Condition();
      int Wait( Mutex * );
      int TimedWait( Mutex *,long,long );
      int Signal();
      int Broadcast();

   private:
      pthread_cond_t * vc;

};

#endif

