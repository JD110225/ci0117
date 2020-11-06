/**
 *  Programa base para la construcción de centros en una muestra de datos
 *  La muestra de datos está representada por la variable "puntos", todos los puntos generados al azar
 *  Los centros están representados por la variable "centros", todos los puntos colocados en el origen
 *
 *  CI0117 Programación paralela y concurrente
 *  Tercera tarea programada, grupos 2 y 3
 *  2020-ii
 *
**/

#include <cstdio>
#include "VectorPuntos.h"
#include "string"
#include <algorithm>
#include <omp.h>
#include <iostream>
using namespace std;
#define PUNTOS 100000
#define CLASES 17
#define dim 2
#define defaultFileName "resultado.eps"   //Nombre default donde guardar el archivo .eps
long casillas = CLASES;   //valor default
long muestras = PUNTOS;    //valor default
char* fileName=defaultFileName;
//Si se pasan parametros en el main se cambia el valor de casillas y muestras

int totalCambios = 0;	// Contabiliza la totalidad de los cambios realizados al grupo de puntos




/**
 *  Coloca a cada punto en una clase de manera aleatoria
 *  Utiliza el vector de clases para realizar la asignación
 *  En el readme se explica detalladamente el segundo modo de asignacion(la estrategia propia)
**/
void asignarPuntosAClases( long * clases, int modo) {
   long clase, pto;
   switch ( modo ) {
      case 0:	// Asignacion aleatoria de puntos a clases
         #pragma omp parallel for num_threads(4)
         for ( pto = 0; pto < muestras; pto++ ) {
            clase = rand() % casillas;
            clases[ pto ] = clase;
         }
         break;
      case 1:{	// Explicacion de la estrategia en el Readme
         int grupo=0;
         int indice=0;
         int groupCounter=1;
         int elementosPorGrupo=(muestras/casillas)+1;
         while(indice<muestras){
            clases[indice]=grupo;
            if(groupCounter==elementosPorGrupo){
               ++grupo;
               groupCounter=0;
            }
            ++groupCounter;
            ++indice;
         }
      }
   }

}
//Cuenta cuantos elementos hay en un cluster en particular
int contarElementos(long* clases,int cluster){
   int counter=0;
   for(int i=0;i< muestras;++i){
      if(clases[i]==cluster){
         ++counter;
      }
   }
   return counter;
}
//Metodo para visualizar el vector de clases(para debuggear)
void verClases(long clases[CLASES]){
   for(int i=0;i<muestras;++i){
      printf("%d\t",clases[i]);
   }
   printf("\n");
}
//Metodo auxiliar para copiar los contenidos de un arreglo a otro
void copyContents(long* clases,long* otro,int size){
   for(int i=0;i<size;++i){
      otro[i]=clases[i];
   }
}
/**
 *  Programa muestra
 *  Variable: clases, almacena la clase a la que pertenece cada punto, por lo que debe ser del mismo tamaño que las muestras
 *  Variable: contClases, almacena los valores para la cantidad de puntos que pertenecen a un conjunto
**/
int main( int cantidad, char ** parametros ) {
   srand(time(NULL));  
   long cambios, clase, minimo, pto;
   Punto * punto;
//Procesar los parámetros del programa
//Primero se pasa la cantidad de muestras,luego la cantidad de conjuntos, estos dos son necesarios
   if(cantidad>1){
      muestras=atoi(parametros[1]);
      casillas=atoi(parametros[2]);
   }
   //Parametro opcional
   if(cantidad==4){
      fileName=parametros[3];
   }
   VectorPuntos  centros = VectorPuntos( casillas );
   VectorPuntos puntos = VectorPuntos( muestras, 10 );   	// Genera un conjunto de puntos limitados a un círculo de radio 10
   long clases[ muestras ];		// Almacena la clase a la que pertenece cada punto
   long contClases[ casillas ];
   puntos.genEpsFormat( &centros, clases, (char *) "Pre.eps" );
   long bestClases[muestras];
   double menorDisimilaridad=__DBL_MAX__;
   //Iniciar a contar el tiempo
   double disimilaridad;
   double inicio=omp_get_wtime();
   // #pragma omp parallel for num_threads(3) 
   for(int i=0;i<3;++i){
      asignarPuntosAClases( clases,0);	// Asigna los puntos a las clases establecidas
      do {
      // Coloca todos los centros en el origen
      // Promedia los elementos del conjunto para determinar el nuevo centro
      for(int i=0;i<casillas;++i){
         Punto* p=(puntos.findMean(i,clases,contarElementos(clases,i),dim));
         centros.change(i,p);
      }
         cambios = 0;	// Almacena la cantidad de puntos que cambiaron de conjunto
      // Cambia la clase de cada punto al centro más cercano
      #pragma omp parallel for num_threads(4) \
      reduction(+:cambios)
      for(int i=0;i<muestras;++i){
         int index=centros.masCercano(puntos[i]);
         if(clases[i]!=index){
            clases[i]=index;
            ++cambios;
         }
      }
         totalCambios += cambios;
      } while ( cambios > 0 );	// Si no hay cambios el algoritmo converge
      disimilaridad=centros.disimilaridad( &puntos, clases );
      printf( "Valor de la disimilaridad en la solución encontrada %g, con un total de %ld cambios\n", disimilaridad, totalCambios );
      if(disimilaridad<menorDisimilaridad){
         menorDisimilaridad=disimilaridad;
         copyContents(clases,bestClases,muestras);
      }
   }
   double fin=omp_get_wtime();
   printf( "Tiempo empleado: %gms\n", (fin - inicio)/1000 );
   printf("\n\t\t\t\tMEJOR RESULTADO\n\n");
   printf( "Valor de la menor disimilaridad encontrada %g, con un total de %ld cambios\n", menorDisimilaridad, totalCambios );

// // Con los valores encontrados genera el archivo para visualizar los resultados
   puntos.genEpsFormat( &centros, bestClases, (char *) fileName );
}

