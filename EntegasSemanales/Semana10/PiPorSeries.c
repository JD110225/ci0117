/*
 * Calcula el numero PI utilizando una serie "infinita"
 * Debe recibir la cantidad de iteraciones como parametro
 *
 *   pi = (-1)^i x 4/(2xi + 1)
 *
 *  Autor: Programacion Paralela y Concurrente (Francisco Arroyo)
 *  Fecha: 2020/Ago/08
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

#ifdef DEBUG
int*    iterations;
#endif



double calcularPi(long terminos, int threads){
   double pi = 0.0;
   long divisor;
   double alterna = 1;
   long i;
   #pragma omp parallel for num_threads(threads) \
   reduction(+:pi) private(alterna, divisor)//default(none) shared(pi) private(alterna, i, divisor)
   for (i = 0; i < terminos; i++ ) {
      alterna = (i % 2 == 0) ? 1.0 : -1.0;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
      divisor = i + i + 1;		// 2 x termino + 1
      //printf("Pi es %f en el hilo %i \n", pi, omp_get_thread_num());
      pi += 4 * (alterna/divisor);		// 4 / (2xi + 1)
      
      #ifdef DEBUG
      iterations[i] = omp_get_thread_num();
      #endif
   }

   return pi;
}

void Print_iters(int iterations[], long n) {
   int i, start_iter, stop_iter, which_thread;

   printf("\n");
   printf("Thread\t\tIterations\n");
   printf("------\t\t----------\n");
   which_thread = iterations[0];
   start_iter = stop_iter = 0;
   for (i = 0; i <= n; i++)
      if (iterations[i] == which_thread)
         stop_iter = i;
      else {
         printf("%4d  \t\t%d -- %d\n", which_thread, start_iter,
               stop_iter);
         which_thread = iterations[i];
         start_iter = stop_iter = i;
      }
   printf("%4d  \t\t%d -- %d\n", which_thread, start_iter,
               stop_iter);
}

int main( int argc, char ** argv ) {
   long terminos;
   double pi = 0.0;
   int threads = 10;

   terminos = 1000000;
   if ( argc > 1 ) {
      threads = atoi(argv[1]);
      terminos = atol(argv[2]);
   }

   #ifdef DEBUG
   iterations = malloc((terminos+1)*sizeof(int));
   #endif

   double start = omp_get_wtime();
   pi = calcularPi(terminos, threads);
   double time = omp_get_wtime() - start;
   
   #ifdef DEBUG
   Print_iters(iterations, terminos);
   free(iterations);
   #endif

   printf( "Valor calculado de Pi con %d threads es \033[91m %g \033[0m con %ld terminos en %g ms \n", threads, pi, terminos, time );
}
