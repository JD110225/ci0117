/*
 * Suma uno a un total mil veces por cada proceso generado
 * Recibe como parametro la cantidad de procesos que se quiere arrancar
 * Author: Programacion Concurrente (Francisco Arroyo / Eduardo Ayales)
 * Version: 2020/Ago/23
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

#ifdef DEBUG
int*    iterations;
#endif

long sumUno(int threads) {
    int i;
    long totalParcial = 0;
   #pragma omp parallel for num_threads(threads) \
      // reduction(+: totalParcial)
    for (i = 0; i < 1000; i++) {
        #pragma omp critical
        totalParcial++;
         #ifdef DEBUG
         iterations[i] = omp_get_thread_num();
         #endif
    }

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
   int threads = 10;
   long resultadoGlobal = 0;

   if(argc > 1){
      threads = atoi(argv[1]);
   }
   int n=1000;
   #ifdef DEBUG
   iterations = malloc((n)*sizeof(int));
   #endif

   double start = omp_get_wtime();
   resultadoGlobal = sumUno(threads);
   double time = omp_get_wtime() - start;

   #ifdef DEBUG
   Print_iters(iterations, 1000);
   free(iterations);
   #endif
   printf( "Valor acumulado por %d threads es \033[91m %ld \033[0m en %f ms\n", threads, resultadoGlobal, time );

}

