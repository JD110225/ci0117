#include <sys/types.h>
#include "sys/ipc.h"  //esto solo esta en Linux creo FFF
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "Semaforo.h"
using namespace std;
Semaforo::Semaforo(int valorInicial){
    int resultado;
    union semun {
        int              val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                    (Linux-specific) */
    }valor;

    this->id=semget(IPC_PRIVATE,1,IPC_CREAT|0600);
    if(this->id==-1){
        perror("Semaforo:Semaforo resource creation");
        exit(0);
    }
    valor.val=valorInicial;
    resultado=semctl(id,0,SETVAL,valor);
    if(resultado==-1){
        perror("Semaforo:Semaforo resource creation");
        exit(0);
    }
}
/*
Wait: va a esperar siempre que el valor sea <=0, si es >0 no espera.
Siempre wait decrementa el valor del semaforo
Singal incrementa el valor del semaforo y deja pasar a alguno de la cola FIFO.
Para sacar gente de la cola el sem tiene que ser menor que 0, si es mayor que 0 no hay procesos esperando en la cola.
*/
Semaforo::~Semaforo(){
    int resultado;
    resultado=semctl(id,0,IPC_RMID);
    if(resultado==-1){
        perror("Semaforo::~Semaforo");
        exit(0);
    }            
}
int Semaforo::signal(){
    int resultado;
    struct sembuf signal;
    signal.sem_num=0;
    signal.sem_op=1;
    signal.sem_flg=0;
    resultado=semop(this->id,&signal,1);
    if(resultado==-1){
        perror("Semaforo::Signal");
        exit(0);
}
return resultado;
}
int Semaforo::wait(){
    int resultado;
    struct sembuf wait;
    wait.sem_num=0;
    wait.sem_op=-1;
    wait.sem_flg=0;
    resultado=semop(this->id,&wait,1);
    if(resultado==-1){
        perror("Semaforo::Wait");
        exit(0);
    } 
    return resultado;
}