/*
 *  Ejemplo base para el problema de la ronda
 *
 *  CI-0117 Programacion concurrente y paralela
 *  Fecha: 2020/Ago/24
 *
 */

#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "countingSem.h"
#define MaxParticipantes 5
#define papaDefault 10
sem* semaforos; //array de semaforos
// Variable global para el numero de participantes
int participantes = MaxParticipantes;
int papa;
int vivos;
int jugadorInicial;
int cambiarPapa(int papa)
{
    //Reglas de Collatz
    return (papa % 2 == 0) ? (papa / 2) : (3 * papa + 1);
}

void participante()
{
    // printf("Hello world from %i\n",omp_get_thread_num());
    bool vivo = true;
    int who = omp_get_thread_num();
    while (papa != -1) {
        semaforos[who].Wait();
        int nextPlayer=who+1;
        nextPlayer = (nextPlayer > (participantes - 1)) ? 0 :nextPlayer;
        if (vivo && papa != -1) {
           printf("Soy jugador %ld y me llego papa con valor %i \n", who, papa);
            papa = cambiarPapa(papa);
            if (papa == 1) {
                printf("Exploto papa en %i \n", who);
                vivos--;
                vivo = false;
                if (vivos == 1) {
                    papa = -1;
                } 
                else if (vivos>1){
                    papa = random() % 20 + 1; 
                }
            }
        }
        semaforos[nextPlayer].Signal();
    }
    if(vivos==1 && vivo){
        printf("EL ganador es %i\n",omp_get_thread_num());
    }

}

int main(int argc, char** argv)
{
    srand(time(NULL));
    int id, j, resultado;
    papa=papaDefault;
    participantes=MaxParticipantes;
    if (argc > 1) {
        participantes = atoi(argv[1]);
        papa = atoi(argv[2]); 
    }
    if (participantes <= 0) {
        participantes = MaxParticipantes;
    }

    vivos = participantes;
    semaforos=new sem[participantes];
    for(int i=0;i<participantes;++i){
        semaforos[i]=sem(0);
    }
    jugadorInicial = rand()%participantes+1;
    semaforos[jugadorInicial].Signal();
    printf("Creando una ronda de %d participantes\n", participantes);

#pragma omp parallel num_threads(participantes)
    
        participante();

}
