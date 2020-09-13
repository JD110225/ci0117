/*
 *  C++ class to encapsulate Unix semaphore intrinsic structures and system calls
 *  Author: Programacion Concurrente (Francisco Arroyo)
 *  Version: 2020/Ago/24
 *
 */

class Semaforo {
   public:
      Semaforo( int, int = 0 );
      ~Semaforo();
      int Signal( int = 0 );
      int Wait( int = 0 );
      void SP( int, int );
      void SV( int, int );

   private:
      int id;		// Identificador del semaforo
      int nsems;	// Cantidad de semaforos en el arreglo
};

