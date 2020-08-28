#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include "Buzon.h"
#define SIZE 9
struct msgbuff2{
    long mtype;
    double arr[SIZE];
};
Buzon b;
struct msgbuff2 a;
int st;
double calcularSumaParcialPi( int proceso, long inicial, long terminos ) {
   b.Recibir(&a,89);
   if(proceso>0){
   }
   double casiPi = 0;
   double alterna = 4;
   long divisor = 0;
   long termino;

   for ( termino = inicial; termino < terminos; termino++ ) {
      divisor = termino + termino + 1;		// 2 x termino + 1
      casiPi += alterna/divisor;		// 4 / (2xi + 1)
      alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
   }
   a.arr[proceso] = casiPi;			// Guarda el resultado en el vector y finaliza
    b.Enviar(a.arr,89);
    exit( 0 );
}


int main( int argc, char ** argv ) {
    long terminos, inicio, fin;
    int proceso;
    int pid;    
    double* casiPi=new double[9];
    for(int i=0;i<9;++i){
        casiPi[i]=0;
    }
    terminos = 1000000;
        if ( argc > 1 ) {
            terminos = atol( argv[ 1 ] );
        }

    for ( proceso = 0; proceso < 9; proceso++ ) {
        inicio = proceso * terminos/10;
        fin = (proceso + 1) * terminos/10;
        b.Enviar(casiPi,89);

        pid = fork();
        if ( ! pid ) {
            calcularSumaParcialPi( proceso, inicio, fin );
        } else {
            wait(NULL);
            b.Recibir(&a,89);
            casiPi[proceso]=a.arr[proceso];
            }            
        }
    for ( proceso = 1; proceso < 10; proceso++ ) {
        casiPi[ 0 ] += a.arr[ proceso ];
    }

    printf( "Valor calculado de Pi es \033[91m %g \033[0m con %ld terminos\n", casiPi[ 0 ], terminos );
    }