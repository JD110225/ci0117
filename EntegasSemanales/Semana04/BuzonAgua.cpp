

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "BuzonAgua.h"
#define KEY 0xB12345 
#define SIZE 9
struct Agua{
    long mtype;
    int cO=0;
    int cH=0;
};
struct chopSticks{
    long mtype;
    bool chopsticks[5];
};
BuzonAgua::BuzonAgua(){
    //Construir el recurso (msgget)
    this->id = msgget(KEY, IPC_CREAT | 0600);
    if (this->id == -1)
    {
        perror("Buzon::Buzon");
        exit(67);
    }
    this->creador = getpid();
}

BuzonAgua::~BuzonAgua()
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
int BuzonAgua::Enviar(bool* chop,long tipo){
    int resultado;
    struct chopSticks chops;
    chops.mtype=tipo;
    for(int i=0l;i<5;++i){
        chops.chopsticks[i]=chop[i];
    }
    resultado = msgsnd(this->id, (const void *)&chops, sizeof(chops), IPC_NOWAIT);
    if (resultado == -1)
    {
        perror("Buzon::Enviar");
        exit(69);
    }
    return resultado;
}
int BuzonAgua::Enviar(long tipo,int cO,int cH)
{
    int resultado;
    struct Agua m;
    m.mtype = tipo; 
    m.cO = cO;
    m.cH=cH;
    resultado = msgsnd(this->id, (const void *)&m, sizeof(m), IPC_NOWAIT);
    if (resultado == -1)
    {
        perror("Buzon::Enviar");
        exit(69);
    }
    return resultado;
}
int BuzonAgua::Recibir(struct chopSticks* chops,long tipo){
    int resultado;
    resultado=msgrcv(this->id, (void*)chops,sizeof(struct chopSticks),tipo,IPC_NOWAIT);
    if (resultado == -1){
            perror("Buzon::Recibir");
            exit(59);
        }
    return resultado;
}
int BuzonAgua::Recibir(struct Agua *data, long tipo)
{
    int resultado;

    resultado = msgrcv(this->id, (void *)data, sizeof(struct Agua), tipo, IPC_NOWAIT); //Espera por el mensaje
    if (resultado == -1){
            perror("Buzon::Recibir");
            exit(59);
        }
    return resultado;
}
