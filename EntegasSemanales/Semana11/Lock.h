#include <omp.h>
class Lock{
    public:
        Lock();
        ~Lock();
        void Acquire();
        void Release();
    private:
        omp_lock_t lock;
};