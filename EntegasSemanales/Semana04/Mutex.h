

/**
 * Esta clase encapsula las funciones para la utilizacion de Mutexs
 * Se provee los metodos para realizar la sincronizacion de procesos
 * a traves de los llamados tradicionales a Lock y Unlock
*/

#ifndef _MUTEX_H
#define _MUTE_HX
#include "SemaforoArray.h"

class Mutex {
   public:
      Mutex();
      ~Mutex();
      void Lock();
      void Unlock();

   private:
      SemaforoArray * mutex;

};

#endif


