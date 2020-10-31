#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <unistd.h>
//Nota: La tabla con los tiempos esta en un archivo aparte en el repositorio
using namespace std;
bool isPrime( long num,int workers ) {
    bool flag=true;
    //Revisar mas alla de la raiz de num es redundante y aumenta considerablemente el tiempo de ejecucion
    //pero el algoritmo se hizo ineficiente adrede para que hayan mas iteraciones y asi contrastar mejor OMP vs Serial
    int i;
    #pragma omp parallel for num_threads(workers)
    for(i=2;i<num;++i){
        if(num%i==0){
            flag=false;
        }
        // cout<<"Hello from thread: "<<omp_get_thread_num()<<endl;
    }
    //Manejar el caso en que el parametro sea 1.
    if(num==1){
        flag=false;
    }
    return flag;    
}

/**
 *  Find the numbers
**/
void encontrarRepresentacion(int numero,int threads){
    bool encontrada=false;
    for(int i=2;i<numero && !encontrada;++i){
        if(isPrime(i,threads) && isPrime(numero-i,threads)){
            #pragma omp critical
            {
                //DESCOMENTAR PARA VER SUMA DE PRIMOS
                // cout<<numero<<" = "<<i<<" + "<<numero-i<<endl;
            }
            encontrada=true;
        }
    }
}
int doWork( int limite,int threads ) {
    int encontrados=0;
    #pragma omp parallel for num_threads(threads)
    for(int i=6;i<=limite;i+=2){
        encontrarRepresentacion(i,threads);
        ++encontrados;
    }
    return encontrados;
}
int main( int cantidad, char ** parametros ) {
   long valor, worker, workers, limite, encontrados;
   double inicio, fin;
   limite = 50;
   if ( cantidad > 1 ) {
      limite = atol( parametros[ 1 ] );
   }

   workers = 10;
   if ( cantidad > 2 ) {
      workers = atol( parametros[ 2 ] );
   }
    inicio = omp_get_wtime();
    encontrados=doWork(limite,workers);
    fin = omp_get_wtime();
    printf( "Sumas de pares encontradas %d, tiempo %gms\n", (int) encontrados, (fin - inicio)/1000 );
}