#include "Lock.h"
#include "Condition.h"


class Monitor {

   public:
    Monitor(int);
    ~Monitor();
    void verEstados();
    void ComprarAlmuerzo(int cual);
    void Almorzar(int cual);
    void Salir(int cual);
    void test(int);
    int getUltimo();
   private:
    int cantidadTotal;
    int estudiantesSentados;
    int estudianteEsperando;
    Lock * lock;
    enum estados {FINISHED,WAITING,EATING};
    estados* state;     // Para conocer el estado de cada estudiante
    Condition** self;   //debe ser dinamico porque el tamano debe definirse por el usuario.

};