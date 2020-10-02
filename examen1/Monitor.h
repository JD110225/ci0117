#include "Lock.h"
#include "Condition.h"
class Monitor{
    public:
    Monitor();
    Monitor(int);
    ~Monitor();
    void ComprarAlmuerzo(int);
    void Almorzar(int);
    void Salir(int);
    int cantidadEstudiantes;
    private:
    Lock* lock;
    Condition* self;
};