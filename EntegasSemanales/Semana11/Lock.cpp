#include "Lock.h"
#include <iostream>
using namespace std;
Lock::Lock(){
    omp_init_lock(&this->lock);
}
Lock::~Lock(){
    omp_destroy_lock(&this->lock);
}
void Lock::Acquire(){
    omp_set_lock(&this->lock);
}
void Lock::Release(){
    omp_unset_lock(&this->lock);
}