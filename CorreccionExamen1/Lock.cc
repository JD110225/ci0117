/*  Esta clase encapsula las funciones para la utilizacion de Locks
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Ago/24
 *
 */

#include "Lock.h"

/*
 *  Creates a lock, using a semaphore
 */
Lock::Lock() {

   this->lock = new Semaforo( 1, 1 );

}


/**
 * Destroys the lock
 */
Lock::~Lock() {

   delete this->lock;

}


/**
 * Acquires the lock
 *
 */
void Lock::Acquire() {

   this->lock->Wait();

} 

  
/**
 * Release the lock
 *
 */
void Lock::Release() {

   this->lock->Signal();

}

