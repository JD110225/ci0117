
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "Semaforo.h"
void primero() {
   printf( "Voy primero\n" );
}
void segundo() {
   printf( "Voy segundo\n" );
}
void pruebaSerial() {

   primero();
   segundo();

}
int main() {
    Semaforo s( 0 );	// El semáforo debe estar inicializado en cero

    pruebaSerial();

    if ( ! fork() ) {
       primero();
       s.signal();
       exit(0);
    }
    else {
        s.wait();
        segundo();
    }

}
/*
   Salida esperada:
     Voy primero
     Voy segundo
     Voy primero
     Voy segundo
*/
