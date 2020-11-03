#include <stdio.h>
#include <string.h>
#include <mpi.h>
const int MAXSTRING = 100;
int main(void){
    char greeting[MAXSTRING];
    int commsz=3;
    int myrank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &commsz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if(myrank != 0){
        sprintf(greeting, "Greetings from process %d of %d!",myrank, commsz);
        MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0,MPI_COMM_WORLD);
        }
        else{
            printf("Greetings from process %d of %d!\n", myrank,commsz);
            for(int q = 1; q < commsz; q++){
                MPI_Recv(greeting, MAXSTRING, MPI_CHAR, q,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("%s\n", greeting);
            }
    }
    MPI_Finalize();
    return 0;
}