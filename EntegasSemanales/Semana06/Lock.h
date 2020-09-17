/**
 * Esta clase encapsula las funciones para la utilizacion de Locks
 * Se provee los metodos para realizar la sincronizacion de procesos
 * a traves de los llamados tradicionales a Acquire y Release
 *
 *  Es construida utilizando un mutex lock
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Set/09
 *
**/

#ifndef _LOCK_H
#define _LOCK_H
#include "Mutex.h"

class Lock {
   public:
      Lock();
      ~Lock();
      void Acquire();
      void Release();
      Mutex* getMutex();

   private:
      Mutex * mutex;

};

#endif

