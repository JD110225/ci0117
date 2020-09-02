/*
   C++ class to encapsulate Unix message passing intrinsic structures and
   system calls

*/

class BuzonAgua {
   public:
      BuzonAgua();
      ~BuzonAgua();
      int Enviar(long tipo,int cO,int cH);
      // int Enviar(const char *label, long n, long tipo);
      // int Enviar(double* arr,long mtype);
      int Recibir( struct Agua *data, long tipo);
      // int Recibir( struct msgbuff2 *data, long tipo);     
   private:
      int id;		// Identificador del buzon
      int creador; //Creador del recurso
};
