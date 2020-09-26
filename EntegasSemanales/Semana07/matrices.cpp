//Nota: Se usan matrices de dimension 200 en vez de 100 para poder demostrar mejor las mejoras en tiempo
// Hilos   Tiempo empleado(ms)
// 2       60.118
// 4       57.79
// 16      54.89
// 32      50.22
// 64      62.021 (atipico)
// Serial  76.032     
#include <bits/stdc++.h> 
using namespace std; 
#include <iostream>
#include <sys/time.h>
using namespace std;
#define DIM 200   //Dimension de la matriz  
#define THREADS 2   //Cantidad de hilos a utilizar
int** matrizResultado = new int* [DIM];
int** generarMatrizAleatoria() {
    srand(time(NULL));
    int** matriz = new int* [DIM];
    for (int i = 0; i < DIM; ++i) {
        matriz[i] = new int[DIM];
    }
    for (int f = 0; f < DIM; ++f) {
        for (int c = 0; c < DIM; ++c) {
            matriz[f][c] = (rand() % 10)+1;   //valor random entre 0 y 10.

        }
    }
    return matriz;
}
void initialize(){
    for (int f = 0; f < DIM; ++f) {
        matrizResultado[f] = new int[DIM];
    }
}
int** m1 = generarMatrizAleatoria();
int** m2 = generarMatrizAleatoria();


void startTimer( struct timeval * timerStart) {
   gettimeofday( timerStart, NULL );
}


/*
 *  time elapsed in ms
 */
double getTimer( struct timeval timerStart ) {
   struct timeval timerStop, timerElapsed;
   gettimeofday(&timerStop, NULL);
   timersub(&timerStop, &timerStart, &timerElapsed);
   return timerElapsed.tv_sec*1000.0+timerElapsed.tv_usec/1000.0;
}
void* multiplication(void* arg) 
{ 
     intptr_t threadNum=(intptr_t)arg;
    for (int i = threadNum * DIM / THREADS; i < (threadNum + 1) * DIM / THREADS; i++)  
        for (int j = 0; j < DIM; j++)  
            for (int k = 0; k < DIM; k++)  
                matrizResultado[i][j] += m1[i][k] * m2[k][j]; 
    pthread_exit(0);
} 
void displayMatrix(int** m){
    for (int f = 0; f < DIM; ++f) {
        for (int c = 0; c < DIM; ++c) {
            cout << m[f][c]<<" ";
        }
        cout << "\n";
}
}
int main() 
{ 
    initialize();
    m1=generarMatrizAleatoria();
    m2=generarMatrizAleatoria();
    pthread_t threads[THREADS]; 
    struct timeval timerStart;
    double used;
    startTimer( & timerStart );
    for (int i = 0; i < THREADS; i++) { 
        pthread_create(&threads[i], NULL, multiplication, (void*)(i)); 
    } 
    for (int i = 0; i < THREADS; i++)  
        pthread_join(threads[i], NULL);   
    used = getTimer( timerStart );
    cout<<"Time used: "<<used<<endl;
    return 0; 
} 
