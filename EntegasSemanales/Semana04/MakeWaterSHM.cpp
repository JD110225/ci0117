#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <string.h>
#include "Semaforo.h"
#include <iostream>
#define moleculas 5;
using namespace std;
struct Agua{
    int cO=0;
    int cH=0;
};
Semaforo sH(0),sO(0);
struct Agua* agua;
void O(int who){
    if(agua->cH>1){
        cout<<"H's: "<<agua->cH<<", O's: "<<agua->cO+1<<endl;
        agua->cH-=2;
        sH.signal();
        sH.signal();
    }
    else{
        agua->cO++;
        sO.wait();
    }
    exit(0);
}
void H(int who){
    if(agua->cH>0 && agua->cO>0){
        cout<<"H's: "<<agua->cH+1<<" O's: "<<agua->cO<<endl;
        agua->cH--;
        agua->cO--;
        sH.signal();
        sO.signal();
    }
    else{
        agua->cH++;
        sH.wait();
    }
    exit(5);
}
int main(int argc,char** argv){
    // Semaforo s;
    int cantidadMoleculas=moleculas;
    int id= shmget( 123456, 1024, 0700 | IPC_CREAT );
    agua=(Agua *) shmat( id, NULL, 0 );	
    srand(time(0));
    for(int i=0;i<cantidadMoleculas;++i){
        int pid=fork();
        int random=rand()%2;
        if(!pid){
            if(random%2==0){
                cout<<"Se genero un H random"<<endl;
                H(i);
            }
            else{
                cout<<"Se genero un O random"<<endl;
                O(i);
            }          
        }
    }
    for(int i=0;i<cantidadMoleculas;++i){
        int s;
        wait(&s);
    }
    shmdt( agua );		// Se "despega" del area compartida
    shmctl( id, IPC_RMID, NULL );
    return 0;
}