#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include "BuzonAgua.h"
#include <iostream>
using namespace std;
struct chopSticks{
    long mtype;
    bool chopsticks[5];
};
BuzonAgua b;
struct chopSticks receive;
bool* chops=new bool[5];
void initializeChopsticks(){
    for(int i=0;i<5;++i){
       chops[i]=0;
    } 
}
void displayArray(bool* arr,int size){
    for(int i=0;i<size;++i){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}
int filosofo(int i){
    int chop1 =  (i < 4)? i: 0;                
    int chop2 = (i < 4)? i+1: 4;
    int counter=0;
    while(counter<3){
        b.Recibir(&receive,2020);
        if(receive.chopsticks[chop1]==0 && receive.chopsticks[chop2]==0){
             ++counter;
            receive.chopsticks[chop1]=1;
            receive.chopsticks[chop2]=1;
            b.Enviar(receive.chopsticks,2020);
            printf("Filosofo %d esta comiendo\n",i);
            receive.chopsticks[chop1]=0;
            receive.chopsticks[chop2]=0;
        }
        else{
            printf("Filosofo %d esta pensando\n",i);
        }
        b.Enviar(receive.chopsticks,2020);
    } 
    printf("\t\tEl filosofo %d comio %d veces\n",i,3);
    exit(0);
    return 0;
}
int main(int argc,char **argv){
    initializeChopsticks();
    b.Enviar(chops,2020);
    int pid;
    for(int i=0;i<5;++i){
        pid=fork();
        if(!pid){
            filosofo(i);
        }
    }
    for(int i=0;i<5;++i){
        int status;
        wait(&status);
    }
    return 0;
}