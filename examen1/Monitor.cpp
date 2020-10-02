#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Monitor.h"
using namespace std;
Monitor::Monitor(int cantidadEstudiantes) {
    this->cantidadEstudiantes=cantidadEstudiantes;
    int i;
    this->lock = new Lock(); 
    self=new Condition[cantidadEstudiantes];
   for ( i = 0; i < cantidadEstudiantes; i++ ) {
      this->self[ i ] = Condition();
   }
}
void Monitor::ComprarAlmuerzo( int cual ) {
   this->lock->Acquire();
   int espera;
    cout<<"Estudiante "<<cual<<" comprando almuerzo"<<endl;
   espera = random() & 0x3;
   sleep( espera );
   this->lock->Release();
}


/**
 *
**/
void Monitor::Almorzar( int cual ) {
   this->lock->Acquire();
   int espera;
    cout<<"Estudiante "<<cual<<" almorzando"<<endl;
   espera = random() & 0x3;
   sleep( espera );
   this->lock->Release();

}


/**
 *
**/
void Monitor::Salir( int cual ) {
   this->lock->Acquire();
   int espera;
    cout<<"Estudiante "<<cual<<" salio del comedor"<<endl;
   espera = random() & 0x3;
   sleep( espera );
   this->lock->Release();
}
