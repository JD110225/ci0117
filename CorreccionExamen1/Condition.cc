
#include "Condition.h"

/*
 *  Creates a new condition variable
 *
 *  Uses an internal structure to make workers wait for resources
 *
**/
Condition::Condition(){

   this->workers = 0;
    internalWaitMechanism=new Semaforo(1,0);
   // this->internalWaitMechanism = new Lock();	// Could be any other mechanism
   //this->internalWaitMechanism->Acquire(); 
}


/**
 * Destroys our condition variable
**/
Condition::~Condition() {

   // delete this->internalWaitMechanism;

}


/**
 *  Wait for the condition controlled by our variable
 *
**/
void Condition::Wait( Lock * affectedLock ) {

   this->workers++;
   affectedLock->Release();
   internalWaitMechanism->Wait();
   affectedLock->Acquire();
   // To be completed by students

} 

  
/**
 *  Notify one worker from the queue, if empty has no effect
 *
**/
void Condition::NotifyOne() {
   internalWaitMechanism->Signal();
   // if ( this->workers > 0 ) {
   //    this->workers--;	// One
   //    internalWaitMechanism->Signal();
   // }

}


/**
 *  Same method as notify one, declared for compatibility with many examples
 *
**/
void Condition::Signal() {

   this->NotifyOne();

}

int Condition::getWorkers(){
   return this->workers;
}
/**
 *  Signal all workers from the queue, if empty has no effect
 *
**/
void Condition::NotifyAll() {

   while ( this->workers > 0 ) {
      this->workers--;
      NotifyOne();
   // To be completed by students
   }

}