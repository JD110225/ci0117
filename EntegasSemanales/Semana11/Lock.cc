/*  Esta clase encapsula las funciones para la utilizacion de Locks
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Set/09
 *
 */

#include "Lock.h"

/*
 *  Creates a lock, using a mutex
 */
Lock::Lock() {
    mutex=new Mutex();
}


/**
 * Destroys the lock
 */
Lock::~Lock() {

    delete mutex;
}


/**
 * Acquires the lock
 *
 */
void Lock::Acquire() {
    mutex->Lock();

} 

  
/**
 * Release the lock
 *
 */
void Lock::Release() {
    mutex->Unlock();
}
Mutex* Lock::getMutex(){
    return this->mutex;
}
