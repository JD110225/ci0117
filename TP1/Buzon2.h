/*
   C++ class to encapsulate Unix message passing intrinsic structures and
   system calls

*/

class Buzon2 {
   public:
      Buzon2();
      ~Buzon2();
      int Enviar(long tipo,int valorPapa);
      // int Enviar(const char *label, long n, long tipo);
      // int Enviar(double* arr,long mtype);
      int Recibir( struct msgbuff1 *data, long tipo);
      // int Recibir( struct msgbuff2 *data, long tipo);     
   private:
      int id;		// Identificador del buzon
      int creador; //Creador del recurso
};
