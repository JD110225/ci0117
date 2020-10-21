#include "countingSem.h"
#include <iostream>
#include <unistd.h>
using namespace std;
sem::sem(){

}
sem::sem(int counter){      //-3,0,-10  default lock empieza en 1
    this->counter=counter;
    omp_init_lock(&this->semaphore);
    if(counter<=0){
        this->counter=0;
        omp_set_lock(&this->semaphore);
    }
}
sem::~sem(){
    omp_destroy_lock(&this->semaphore);
}
void sem::viewCounter(){
    cout<<"Contador del semaforo: "<<this->counter<<endl;
}
void sem::Wait(){
    //Wait reduce el valor del contador
    #pragma omp critical 
    {
    if(counter!=0){
        --counter;
    }
    }
    omp_set_lock(&this->semaphore);

}
void sem::Signal(){
    //Signal incrementa el valor del counter
    #pragma omp critical 
    {
    ++counter;
    if(counter==1){
        omp_unset_lock(&this->semaphore);
    }
}
}
