#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h>
#define MAXPARTICIPANTES 10

// Variable global para el numero de participantes
// int participantes = MAXPARTICIPANTES;
int participantesActivos;
int valorPapa;
int commsz;
int participanteInicial;
/*
*  Estructura para el paso de mensajes entre procesos
*/
// struct msgbuff {
//  long mtype;

//  int idParticipanteDeseado; //Esta es a la persona que le debe llegar la papa.
//  int valorPapa;

// };

//   Aplica las reglas de Collatz al valor de la papa
int cambiarPapa(int papa)
{
    if (1 == (papa & 0x1))
    {                                  // papa es impar
        papa = (papa << 1) + papa + 1; // papa = papa * 2 + papa + 1
    }
    else
    {
        papa >>= 1; // n = n / 2, utiliza corrimiento a la derecha, una posicion
    }
    return papa;
}
//Mandar mensaje con papa Y participantes activos y es GG's
int participante(int partId)
{                       
    bool activo = true; 
    int next=((partId+1)%commsz);
    int prev=partId==0?commsz-1:partId-1;
    while (valorPapa!=-1)
    {
    // printf("Previo es: %d\n",prev);
    MPI_Recv(&valorPapa,1,MPI_INT,prev,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    MPI_Recv(&participantesActivos,1,MPI_INT,prev,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    if(activo){
        if(valorPapa!=-1){
            printf("Participante %d recibio papa %d \n",partId,valorPapa);
        }
        else{   //Si sigue activo y el valor de la papa es -1 es el ganador!
            printf("\n\t\t\tEL GANADOR ES: %d!!!\n\n",partId);
        }
        valorPapa=cambiarPapa(valorPapa);
        if(valorPapa==1){
            printf("La papa fue estallada por el participante %d \n",partId);
            participantesActivos--;
            if(participantesActivos>1){
                printf("Quedan %d participantes \n",participantesActivos);
            }
            activo=false;
            valorPapa=10;
        }
    }
    else if(valorPapa!=-1){
        printf("Participante %d es un jugador pasivo...\n",partId);
    }
    if(participantesActivos==1){
        valorPapa=-1;
    }
    usleep(10); 
    MPI_Send(&valorPapa,1,MPI_INT,next,0,MPI_COMM_WORLD);   
    MPI_Send(&participantesActivos,1,MPI_INT,next,0,MPI_COMM_WORLD);   
    }
    
}
int main(int argc, char **argv)
{
    int id, i, j, resultado;
    srand(time(NULL));
    valorPapa=rand()%10+1;  
    int receive;
    int send=10;                             // Ya tenemos el num de participantes
    int myrank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &commsz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    // srand(time(NULL)+myrank);

    //MPI ES THRASH 
    // if(myrank==0){
    //     int random=rand()%commsz;
    //     printf("Huh\n");
    //     MPI_Send(&send,1,MPI_INT,1,0,MPI_COMM_WORLD);
    // }
    // else{
    //     MPI_Recv(&receive,1,MPI_INT,(myrank-1)%commsz,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    //     printf("Soy: %d\n",myrank);
    //     MPI_Send(&receive,1,MPI_INT,(myrank+2)%commsz,0,MPI_COMM_WORLD);
    // }

    
    participantesActivos=commsz;
    participanteInicial=rand()%commsz;
    int prev=participanteInicial==0?commsz-1:participanteInicial-1;
    if(myrank==prev){
        // printf("Inicial : %d\n",participanteInicial);
        MPI_Send(&valorPapa,1,MPI_INT,participanteInicial,0,MPI_COMM_WORLD);
        MPI_Send(&participantesActivos,1,MPI_INT,participanteInicial,0,MPI_COMM_WORLD);
    }
    participante(myrank);
    MPI_Finalize();
    return 0;
}