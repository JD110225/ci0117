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
#define MaxParticipantes 10
#define papaDefault 10
sem* semaforos; //array de semaforos
// omp_lock_t* locks;
// Variable global para el numero de participantes
int participantes = MaxParticipantes;
int papa;
int vivos;
int jugadorInicial;
int cambiarPapa(int papa)
{
    return (papa % 2 == 0) ? (papa / 2) : (3 * papa + 1);
}

void participante()
{
    // printf("Hello world from %i\n",omp_get_thread_num());
    bool vivo = true;
    while (papa != -1) {
       int who = omp_get_thread_num();
        if (who != jugadorInicial) {
            // omp_set_lock(&locks[who]);
            semaforos[who].Wait();
        } else {
            jugadorInicial = -420;
        }
        int tipo=who+1;
        // printf("Who: %i\n",who);
        tipo = (tipo > (participantes - 1)) ? 0 :tipo;
        // printf("Incial: %i\n",jugadorInicial);
        // omp_unset_lock(&locks[tipo]);
        if (vivo && papa != -1) {
           printf("Soy jugador %ld y me llego papa con valor %i \n", who, papa);
            papa = cambiarPapa(papa);
            if (papa == 1) {
                printf("Exploto papa en %i \n", who);
                vivos--;
                vivo = false;
                if (vivos == 1) {
                    // printf("El ganador es %i \n", who);
                    papa = -1;
                } 
                else if (vivos>1){
                    papa = random() % 20 + 1; //Pasa una papa random
                }
            }
        }
        // omp_unset_lock(&locks[tipo]);
        semaforos[tipo].Signal();
    }
    if(vivos==1 && vivo){
        printf("EL ganador es %i\n",omp_get_thread_num());
    }

}

int main(int argc, char** argv)
{
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

    // locks = new omp_lock_t[participantes];
    semaforos=new sem[participantes];
    // for (int i = 0; i < participantes; ++i) {
    //     omp_init_lock(&locks[i]);
    // }

    //omp_unset_lock(&locks[0]);
    srandom(time(NULL));
    //Valor Inicial Papa check for negatives
    //jugadorInicial = random() % participantes + 1; //Jugar random que empieza
    jugadorInicial = 0;

    // m.setData(papaInicial, jugadorInicial);

    printf("Creando una ronda de %d participantes\n", participantes);
    // El programa principal decidirá cual es el primer participante en arrancar y el valor inicial de la papa

    //testing
    // for (int i = 0; i < participantes; ++i) {
    //     omp_set_lock(&locks[i]);
    // }

#pragma omp parallel num_threads(participantes)
    
        participante();
    

    // for (int i = 0; i < participantes; ++i) {
    //     omp_destroy_lock(&locks[i]);
    // }
}
