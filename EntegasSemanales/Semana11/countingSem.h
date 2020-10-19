#include "omp.h"
class sem{
    public:
        sem();
        ~sem();
        void viewCounter();
        void Wait();
        void Signal();
    private:
        int counter;
        omp_lock_t semaphore;
};