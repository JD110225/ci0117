#include "Lock.h"

class Condition {

   public:
      Condition();
      ~Condition();
      void Wait( Lock * );
      void NotifyOne();
      void NotifyAll();
      void Signal();
      int getWorkers();

   private:
     int workers;
     Semaforo* internalWaitMechanism;
   //   Lock * internalWaitMechanism;

};

