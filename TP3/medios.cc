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

#define PUNTOS 100000
#define CLASES 17


int totalCambios = 0;	// Contabiliza la totalidad de los cambios realizados al grupo de puntos


/**
 *  Coloca a cada punto en una clase de manera aleatoria
 *  Utiliza el vector de clases para realizar la asignación
 *  
**/
void asignarPuntosAClases( long * clases, int modo ) {
   long clase, pto;

   switch ( modo ) {
      case 0:	// Aleatorio
         for ( pto = 0; pto < PUNTOS; pto++ ) {
            clase = rand() % CLASES;
            clases[ pto ] = clase;
         }
         break;
      case 1:	// A construir por los estudiantes
         break;
   }

}
int contarElementos(long* clases,int cluster){
   int counter=0;
   for(int i=0;i< PUNTOS;++i){
      if(clases[i]==cluster){
         ++counter;
      }
   }
   return counter;
}
void verClases(long clases[CLASES]){
   for(int i=0;i<PUNTOS;++i){
      printf("%d\t",clases[i]);
   }
   printf("\n");
}
/**
 *  Programa muestra
 *  Variable: clases, almacena la clase a la que pertenece cada punto, por lo que debe ser del mismo tamaño que las muestras
 *  Variable: contClases, almacena los valores para la cantidad de puntos que pertenecen a un conjunto
**/
int main( int cantidad, char ** parametros ) {
   long cambios, clase, minimo, pto;
   Punto * punto;
   long casillas = CLASES;
   long muestras = PUNTOS;
// Procesar los parámetros del programa
   VectorPuntos  centros = VectorPuntos( casillas );
   // Punto* centros=new Punto[CLASES];
   // VectorPuntos * puntos  = new VectorPuntos( muestras, 10 );	// Genera un conjunto de puntos limitados a un círculo de radio 10
   VectorPuntos  puntos  = VectorPuntos( muestras, 10 );
   long clases[ muestras ];		// Almacena la clase a la que pertenece cada punto
   long contClases[ casillas ];
   asignarPuntosAClases( clases, 0 );	// Asigna los puntos a las clases establecidas
   // for(int i=0;i<CLASES;++i){
   //    Punto* p=(puntos.findMean(i,clases,contarElementos(clases,i)));
   //    centros.change(i,p);
   // }
   puntos.genEpsFormat( &centros, clases, (char *) "Pre.eps" );

   // for(int i=0;i<muestras;++i){
   //    printf("Punto: ");
   //    puntos[i]->ver();
   //    printf("Cluster: %ld\n",clases[i]);
   // }
   printf("\n\t\tCentros: \n");
   centros.display();
   printf("\n");
   // for(int i=0;i<PUNTOS;++i){
   //    int index=centros.masCercano(puntos[i]);
   //    clases[i]=index;
   //    printf("Punto: ");
   //    puntos[i]->ver();
   //    printf("Centro: ");
   //    centros[index]->ver();
   // }
   printf("\n\n");
   verClases(clases);
   // printf("Size: %i\n",centros->demeTamano());
   // puntos.display();
   // for(int i=0;i<casillas;++i){
   //    printf("Size: %i\n",centros[i].demeTamano());
   // }
   do {
   //    Hmm hmm hmm esta logica esta interesting

	// Coloca todos los centros en el origen
	// Promedia los elementos del conjunto para determinar el nuevo centro
   for(int i=0;i<CLASES;++i){
      Punto* p=(puntos.findMean(i,clases,contarElementos(clases,i)));
         centros.change(i,p);
      }
   
      cambios = 0;	// Almacena la cantidad de puntos que cambiaron de conjunto
	// Cambia la clase de cada punto al centro más cercano
   for(int i=0;i<PUNTOS;++i){
      int index=centros.masCercano(puntos[i]);
      if(clases[i]!=index){
         clases[i]=index;
         ++cambios;
      }
   }
      totalCambios += cambios;

   } while ( cambios > 0 );	// Si no hay cambios el algoritmo converge


//    printf( "Valor de la disimilaridad en la solución encontrada %g, con un total de %ld cambios\n", centros->disimilaridad( puntos, clases ), totalCambios );

// // Con los valores encontrados genera el archivo para visualizar los resultados
   puntos.genEpsFormat( &centros, clases, (char *) "Post.eps" );

}

