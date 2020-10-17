#include <time.h>
#include <omp.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef DEBUG
int* iterations;
#endif
//      TIEMPO TRANSCURRIDO CON TAMANO 200 Y 10 HILOS:
// 1)STATIC: 0.035 (sin poner schedule(runtime))
// 2)DYNAMIC: 0.030 (export OMP_SCHEDULE=DYNAMIC)
// 3)GUIDED:  0.028 (export OMP_SCHEDULE=GUIDED)
//Conclusion: El metodo por defecto de "static" es  menos eficiente que el de 
//"dynamic" y el mejor de todos(a pesar de que los tres esten sumamente parejos)
//es el metodo "guided"
//Nota: el schedule(runtime esta comentado en la linea 23)
int** matrixMultiplication(int** a, int** b,int size,int threads){
    int** matrizResultado = (int**)malloc((size*size*sizeof(int)));
    for (int f = 0; f < size; ++f) {
        matrizResultado[f] = (int*)malloc((size)*sizeof(int));
    }
    int f,c,k=0;
    #pragma omp parallel for num_threads(threads)private(f,c,k) //schedule(runtime)
    for (f = 0; f < size; ++f) {
    for (c = 0; c < size; c++){
        for (k = 0; k < size; k++){
            matrizResultado[f][c] += a[f][k] * b[k][c]; 
        }

    }
    int t=omp_get_thread_num();
    #ifdef DEBUG
    iterations[f] = omp_get_thread_num();
    #endif
}

    return matrizResultado;
}
int** generarMatrizAleatoria(int size) {
    srand((int)time(0));
    int** matriz = (int**)malloc((size*size*sizeof(int)));
    for (int i = 0; i < size; ++i) {
        matriz[i] = (int*)malloc((size*size*sizeof(int)));
    }
    for (int f = 0; f < size; ++f) {
        for (int c = 0; c < size; ++c) {
            matriz[f][c] = (rand() % 10)+1;   //valor random entre 0 y 10.

        }
    }
    return matriz;
}
void displayMatrix(int** m,int size){
    for (int f = 0; f < size; ++f) {
        for (int c = 0; c < size; ++c) {
            printf("%d ",m[f][c]);
        }
        printf("\n");
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
int main(int argc,char** argv)
{
    int size=200;
    int threads=10;
    if ( argc > 1 ) {
      threads = atoi(argv[1]);
      size = atoi(argv[2]);
   }
    int** m1 = generarMatrizAleatoria(size);
    int** m2 = generarMatrizAleatoria(size);
    printf("T: %d, S: %d\n ",threads,size);
    #ifdef DEBUG
    iterations = malloc((size+1)*sizeof(int));
    #endif
    double start=omp_get_wtime();
    int** resultMatrix=matrixMultiplication(m1, m2, size,threads);
    double empleado=omp_get_wtime()-start;
    // printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC); //stack overflow:https://stackoverflow.com/questions/876901/calculating-execution-time-in-c 
    // printf("M1: \n");
    // displayMatrix(m1, size);
    // printf("M2: \n");
    // displayMatrix(m2, size);
    // printf("RESULT: \n");
    // displayMatrix(resultMatrix,size);
   #ifdef DEBUG
   Print_iters(iterations, size);
   free(iterations);
   #endif
   printf("Tiempo empleado: %f\n",empleado);

}
