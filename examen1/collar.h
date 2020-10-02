/**
 * Represent a pearl collar
 * With Black, White, and Transparent pearls
 * Transparent pearl can be counted as White or Black pearls in sequences
 * @author	Programacion Concurrente
 * @version	2020/Oct/2
 * Primer examen parcial
 *
**/


class Collar {
   public:
      Collar( int = 100 );
      Collar( const char * );
      ~Collar();
      char * toString();

   private:
      char * collar;
};

