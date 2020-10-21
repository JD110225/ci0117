/*  Esta clase encapsula las funciones para la utilización de variables de condi
ción
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Set/09
 *
**/

#include "Condition.h"

Condition::Condition() {
   this->workers=0;
   this->internalWaitMechanism= new Lock();
}
Condition::~Condition() {
   delete this->internalWaitMechanism;
}


/*
 * pthread_cond_wait
 */
void Condition::Wait( Lock * affectedLock ) {
   this->workers++;
   affectedLock->Release();
   internalWaitMechanism->Acquire();
   affectedLock->Acquire();
}


/*
 * pthread_cond_timedwait
 */
// int Condition::TimedWait( Mutex * mutex,long nsec,long sec ) {
//    int resultado;
//    struct timespec lapso;
//    lapso.tv_nsec = nsec;
//    lapso.tv_sec = sec;
//    resultado=pthread_cond_timedwait(vc,(mutex->getMutex()),&lapso);
//    if(resultado==-1){
//       perror("Constructor error");
//       exit(0);
//    }
//    return resultado;
// }


/*
 * pthread_cond_signal
 */
void Condition::Signal() {
   if(this->workers>0){
      this->workers--;
      internalWaitMechanism->Release();
   }
}


/*
 * pthread_cond_broadcast
 */
// int Condition::Broadcast() {
//    int resultado;
//    resultado=pthread_cond_broadcast(vc);
//    if(resultado==-1){
//       perror("Signal error");
//       exit(0);
//    }
//    return resultado;
// }

