#include "omp.h"
class sem{
    public:
        sem(int counter);
        sem();
        ~sem();
        void viewCounter();
        void Wait();
        void Signal();
        void semaphoreUpdate(int operation);
    private:
        int counter;
        omp_lock_t semaphore;
};