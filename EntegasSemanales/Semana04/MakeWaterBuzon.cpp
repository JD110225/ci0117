#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <string.h>
#include "Semaforo.h"
#include "BuzonAgua.h"
#include <iostream>
#define moleculas 5;
using namespace std;
struct Agua{
    long mtype;
    int cO=0;
    int cH=0;
};
struct Agua* agua;
struct Agua receive;
BuzonAgua b;
int st;
Semaforo sH(0),sO(0);
void O(int who){
    b.Recibir(&receive,2020);
    if(receive.cH>1){
        cout<<"H's: "<<receive.cH<<", O's: "<<receive.cO+1<<endl;
        receive.cH-=2;
        sH.signal();
        sH.signal();
    }
    else{
        receive.cO++;
        sO.wait();
    }
    b.Enviar(2020,receive.cH,receive.cO);
    usleep(100);
    exit(6);
}
void H(int who){
    b.Recibir(&receive,2020);
    cout<<"Hello?"<<receive.cH<<endl;
    if(receive.cH>0 && receive.cO>0){
        cout<<"H's: "<<receive.cH+1<<" O's: "<<receive.cO<<endl;
        receive.cH--;
        receive.cO--;
        sH.signal();
        sO.signal();
    }
    else{
        agua->cH++;
        sH.wait();
    }
    b.Enviar(2020,receive.cH,receive.cO);
    usleep(100);
    exit(5);
}
int main(int argc,char** argv){
    b.Enviar(2020,0,0);
    // int st=b.Recibir(&receive,15);
    // cout<<receive.cH<<endl;
    int cantidadMoleculas=moleculas;
    // int id= shmget( 123456, 1024, 0700 | IPC_CREAT );
    // agua=(Agua *) shmat( id, NULL, 0 );	
    srand(time(0));
    for(int i=0;i<cantidadMoleculas;++i){
        int pid=fork();
        int random=rand()%2;
        if(!pid){
            // b.Recibir(&receive,2020);
            if(random%2==0){
                cout<<"Se genero un H random"<<endl;
                H(i);
            }
            else{
                cout<<"Se genero un O random"<<endl;
                O(i);
            } 
;         
        }
    }
    for(int i=0;i<cantidadMoleculas;++i){
        int s;
        wait(&s);
    }
    // // shmdt( agua );		// Se "despega" del area compartida
    // // shmctl( id, IPC_RMID, NULL );
    return 0;
}