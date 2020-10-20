/*  Implantacion de la clase Mutex utilizando PThreads
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Abr/23
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include "Mutex.h"


/**
 *
 *  pthread_mutex_init()
 *
**/
Mutex::Mutex() {
   // mutex=(pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
   mutex=new pthread_mutex_t();
   int resultado;
   pthread_mutexattr_t * atributos;
   resultado=pthread_mutex_init(mutex,NULL);
   if(resultado==-1){
      perror("Destructor error");
      exit(0);
   }
}


/**
 *
 * pthread_mutex_destroy()   pthread_mutexattr_t * atributos;
 *
**/
Mutex::~Mutex() {
   int resultado;
   resultado=pthread_mutex_destroy(mutex); 
   if(resultado==-1){
      perror("Destructor error");
      exit(0);
   }
}


/**
 *
 * pthread_mutex_lock()
 *
**/
int Mutex::Lock() {
   int resultado = 0;
   resultado=pthread_mutex_lock(mutex);
   if(resultado==-1){
      perror("Destructor error");
      exit(0);
   }
   return resultado;

}


/**
 *  pthread_mutex_trylock()
**/
int Mutex::TryLock() {
   int resultado = 0;
   resultado=pthread_mutex_trylock(mutex);
   if(resultado==-1){
      perror("Destructor error");
      exit(0);
   }
   return resultado;

}

/**
 *  pthread_mutex_unlock()
**/
int Mutex::Unlock() {
   int resultado = 0;
   resultado=pthread_mutex_unlock(mutex);
   if(resultado==-1){
      perror("Destructor error");
      exit(0);
   }
   return resultado;

}


/**
 * 
**/
pthread_mutex_t* Mutex::getMutex() {

   return this->mutex;
}

