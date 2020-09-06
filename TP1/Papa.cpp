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
#define MaxParticipantes 5 //Valor predeterminado para el numero de participantes
#define valorDefault 10 //Valor predeterminado de la Papa
// Variable global para el numero de participantes
int participantes = MaxParticipantes;
int valorPapa=valorDefault;
/*
   El i-esimo elemento del arreglo perdedores tendra valor 1
   si el proceso "i" exploto la papa y 0 en caso contrario,
   ademas es un segmento de memoria compartida.
*/
bool* perdedores=new bool[participantes];

/*
 *  Estructura para el paso de mensajes entre procesos
 */
struct msgbuff1 {
   long mtype;
   int valorPapa;
};
Buzon2 b;
struct msgbuff1 receive;

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
//Si hay n participantes, este metodo verifica si n-1 de ellos ya explotaron la papa(perdieron)
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
// Cuando ya acabo el juego, se busca aquel proceso que nunca exploto la papa(el ganador).
int getWinner(){
   int winnerId=-1;
   if(endOfGame()){
      for(int i=0;i<participantes;++i){
         if(perdedores[i]==0){
            winnerId=i;
            break;
         }
      }
   }
   return winnerId;
}
/*
   Este metodo realiza todas las acciones respectivas de un participante cuando recibe una papa.
*/
int participante( int id ,Semaforo s,bool procesoPasivo) {
    int nuevaPapa;
    if(!procesoPasivo){
      nuevaPapa=cambiarPapa(receive.valorPapa);
      if(nuevaPapa==1){
         perdedores[id]=1;
         //showArray(perdedores,10);
         printf("\n\t\tProceso %d EXPLOTO la papa\n\n",id);
         srand (getpid());
         nuevaPapa=rand()%10 +2; //+2 para que la papa no comience siendo uno.
         printf("Nuevo valor random de la papa: %d\n",nuevaPapa);
      }
    }
   else{
      nuevaPapa=receive.valorPapa;
   }
   b.Enviar(3,nuevaPapa);
    s.signal();
    exit(0);
}
int main( int argc, char ** argv ) {
    Semaforo s(0); //synchronization.
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
   printf( "Creando una ronda de %d participantes\n", participantes );
   for(int i=0;i<participantes;++i){
      perdedores[i]=0;
   }
   int id= shmget( 123456, 1024, 0700 | IPC_CREAT );
   perdedores=(bool*)shmat( id, NULL, 0 );	
   bool end=false; //Si el juego ya finalizo.
   while(!end){
      for ( i = 0; i < participantes; i++ ) {
         if ( ! fork() ) {
            b.Recibir(&receive,3);
            bool esPasivo=perdedores[i]==1;
            if(esPasivo){
               printf("Proceso %d recibio papa: %d (es un participante pasivo)\n",i,receive.valorPapa);
            }
            else{
               printf("Proceso %d recibio papa: %d\n",i,receive.valorPapa);             
            }
            participante( i ,s,esPasivo);
         }
         else{
            s.wait();
         }
         
      }
      end=endOfGame();
   }
   int winnerId=getWinner();
   printf("El proceso ganador es: %d!!!\n",winnerId);
   shmdt( perdedores );		
   shmctl( id, IPC_RMID, NULL );
   for ( i = 1; i <= participantes; i++ ) {
      j = wait( &resultado );
   }
}

