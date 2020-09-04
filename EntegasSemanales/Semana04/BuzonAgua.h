/*
   C++ class to encapsulate Unix message passing intrinsic structures and
   system calls

*/

class BuzonAgua {
   public:
      BuzonAgua();
      ~BuzonAgua();
      int Enviar(bool* chop,long tipo);
      int Enviar(long tipo,int cO,int cH);
      int Recibir( struct Agua *data, long tipo);
      int Recibir(struct chopSticks *chops,long tipo);
   private:
      int id;		// Identificador del buzon
      int creador; //Creador del recurso
};
