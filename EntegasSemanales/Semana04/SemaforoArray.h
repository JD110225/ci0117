class SemaforoArray {
   public:
      SemaforoArray( int, int = 0 );
      ~SemaforoArray();
      int Signal( int = 0 );
      int Wait( int = 0 );
      void SP( int, int );
      void SV( int, int );

   private:
      int id;		// Identificador del semaforo
      int nsems;	// Cantidad de semaforos en el arreglo
};
