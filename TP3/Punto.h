/**
 *   Representa un punto en dos dimensiones
 *
**/


#include <string>

class Punto {
   public:
      Punto();
      Punto( double, double, double = 0 );
      double demeX();
      double demeY();
      double demeZ();
      void ponga( double, double, double = 0 );
      double dist2( Punto * );
      double dist3(Punto *);
      void sume( Punto * );
      void divida( double );
      void ver();
   private:
      double x, y, z;

};

