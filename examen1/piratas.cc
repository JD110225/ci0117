/**
 * Represent a pearl collar
 * With Black, White, and Transparent pearls
 * Transparent pearl can be counted as White or Black pearls in sequences
 * @author	Programacion Concurrente
 * @version	2020/Oct/2
 * Primer examen parcial
 *
**/

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "collar.h"
using namespace std;
Collar *collar;  //VARIABLE GLOBAL PARA LOS PTHREADS.
int tamanoCollar; //VARIABLE GLOBAL PARA LOS PTHREADS.
int hilos; //CANTIDAD DE THREADS A UTILIZAR PARA LA VERSION CONCURRENTE.
int mejorGlobal; //VARIABLE GLOBAL PARA LOS PTHREADS.
int mejorCorte;
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

//Conteo de perlas a la izquierda
int countIzquierda(int posicion,char* collar,int size){
   char c=collar[posicion-1];
   int counter=0;
   bool direccion=0;
   bool valido=1;
   for(int i=posicion-1;i>=0 && valido;--i){
      if(collar[i]==c || collar[i]=='T'){
         ++counter;
      }
      else{
         valido=false;
      }
   }
   if(valido){
      for(int i=size-1;i>=0 && valido;--i){
         if(collar[i]==c || collar[i]=='T'){
            ++counter;
         }
         else{
            valido=false;
         }
      }
   }
   usleep(10);
   return counter;
}
//Conteo de perlas a la derecha
int countDerecha(int posicion,char* collar,int size){
   char c=collar[posicion];
   int counter=0;
   bool valido=1;
   for(int i=posicion;i<size && valido;++i){
      if(collar[i]==c || collar[i]=='T'){
         ++counter;
      }
      else{
         valido=false;
      }
   }
   if(valido){
      for(int i=size-1;i>=0 && valido;--i){
         if(collar[i]==c || collar[i]=='T'){
            ++counter;
         }
         else{
            valido=false;
         }
      }
   }
   usleep(10);
   return counter;
}
//Averiguar el conteo desde todos los indices posibles del collar para calcular la mejor posicion para el corte.
int mejorGanancia(char *collar,int size){
   int mejor=0;
   for(int i=0;i<size;++i){
      int ganancia=countIzquierda(i,collar,size)+countDerecha(i,collar,size);
      if(ganancia>mejor){
         mejor=ganancia;
         mejorCorte=i;
      }
   }
   cout<<"\n";
   return mejor;
}
//Solucion con Pthreads, se emplea un mapeo estatico por bloques
//realizando una distribucion equitativa de tareas entre trabajadores.
//Se pasa el numero de hilo en el parametro t y este hilo
// calculara las mejorGanancia desde la posicion start hasta la posicion end
// las cuales dependeran de la cantidad de hilos y el tamano del collar
void* mejorGananciaConcurrente(void * t){
   intptr_t threadNum=(intptr_t)t;
   int start=threadNum * tamanoCollar / hilos;
   int end= (threadNum + 1) * tamanoCollar / hilos;
   int mejorLocal=0;
   for(int i=start;i<end;++i){
      int g=countIzquierda(i,collar->toString(),tamanoCollar)+countDerecha(i,collar->toString(),tamanoCollar);
      if(g>mejorLocal){
         mejorLocal=g;
      }
   }
   if(mejorLocal>mejorGlobal){
      mejorGlobal=mejorLocal;
   }
   // return mejorLocal;
}
int main( int argumentos, char ** valores ) {
   // Collar * collar1 = new Collar( "TTBBNNTT" );
   //COLLAR DE PRUEBA
   collar = new Collar( "BBBNTTNNBNNBT" );
   mejorCorte=0;
   mejorGlobal=0;
   hilos=13;      //Puede variar este numero
   tamanoCollar=13; //NECESARIO CAMBIAR ESTE PARAMETRO PARA PRUEBAS
    struct timeval timerStart;
    double used;
   // Collar * collar3 = new Collar( "TTTTTTTTT" );
   // Collar * collar4 = new Collar( 1024 );

               // EJEMPLO SERIAL PRUEBA DEL ENUNCIADO
   cout<<"EJEMPLO PRUEBA DEL ENUNCIADO"<<endl;
   cout<<"Ganancia de punto de corte 3: ";
   int izq=countIzquierda(3,collar->toString(),13);
   int der=countDerecha(3,collar->toString(),13);
   cout<<izq+der<<endl;
    startTimer( & timerStart );
   int m=mejorGanancia(collar->toString(),13);
   cout<<"Mejor ganancia (SERIAL): "<<m<<endl;
   cout<<"Mejor corte (SERIAL): "<<mejorCorte<<endl;
   used = getTimer( timerStart );
   cout<<"Time spent SERIAL: "<<used<<endl;

               //VERSION CONCURRENTE
   long hilo;
   pthread_t * pthilos;
   pthilos = (pthread_t *) calloc( hilos, sizeof( pthread_t ) );
    startTimer( & timerStart );
    for ( hilo = 0; hilo <hilos; hilo++ ) {
        pthread_create( & pthilos[ hilo ], NULL, mejorGananciaConcurrente, (void *) hilo );
         // usleep(10);
      }   
    for ( hilo = 0; hilo < hilos; hilo++ ) {
        pthread_join( pthilos[ hilo ], NULL );
    }
    used = getTimer( timerStart );
    cout<<"Mejor ganancia(PTHREADS) con "<<hilos<<" hilos "<<mejorGlobal<<endl;
    cout<<"Mejor corte(PTHREADS) con "<<hilos<<" hilos "<<mejorCorte<<endl;
    cout<<"Time spent CONCURRENTE: "<<used<<endl;
}

