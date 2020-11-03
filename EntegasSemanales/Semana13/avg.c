
// Author: Wes Kendall
// Copyright 2012 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Program that computes the average of an array of elements in parallel using
// MPI_Scatter and MPI_Allgather
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>
// Computes the average of an array of numbers
int** generarMatrizAleatoria(int size) {
    // srand((int)time(0));
    int** matriz = (int**)malloc((size*size*sizeof(int)));
    for (int i = 0; i < size; ++i) {
        matriz[i] = (int*)malloc((size*size*sizeof(int)));
    }
    for (int f = 0; f < size; ++f) {
        for (int c = 0; c < size; ++c) {
            matriz[f][c] = (rand() % 10)+1;   //valor random entre 0 y 10.

        }
    }
    return matriz;
}
int* alphaChadStrat(int** matriz,int size){
  int* arraryRepresentation=(int*) malloc(size*size*sizeof(int));
  for(int f=0;f<size;++f){
    for(int c=0;c<size;++c){
        arraryRepresentation[f*size+c]=matriz[f][c];
    }
  }
  return arraryRepresentation;
}
float compute_avg(float *array, int num_elements) {
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elements; i++) {
    sum += array[i];
  }
  return sum / num_elements;
}
void viewArray(int* array,int size){
      for(int i=0;i<size;++i){
            printf("%d ",array[i]);
      }
      printf("\n");
}

int main(int argc, char** argv) {
  srandom(time(0));
  int size=3;
  int** m1 = generarMatrizAleatoria(size);
  int** m2 = generarMatrizAleatoria(size);
  int *m1Array=alphaChadStrat(m1,size);
  int *m2Array=alphaChadStrat(m2,size);
  MPI_Init(NULL, NULL);
  int world_rank;
  int world_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  if(world_rank==0){
    viewArray(m1Array,size*size);
    viewArray(m2Array,size*size);
  }
  int* subArray1=(int*)malloc(size*sizeof(int));
  int* subArray2=(int*)malloc(size*sizeof(int));
  MPI_Scatter(m1Array,size,MPI_INT,subArray1,size,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Scatter(m2Array,size,MPI_INT,subArray2,size,MPI_INT,0,MPI_COMM_WORLD);

  printf("Process: %i\n",world_rank);
  viewArray(subArray1,size);
  viewArray(subArray2,size);

  // float *rand_nums =(float *)malloc(sizeof(float) * num_elements_per_proc*world_size);;
  // if (world_rank == 0) {
  //       for(int i=0;i<num_elements_per_proc*world_size;++i){
  //             rand_nums[i]=i*2;
  //       }
  //   viewArray(rand_nums,num_elements_per_proc*world_size);
  // }

  // float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);

  // MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums,
  //             num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // float sub_avg = compute_avg(sub_rand_nums, num_elements_per_proc);
  // float *sub_avgs = (float *)malloc(sizeof(float) * world_size);
  // MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT,0,MPI_COMM_WORLD);

  // float avg = compute_avg(sub_avgs, world_size);
  // if(world_rank==0){
  //   printf("Avg of all elements from proc %d is %f\n", world_rank, avg);
  // }

  // // Clean up
  // if (world_rank == 0) {
  //   free(rand_nums);
  // }
  // free(sub_avgs);
  // free(sub_rand_nums);
  MPI_Finalize();
}



