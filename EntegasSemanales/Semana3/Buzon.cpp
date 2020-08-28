

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "Buzon.h"
#define KEY 0xB12345 
#define SIZE 9
struct msgbuff1
{
    long mtype; // message type, must be > 0
    long times;  // Times that label appears
    char label[64];
    //char * label;
};
struct msgbuff2{
    long mtype;
    double arr[SIZE];
};
int Buzon::Enviar(double* arr,long mtype){
    int resultado;
    struct msgbuff2 m;
    m.mtype = mtype; 
    for(int i=0;i<SIZE;++i){
        m.arr[i]=arr[i];
    }
    resultado = msgsnd(this->id, (const void *)&m, sizeof(m), IPC_NOWAIT);

    if (resultado == -1)
    {
        perror("Buzon::Enviar");
        exit(69);
    }

    return resultado;
}

Buzon::Buzon()
{
    //Construir el recurso (msgget)
    this->id = msgget(KEY, IPC_CREAT | 0600);
    if (this->id == -1)
    {
        perror("Buzon::Buzon");
        exit(67);
    }
    this->creador = getpid();
}

Buzon::~Buzon()
{
    int resultado;
    //Destruir el recurso (msgctl)

    if (this->creador == getpid())
    {
        resultado = msgctl(this->id, IPC_RMID, NULL);
        if (resultado == -1)
        {
            perror("Buzon::~Buzon");
            exit(68);
        }
    }
}
int Buzon::Enviar(const char *label, long n, long tipo)
{
    int resultado;
    //Enviar un mensaje tipo tira de caracteres (msgsnd)

    struct msgbuff1 m;
    m.mtype = tipo; 
    m.times = n;
    strcpy(m.label, label);
    resultado = msgsnd(this->id, (const void *)&m, sizeof(m), IPC_NOWAIT);

    if (resultado == -1)
    {
        perror("Buzon::Enviar");
        exit(69);
    }

    return resultado;
}

int Buzon::Recibir(struct msgbuff1 *persona, long tipo)
{
    int resultado;

    resultado = msgrcv(this->id, (void *)persona, sizeof(struct msgbuff1), tipo, IPC_NOWAIT); //Espera por el mensaje
    if (resultado == -1){
            perror("Buzon::Recibir");
            exit(59);
        }
    return resultado;
}
int Buzon::Recibir(struct msgbuff2 *data, long tipo)
{
    int resultado;

    resultado = msgrcv(this->id, (void *)data, sizeof(struct msgbuff2), tipo, IPC_NOWAIT); //Espera por el mensaje
    if (resultado == -1){
            perror("Buzon::Recibir");
            exit(59);
        }
    return resultado;
}
