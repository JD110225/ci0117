

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "Buzon2.h"
#define KEY 0xB12345 
#define SIZE 9
struct msgbuff1
{
    long mtype; // message type, must be > 0
    int valorPapa;
    // long times;  // Times that label appears
    // char label[64];
    //char * label;
};
// struct msgbuff2{
//     long mtype;
//     double arr[SIZE];
// };
// int Buzon::Enviar(double* arr,long mtype){
//     int resultado;
//     struct msgbuff2 m;
//     m.mtype = mtype; 
//     for(int i=0;i<SIZE;++i){
//         m.arr[i]=arr[i];
//     }
//     resultado = msgsnd(this->id, (const void *)&m, sizeof(m), IPC_NOWAIT);

//     if (resultado == -1)
//     {
//         perror("Buzon::Enviar");
//         exit(69);
//     }

//     return resultado;
// }

Buzon2::Buzon2()
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

Buzon2::~Buzon2()
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
int Buzon2::Enviar(long tipo,int valorPapa)
{
    int resultado;
    //Enviar un mensaje tipo tira de caracteres (msgsnd)

    struct msgbuff1 m;
    m.mtype = tipo; 
    m.valorPapa = valorPapa;
    // strcpy(m.label, label);
    resultado = msgsnd(this->id, (const void *)&m, sizeof(m), IPC_NOWAIT);
    if (resultado == -1)
    {
        perror("Buzon::Enviar");
        exit(69);
    }
    return resultado;
}

int Buzon2::Recibir(struct msgbuff1 *data, long tipo)
{
    int resultado;

    resultado = msgrcv(this->id, (void *)data, sizeof(struct msgbuff1), tipo, IPC_NOWAIT); //Espera por el mensaje
    if (resultado == -1){
            perror("Buzon::Recibir");
            exit(59);
        }
    return resultado;
}
// int Buzon::Recibir(struct msgbuff2 *data, long tipo)
// {
//     int resultado;

//     resultado = msgrcv(this->id, (void *)data, sizeof(struct msgbuff2), tipo, IPC_NOWAIT); //Espera por el mensaje
//     if (resultado == -1){
//             perror("Buzon::Recibir");
//             exit(59);
//         }
//     return resultado;
// }
