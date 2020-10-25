/**
 *   Representa un conjunto de puntos en dos dimensiones
 *
**/


#include "Punto.h"
#include "time.h"

class VectorPuntos {
   public:
      VectorPuntos( long, double );
      VectorPuntos( long );
      ~VectorPuntos();
      long demeTamano();
      long masCercano( Punto * );
      double variabilidad( Punto *, long, long * );
      double disimilaridad( VectorPuntos *, long * );
      void genEpsFormat( VectorPuntos *, long *, char * );
      void change(int index,Punto* p);
      void sort();
      void swap(int,int);
      Punto * operator [] ( long );
      void display();
      Punto* findMean(int cluster,long* vectorClases,int cantidadEnClusters);
   private:
      long elementos;
      Punto ** bloque;

};

