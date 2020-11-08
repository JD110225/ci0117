#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#define sizeDefault 4
int** generarMatrizAleatoria(int size) {
    // srand((int)time(0));
    int** matriz = (int**)malloc((size*size*sizeof(int)));
    for (int i = 0; i < size; ++i) {
        matriz[i] = (int*)malloc((size*sizeof(int)));
    }
    for (int f = 0; f < size; ++f) {
        for (int c = 0; c < size; ++c) {
            matriz[f][c] = (rand() % 10)+1;   //valor random entre 0 y 10.

        }
    }
    return matriz;
}
int* matrixToArray(int** matriz,int size){
  int* arrayRepresentation=(int*) malloc(size*size*sizeof(int));
  for(int f=0;f<size;++f){
    for(int c=0;c<size;++c){
          arrayRepresentation[f*size+c]=matriz[f][c];
    }
  }
  return arrayRepresentation;
}
int **arrayToMatrix(int *array,int size){
  int **matriz=(int**)malloc(size*size*sizeof(int));
  for (int i = 0; i < size; ++i) {
      matriz[i] = (int*)malloc((size*sizeof(int)));
  }
  for(int i=0;i<size;++i){
    for(int j=0;j<size;++j){
      matriz[i][j]=array[i*size+j];
    }
  }
  return matriz;
}
void verMatriz(int **matriz,int size){
  for(int i=0;i<size;++i){
    for(int j=0;j<size;++j){
      printf("%d ",matriz[i][j]);
    }
    printf("\n");
  }
}
void viewArray(int* array,int size){
      for(int i=0;i<size;++i){
            printf("%d ",array[i]);
      }
      printf("\n");
}
int* getColumna(int** m, int size,int num){
    int* col = (int*)malloc(size*sizeof(int));
    for (int i=0;i<size;++i){
        col[i] = m[i][num];
    }
    return col;
}
int productoPunto(int* a,int* b,int size){
    int counter = 0;
    for (int i = 0; i < size; ++i) {
        counter += a[i] * b[i];
    }
    return counter;
}
int* multMatrizVector(int** matriz,int *vector,int size){
  int* resultado=(int*)malloc(size*sizeof(int));
  for(int i=0;i<size;++i){
      int* columna=getColumna(matriz,size,i);
      resultado[i]=productoPunto(vector,columna,size);
    }
  return resultado;
}
int main(int argc, char** argv) {
  srandom(time(0));
  int size=sizeDefault;
  int** m1 = generarMatrizAleatoria(size);
  int** m2 = generarMatrizAleatoria(size);
  int *m1Array=matrixToArray(m1,size);
  double start,end;
  MPI_Init(NULL, NULL);
  int rank;
  int s;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &s);
  start=MPI_Wtime();
  int* subArray=(int*)malloc(size*sizeof(int));
  MPI_Scatter(m1Array,size,MPI_INT,subArray,size,MPI_INT,0,MPI_COMM_WORLD);
  int* subresult=multMatrizVector(m2,subArray,size);
  if(rank==0){
    printf("Matriz 1: \n");
    verMatriz(m1,size);
    printf("Matriz 2: \n");
    verMatriz(m2,size);
  }
  int *m=(int*)malloc(size*size*sizeof(int));
  MPI_Gather(subresult,size,MPI_INT,m,size,MPI_INT,0,MPI_COMM_WORLD);
  if(rank==0){
    int** resultado=arrayToMatrix(m,size);
    printf("Matriz resultado: \n");
    verMatriz(resultado,size);
  }
  end=MPI_Wtime();
  if(rank==0){
    printf("Time spent: %f\n",end-start);
  }
  MPI_Finalize();
}



