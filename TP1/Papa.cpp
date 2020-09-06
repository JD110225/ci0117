/*
 *  Ejemplo base para el problema de la ronda
 *
 *  CI-0117 Programacion concurrente y paralela
 *  Fecha: 2020/Ago/24
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include "Buzon2.h"
#include "Semaforo.h"
#include <iostream>
using namespace std;
#define MaxParticipantes 5
#define valorDefault 10
// Variable global para el numero de participantes
int participantes = MaxParticipantes;
int valorPapa=valorDefault;
int* perdedores=new int[participantes];

/*
 *  Estructura para el paso de mensajes entre procesos
 */
struct msgbuff1 {
   long mtype;
   int valorPapa;
   // otros elementos
};
Buzon2 b;
struct msgbuff1 receive;
int st;

/*
 *  Aplica las reglas de Collatz al valor de la papa
 *
 */
int cambiarPapa( int papa ) {

   if ( 1 == (papa & 0x1) ) {		// papa es impar
            papa = (papa << 1) + papa + 1;	// papa = papa * 2 + papa + 1
         } else {
            papa >>= 1;			// n = n / 2, utiliza corrimiento a la derecha, una posicion
         }

   return papa;

}
bool endOfGame(){
   bool fin=false;
   int counter=0;
   for(int i=0;i<participantes;++i){
      if(perdedores[i]==1)
         ++counter;
   }
   if(counter==(participantes-1)){
      fin=true;
   }
   return fin;   
}

void showArray(int* arr,int size){
   for(int i=0;i<size;++i){
      cout<<arr[i]<<" ";
   }
   cout<<"\n";
}
int participante( int id ,Semaforo s) {
    int nuevaPapa=cambiarPapa(receive.valorPapa);
    if(nuevaPapa==1){
        perdedores[id]=1;
        //showArray(perdedores,10);
        printf("\n\t\tProceso %d EXPLOTO la papa\n\n",id);
        srand (time(NULL));
        nuevaPapa=rand()%100 +2; //+2 para que la papa no comience siendo uno.
        printf("Nuevo valor random de la papa: %d\n",nuevaPapa);
        b.Enviar(3,nuevaPapa);
    }
    else{
        b.Enviar(3,nuevaPapa);
    }
//    _exit( 0 );	// Everything OK
    s.signal();
    exit(0);
    return 0;

}
int main( int argc, char ** argv ) {
    Semaforo s(0);
    bool end=false;
    // struct msgbuff1 r;
   //  int buzon, id, i, j, resultado;
    int  i, j, resultado;

    if ( argc > 1 ) {
        participantes = atoi( argv[ 1 ] );
        valorPapa=atoi(argv[2]);
   }
   if ( participantes <= 0 ) {
      participantes = MaxParticipantes;
   }
    b.Enviar(3,valorPapa);
   srandom( getpid() );
    printf("Valor papa main: %d\n",valorPapa);
   printf( "Creando una ronda de %d participantes\n", participantes );
   for(int i=0;i<participantes;++i){
      perdedores[i]=0;
   }
   int id= shmget( 123456, 1024, 0700 | IPC_CREAT );
   perdedores=(int*)shmat( id, NULL, 0 );	
   while(!end){
      //ese i=1...ARE YOU GOOD FRANCISCO????
      for ( i = 1; i <= participantes; i++ ) {
         if(perdedores[i]!=1){
            if ( ! fork() ) {
               b.Recibir(&receive,3);
               printf("Proceso %d recibio papa: %d\n",i,receive.valorPapa);
               participante( i ,s);
            }
            else{
               s.wait();
            }
         }
      }
      showArray(perdedores,participantes);
      end=endOfGame();
   }

   shmdt( perdedores );		// Se "despega" del area compartida
   shmctl( id, IPC_RMID, NULL );

// El programa principal decidirá cual es el primer participante en arrancar y el valor inicial de la papa


// Espera que los participantes finalicen
   for ( i = 1; i <= participantes; i++ ) {
      j = wait( &resultado );
   }
}

