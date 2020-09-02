#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <string.h>
#include "Semaforo.h"
#include <iostream>
struct Mesa{
    bool chopsticks[5]={false};
};
Semaforo filo[5];
struct Mesa* mesa;
int Filosofo(int i){
    int j=(i+1)%5;
    //Pensar,generar un random y esperar con usleep
    if(!mesa->chopsticks[i] && !mesa->chopsticks[j]){
        mesa->chopsticks[i]=true;
        mesa->chopsticks[j]=true;
        //Puede comer
    }
    else{
        filo[i].wait();
    }
    return 0;
}
Semaforo* palito[5];
int filosofo(int i){
    // printf("Process %d: primer print\n",i );
    // printf("Process %d: segundo print\n",i);
    int j=(i+1)%5;
    int chop1 =  (i < 4)? i: 0;                
    int chop2 = (i < 4)? i+1: 4; 
    for(int i=0;i<5;++i){
        printf("Filosofo %d esta pensando\n",i);
        // printf("Filosofo: %d\n",i);
        // printf("i: %d\t j: %d\n",palito[i]->getVal(),palito[j]->getVal());
        palito[chop1]->wait();
        palito[chop2]->wait();
        printf("Filosofo %d esta comiendo\n",i);
        // printf("i: %d\t j: %d\n",palito[i]->getVal(),palito[j]->getVal());
        // printf("i: %d\t j: %d\n",palito[i]->getVal(),palito[j]->getVal());
        palito[chop1]->signal();
        palito[chop2]->signal();
        // printf("i: %d\t j: %d\n",palito[i]->getVal(),palito[j]->getVal());
    }
    printf("\t\tEl filosofo %d comio %d veces\n",i,5);
    exit(0);
    // exit(0);
    // for(int i=0;i<100;++i){
    //     //Think
    //     //Mutex.wait() eh?
    //     palito[i].wait();
    //     palito[j].wait();
    //     //Eat Mutex.signal() eh?
    //     palito[i].signal();
    //     palito[j].signal();
    // }
    return 0;
}
int main(int argc,char **argv){
    for(int i=0;i<5;++i){
        palito[i]=new Semaforo(1);
    }
    int id= shmget( 123456, 1024, 0700 | IPC_CREAT );
    mesa=(struct Mesa *) shmat( id, NULL, 0 );	    
    int pid;
    for(int i=0;i<5;++i){
        pid=fork();
        if(!pid){
            filosofo(i);
            // exit(0);
            // exit(0);
            // s.signal();
            // exit(0);
        }
        else{
            // usleep(1);
            // s.wait();
        }
    }
    for(int i=0;i<5;++i){
        int status;
        wait(&status);
    }
    return 0;
}