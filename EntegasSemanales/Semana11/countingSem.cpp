#include "countingSem.h"
#include <iostream>
#include <unistd.h>
using namespace std;
sem::sem(){

}
sem::sem(int counter){      //-3,0,-10  default lock empieza en 1
    this->counter=counter;
    bool lockear=false;
    omp_init_lock(&this->semaphore);
    if(counter<=0){
        this->counter=0;
        omp_set_lock(&this->semaphore);
    }
    // else{
    //     if(counter>0){
    //         for(int i=0;i<counter;++i){
    //             cout<<"Hello world"<<endl;
    //             omp_unset_lock(&this->semaphore);
    //         }
    //     }
    //     else{
    //         cout<<"Counter es: "<<endl;
    //         for(int i=0;i<(counter*-1);++i){
    //             cout<<"Hello world"<<endl;
    //             omp_set_lock(&this->semaphore);
    //         }
    //     }
    // }

    //Los locks empiezan en 1 asi que hay que
    // lockearlos una vez para que empiecen en 0
    // omp_set_lock(&this->semaphore);
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
//Un semaforo negativo es lo mismo que 1 en cero.
// Un semaforo positivo es un mutex
// sem s(0);
sem s(0);
// int main(){
//     #pragma omp parallel num_threads(2)
//     if(omp_get_thread_num()&1){
//         sleep(2);
//         printf("Hilo %d realiza Signal\n",omp_get_thread_num());
//         s.Signal();
//     }
//     else{  
//         printf("Hilo %d esta esperando\n",omp_get_thread_num());
//         s.Wait();
//         printf("Hilo %d termino la espera\n",omp_get_thread_num());
//     }
// }