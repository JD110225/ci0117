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
    // if(partId==participanteInicial){
    //     MPI_Recv(&valorPapa,1,MPI_INT,prev,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    //     MPI_Recv(&participantesActivos,1,MPI_INT,prev,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    // }
    // MPI_Send(&valorPapa,1,MPI_INT,next,0,MPI_COMM_WORLD);
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
            valorPapa=rand()%10+1;
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
    // MPI_Send(&valorPapa,1,MPI_INT,next,0,MPI_COMM_WORLD);   
  
        //     msgrcv(id,& m, sizeof(int)*3, 2028,0); //Espera por mensaje, incluso si no hay ahorita. Por la flag de 0
        //     printf( "Soy %d. La papa tiene un valor de %d\n", partId, m.valorPapa );
        //     if(m.idParticipanteDeseado == partId && activo && m.valorPapa>=0){ //EL mensaje es para mi, estoy activo, Papa no es negativa
        //       printf( "-----------Llego la papa al participante %d\n", partId );
        //       if(m.participantesActivos ==1){//Solo quedo yo en el juego
        //         //Yo gane
        //         m.valorPapa= -1;
        //         activo=false; //Me pongo como inactivo, ya gane y termino el juego
        //         m.participantesActivos--; //Habrian 0 participantes activos
        //         printf( "HAY UN GANADOR! El participante %d ha ganado la ronda! \n", partId );
        //       }else{//Sigue Ronda. Aplico Collatz
        //         m.valorPapa= cambiarPapa(m.valorPapa);
        //         if(m.valorPapa== 1){//Papa estallo
        //           activo=false;
        //           m.participantesActivos--;
        //           m.valorPapa= rand()%998+2; //Nuevo valor aleatorio de la papa, en el mismo intervalo posible que el random inicial del main
        //           printf( "\n\n\n**********ESTALLA LA PAPA al participante %d,se genera un nuevo valor de la papa.\n", partId  );
        //         }else{
        //           printf( "Nuevo valor de la papa: %d \n", m.valorPapa );
        //         }
        //       }
        //       m.idParticipanteDeseado = ((partId - 1) + 1) % participantes + 1; //Se lo paso a la siguiente persona en forma circular.
        //     }
        //     if(m.idParticipanteDeseado == partId && !activo){ //Me pasan la papa, pero yo estoy inactivo
        //       printf( "Participante %d esta inactivo. Se debe pasar la papa al participante siguiente\n", partId );
        //       m.idParticipanteDeseado = ((partId - 1) + 1) % participantes + 1;
        //     }

        //     //Hora de mandar msj. Si yo no era el participante deseado, lo mantengo igual.
        //     printf( "Envio la papa hacia el participante %d con el valor de papa: %d \n", m.idParticipanteDeseado, m.valorPapa );
        //     msgsnd(id, &m, sizeof(int)*3, IPC_NOWAIT);
        //     //Como es un circulo, cualquier i que gane y ponga valor negativo de la papa, lo pasara hacia la misma direccion y llegara hasta la persona i-1 en el respectivo modulo. Lo que significa que todos haran exit, y quedara 1 mensaje en la cola que se eliminara cuando se borre el buzon
        //     if(m.valorPapa<0){//Si valor de la papa es negativo
        //       printf( "VALOR NEGATIVO, ID: %d se sale \n", partId );
        //       _exit( 0 );	//Juego termino, el proceso sale
        //     }
        //     usleep(100);
        //   }
    
}

int main(int argc, char **argv)
{

    int id, i, j, resultado;
    //Setup para mensajes
    //   id = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    //   if (-1==id){
    //     perror("msgget");
    //     exit(1);
    //   }
    // if (argc > 1)
    // {
    //     participantes = atoi(argv[1]);
    // }
    // if (participantes <= 0)
    // {
    //     participantes = MAXPARTICIPANTES;
    // } // Ya tenemos el num de participantes

    //   struct msgbuff m;
    srand(time(NULL));
    //Valores iniciales para que empieze el juego
    //   m.mtype= 2028;
    //  int idParticipanteDeseado = 9; //  Este es el participante que empieza.
                                                           //   m.participantesActivos= participantes;
                                                              //   printf( "Juego de la papa! \nEmpieza el participante %d \n", m.idParticipanteDeseado );
    //Ver si mandaron segundo parametro para el valor de la papa
    // if (argc > 2)
    // {
    //     valorPapa = atoi(argv[2]);
    // }
    valorPapa=rand()%10+1;                               // Ya tenemos el num de participantes
    // int commsz;
    int myrank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &commsz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    participantesActivos=commsz;
    participanteInicial=rand()%commsz;
    // participantes=commsz;
    if(myrank==0){
        printf("Inicial : %d\n",participanteInicial);
        // idParticipanteDeseado = rand() % (commsz) + 1; //  Este es el participante que empieza.        // MPI_Send(&valorPapa,1,MPI_INT,idParticipanteDeseado,0,MPI_COMM_WORLD);
        MPI_Send(&valorPapa,1,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Send(&participantesActivos,1,MPI_INT,1,0,MPI_COMM_WORLD);
    }
    participante(myrank);
    MPI_Finalize();
    return 0;
}