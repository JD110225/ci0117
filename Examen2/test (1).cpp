#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;
//Cantidad de cores: 4
int myRank;
int numProcs;
int tuple1;
int tuple2;
void pasoMensajesDecreciente(int fact){
    int anterior;
    int next;
    if(myRank==(numProcs-1)){
        anterior=0;
    }
    else{
        anterior=myRank+1;
    }
    if(myRank==0){
        next=numProcs-1;
    }
    else{
        next=myRank-1;
    }
  if(myRank==(numProcs-1)){        //Proceso 0 hace el envio inicial
    tuple1=tuple2=fact;
    MPI_Send(&tuple1, 1, MPI_INT, next, 0,
           MPI_COMM_WORLD);
    MPI_Send(&tuple2,1,MPI_INT,next,0,MPI_COMM_WORLD);
  }
    while(tuple2!=1){
            //printf("Soy %d y mi anterior es %d\n",myRank,anterior);
            MPI_Recv(&tuple1, 1, MPI_INT, anterior, 0, MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
            MPI_Recv(&tuple2, 1, MPI_INT, anterior, 0, MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
            if(tuple2!=1){
                printf("Process %d received (%d,%d) from process %d\n", myRank, tuple1,tuple2,anterior);
            }
        if(tuple2!=1){
            tuple1=tuple1*(tuple2-1);
            --tuple2;
            if(tuple2==1){  
                printf("Process %d calculo (%d,%d)\n",myRank,tuple1,tuple2);
            }
        }
        MPI_Send(&tuple1, 1, MPI_INT, next, 0,
            MPI_COMM_WORLD);
        MPI_Send(&tuple2, 1, MPI_INT, next, 0,
            MPI_COMM_WORLD);
    }
}
void pasoMensajes(int fact){
    int tuple1,tuple2;
    int anterior;
    int next=(myRank + 1) % numProcs;
    if(myRank==0){
        anterior=numProcs-1;
    }
    else{
        anterior=myRank-1;
    }
  if(myRank==0){        //Proceso 0 hace el envio inicial
    tuple1=tuple2=5;
    MPI_Send(&tuple1, 1, MPI_INT, next, 0,
           MPI_COMM_WORLD);
    MPI_Send(&tuple2,1,MPI_INT,next,0,MPI_COMM_WORLD);
  }
    while(tuple2!=1){
            MPI_Recv(&tuple1, 1, MPI_INT, anterior, 0, MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
            MPI_Recv(&tuple2, 1, MPI_INT, anterior, 0, MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
            if(tuple2!=1){
                printf("Process %d received (%d,%d) from process %d\n", myRank, tuple1,tuple2,anterior);
            }
        if(tuple2!=1){
            tuple1=tuple1*(tuple2-1);
            --tuple2;
            if(tuple2==1){  
                printf("Process %d calculo (%d,%d)\n",myRank,tuple1,tuple2);
            }
        }
        MPI_Send(&tuple1, 1, MPI_INT, next, 0,
            MPI_COMM_WORLD);
        MPI_Send(&tuple2, 1, MPI_INT, next, 0,
            MPI_COMM_WORLD);
    }
}
int main(int argc,char** argv){
    MPI_Init(NULL, NULL);
    int fact=5; //5 por default
    if(argc==2){
        fact=stoi(argv[1]);
    }
    //cout<<"Factorial: "<<fact<<endl;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    double t1,t2;
    t1=MPI_Wtime();
    //MPI_Barrier(MPI_COMM_WORLD);
    pasoMensajes(fact);
    //tuple1=tuple2=stoi(argv[1]);
    printf("\n");
   // pasoMensajesDecreciente(fact);
    t2=MPI_Wtime();
    if(myRank==0){
        cout<<"Time spent: "<<t2-t1<<endl;
    }
    MPI_Finalize(); 
    return 0;
}