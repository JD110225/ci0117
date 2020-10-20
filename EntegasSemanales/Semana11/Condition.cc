/*  Esta clase encapsula las funciones para la utilización de variables de condi
ción
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Set/09
 *
**/

#include "Condition.h"

/*
 * pthread_cond_init
 */
Condition::Condition() {
   this->workers=0;
   this->internalWaitMechanism= new Lock();
   // int resultado;
   // vc=new pthread_cond_t();
   // resultado=pthread_cond_init(vc,NULL);
   // if(resultado==-1){
   //    perror("Constructor error");
   //    exit(0);
   // }
}


/*
 * pthread_cond_destroy
 */
Condition::~Condition() {
   delete this->internalWaitMechanism;
   // int resultado;
   // resultado=pthread_cond_destroy(vc);
   // if(resultado==-1){
   //    perror("Destructor error");
   //    exit(0);
   // }
}


/*
 * pthread_cond_wait
 */
int Condition::Wait( Lock * affectedLock ) {
   int resultado;
   // resultado=pthread_cond_wait(vc,(mutex->getMutex()));
   // if(resultado==-1){
   //    perror("Wait error");
   //    exit(0);
   // }
   this->workers++;
   affectedLock->Release();
   internalWaitMechanism->Acquire();
   affectedLock->Acquire();
   return resultado;
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
int Condition::Signal() {
   if(this->workers>0){
      this->workers--;
      internalWaitMechanism->Release();
   }
   // int resultado;
   // resultado=pthread_cond_signal(vc);
   // if(resultado==-1){
   //    perror("Signal error");
   //    exit(0);
   // }
   // return resultado;
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

