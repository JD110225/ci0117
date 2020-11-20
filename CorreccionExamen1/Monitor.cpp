#include <stdio.h>
#include <unistd.h>
#include "Monitor.h"

/*
 *
**/
Monitor::Monitor(int cantidadEstudiantes) {
   this->cantidadTotal=cantidadEstudiantes;
   this->estudiantesSentados=cantidadEstudiantes;
   this->estudianteEsperando=-1;
   this->state=new estados[cantidadTotal];
   this->self=new Condition*[cantidadTotal];
   this->lock = new Lock(); 
   for ( int i = 0; i < cantidadTotal; i++ ) {
      this->self[ i ] = new Condition();
   }
}

Monitor::~Monitor() {
   delete this->lock;
   for ( int i = 0; i <this->cantidadTotal; i++ ) {
      delete this->self[ i ];
   }

}

void Monitor::Almorzar( int cual ) {
   this->lock->Acquire();
   int espera;
   this->state[cual]=EATING;
   printf("Estudiante %d almorzando\n",cual);
   this->lock->Release();
}
void Monitor::ComprarAlmuerzo( int cual ) {
   this->lock->Acquire();
   int espera;
   printf("Estudiante %d comprando almuerzo\n",cual);
   this->lock->Release();
}
// Se llama a test cuando quedan 2 estudiantes , para que el 
//penultimo se ponga en wait y el ultimo le haga signal y se puedan
//ir los dos a la vez sin que nadie se quede solo almorzando
void Monitor::test(int cual){
   if(estudianteEsperando==-1){
      estudianteEsperando=cual;  
      this->state[cual]=WAITING;
      printf("\n\t\tEstudiante %d:No me puedo ir, me toca esperar \n",estudianteEsperando);
      this->self[estudianteEsperando]->Wait(this->lock);
   }
   else{
      printf("\t\tEstudiante %d: Ya termine de comer, nos podemos ir\n\n",cual);
      this->self[estudianteEsperando]->Signal();
   }
}
void Monitor::Salir(int cual){
   this->lock->Acquire();
   if(estudiantesSentados==2){
      test(cual);
   }
   this->estudiantesSentados--; 
   printf("El estudiante %d se ha ido Quedan %d estudiantes\n",cual,this->estudiantesSentados);
   this->state[cual]=FINISHED;
   this->lock->Release();
}

