#include <omp.h>
class Lock{
    public:
        Lock();
        ~Lock();
        void Acquire();
        void Release();
        omp_lock_t getLock();
    private:
        omp_lock_t lock;
};