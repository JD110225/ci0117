/*
 * Resuelve el problema de los filósofos utilizando un monitor "Mesa"
 * 
 * Author: Programacion Concurrente (Francisco Arroyo)
 * Version: 2020/Set/03
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <iostream>
#include <omp.h>
using namespace std;
#include "Mesa.h"
Mesa* mesa;
void Filosofo() {
   int cual=omp_get_thread_num();
   int i;
   int eat, think;
   srand( getpid() );
   for ( i = 0; i < 10; i++ ) {	
      think = rand() & 0xfffff;
      usleep( think );
      mesa->pickup( cual );
      printf( "El filósofo %d está comiendo\n", cual );
      eat = rand() & 0xfffff;
      usleep( eat );
      mesa->putdown( cual);
      printf( "El filósofo %d está pensando\n", cual );
      think = rand()  & 0xfffff;
      usleep( think );
   }
   printf("\t\tEl filosofo %i ya comio 10 veces!!\n",cual);
}

int main( int argc, char ** argv ) {
   mesa=new Mesa();
   #pragma omp parallel num_threads(5)
   {
      Filosofo();
   }
}

