#include <iostream>
#include <sys/time.h>
#include <vector>
#include <numeric>
#include <unistd.h>
#define hilos 16
#define limite 200
using namespace std;
void startTimer( struct timeval * timerStart) {
   gettimeofday( timerStart, NULL );
}


/*
 *  time elapsed in ms
 */
double getTimer( struct timeval timerStart ) {
   struct timeval timerStop, timerElapsed;
   gettimeofday(&timerStop, NULL);
   timersub(&timerStop, &timerStart, &timerElapsed);
   return timerElapsed.tv_sec*1000.0+timerElapsed.tv_usec/1000.0;
}
void* solucion(void* t){
    intptr_t threadNum=(intptr_t)t;
    int start=threadNum * limite / hilos;
    int end= (threadNum + 1) * limite / hilos;
    cout<<"Inicio: "<<start<<endl;
    cout<<"Fin: "<<end<<endl; 
}
int main(){
    long hilo;
    pthread_t * pthilos;
    pthilos = (pthread_t *) calloc( hilos, sizeof( pthread_t ) );
    struct timeval timerStart;
    double used;
 startTimer( & timerStart );
    for ( hilo = 0; hilo <hilos; hilo++ ) {
        pthread_create( & pthilos[ hilo ], NULL, solucion, (void *) hilo );
   }   
    for ( hilo = 0; hilo < hilos; hilo++ ) {
        pthread_join( pthilos[ hilo ], NULL );
    }
    used = getTimer( timerStart );
    cout<<"Time spent CONCURRENTE: "<<used<<endl;
    return 0;
}