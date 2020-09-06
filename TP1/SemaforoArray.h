class SemaforoArray {
   public:
      SemaforoArray( int, int = 0 );
      ~SemaforoArray();
      int Signal( int = 0 );
      int Wait( int = 0 );
      void SP( int, int );
      void SV( int, int );
      int getVal(int cual);

   private:
      int id;		// Identificador del semaforo
      int creador;
      int nsems;	// Cantidad de semaforos en el arreglo
};
