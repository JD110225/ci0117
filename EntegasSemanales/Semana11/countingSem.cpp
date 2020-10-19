#include "countingSem.h"
#include <iostream>
#include <unistd.h>
using namespace std;
sem::sem(){
    this->counter=0;
    omp_init_lock(&this->semaphore);
    //Los locks empiezan en 1 asi que hay que
    // lockearlos una vez para que empiecen en 0
    omp_set_lock(&this->semaphore);
}
sem::~sem(){
    omp_destroy_lock(&this->semaphore);
}
void sem::viewCounter(){
    cout<<"Contador del semaforo: "<<this->counter<<endl;
}
void sem::Wait(){
    //Wait reduce el valor del contador
    --counter;
     omp_set_lock(&this->semaphore);
}
void sem::Signal(){
    //Signal incrementa el valor del counter
    ++counter;
    omp_unset_lock(&this->semaphore);
}
sem s;
int inicial=0;
void prueba(){
    // cout<<"hey"<<endl;
    if(omp_get_thread_num()!=inicial){
        s.Wait();
    }
    inicial=omp_get_thread_num();
    cout<<"Hello from thread: "<<omp_get_thread_num()<<endl;
    s.Signal();
    s.viewCounter();
}
// int main(){
//     #pragma omp parallel num_threads(4)
//     prueba();
// }