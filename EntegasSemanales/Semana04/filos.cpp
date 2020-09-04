#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include "Semaforo.h"
#include <iostream>
Semaforo* palito[5];
int filosofo(int i){
    int chop1 =  (i < 4)? i: 0;                
    int chop2 = (i < 4)? i+1: 4; 
    for(int i=0;i<5;++i){
        printf("Filosofo %d esta pensando\n",i);
        palito[chop1]->wait();
        palito[chop2]->wait();
        printf("Filosofo %d esta comiendo\n",i);
        palito[chop1]->signal();
        palito[chop2]->signal();
    }
    printf("\t\tEl filosofo %d comio %d veces\n",i,5);
    exit(0);
    return 0;
}
int main(int argc,char **argv){
    for(int i=0;i<5;++i){
        palito[i]=new Semaforo(1);
    }
    int pid;
    for(int i=0;i<5;++i){
        pid=fork();
        if(!pid){
            filosofo(i);
        }
    }
    for(int i=0;i<5;++i){
        int status;
        wait(&status);
    }
    return 0;
}