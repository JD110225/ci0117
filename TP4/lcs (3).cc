//Estudiantes:
    // Jose David Delgado B92570
    // David Rojas B96873
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <vector>
#include <bits/stdc++.h> 
#include <mpi.h>
using namespace std; 
//Variables globales para no tener que pasarlas como parametros
int numProcs;   // Numero de procesos
vector <string> subsecuencias;  //En este vector se almacenan las subsecuencias calculadas
//por cada proceso
int counter=0;      // Mediante esta variable se contabiliza la cantidad de subsecuencias que 
// calcula cada proceso

//Metodo para timear 
void startTimer( struct timeval * timerStart) {
   gettimeofday( timerStart, NULL );
  
}


//Metodo para timear 

double getTimer( struct timeval timerStart ) {
   struct timeval timerStop, timerElapsed;
   gettimeofday(&timerStop, NULL);
   timersub(&timerStop, &timerStart, &timerElapsed);
   return timerElapsed.tv_sec*1000.0+timerElapsed.tv_usec/1000.0;
}

// Calcula la longitud de la subsecuencia comun mas larga entre el string x y
// el string y, emplea la tecnica de "memorizacion"(se amplia sobre esto en la propuesta inicial)
int lcsLongitud(string x, string y, int sizeX, int sizeY, int** matrizResultados) 
{ 
    // caso base de la recursividad
    if (sizeX == 0 || sizeY == 0) 
        return 0;   
    if (matrizResultados[sizeX - 1][sizeY - 1] != -1) 
        return matrizResultados[sizeX - 1][sizeY - 1]; 
    if (x[sizeX - 1] == y[sizeY - 1]) { 
        matrizResultados[sizeX - 1][sizeY - 1] = 1 + lcsLongitud(x, y, sizeX - 1, sizeY - 1, matrizResultados); 
  
        return matrizResultados[sizeX - 1][sizeY - 1]; 
    } 
    else { 
        matrizResultados[sizeX - 1][sizeY - 1] = max(lcsLongitud(x, y, sizeX, sizeY - 1, matrizResultados), 
                               lcsLongitud(x, y, sizeX - 1, sizeY, matrizResultados)); 
  
        return matrizResultados[sizeX - 1][sizeY - 1]; 
    } 
} 
// Encuentra las subsecuencias del parametro str de una longitud en particular y que empiezan
//con la primera letra de str.
void subsequence(string str,int length,int rank)
{        
        for (int j = str.length(); j > 0; j--) 
        {
            string sub_str = str.substr(0, j);
            char char_array[sub_str.length()+1];
            strcpy(char_array,sub_str.c_str());
            if(sub_str.length()==length){
                if(rank!=0){
                    ++counter;
                }
                subsecuencias.push_back(sub_str);
            }
            if((j)>length){
                for (int k = 1; k < sub_str.length() - 1; k++) 
                {
                    string temp = sub_str;
                    temp.erase(temp.begin() + k);
                    subsequence(temp,length,rank);
                }
            }
        }
    }

//Determina si el parametro "subsecuencia" esta presente en el parametro "string 1"
bool checkLcs(const string string1,int string1Length, const string subsecuencia,int subsecuenciaLength){
    if (subsecuenciaLength == 0){
        return true;
    }
    else if(string1Length == 0){
        return false;
    }
    if (string1[string1Length-1] == subsecuencia[subsecuenciaLength-1]){
        return checkLcs(string1,string1Length-1,subsecuencia,subsecuenciaLength-1);
    }
    return checkLcs(string1,string1Length-1,subsecuencia,subsecuenciaLength);
}
// Itera sobre el vector de subsecuencias hasta que encuentra una subsecuencia comun
// entre las dos hileras
string findLcs (const string string){
    std::string longest;
    int m = string.length();
    for (int i = 0; i < subsecuencias.size(); i++){
        if(checkLcs(string,m,subsecuencias[i],subsecuencias[i].length())){
            longest=subsecuencias[i];
            break;
        }
    }
    return longest;
}
//Inicializa la matriz a utilizar en el metodo LCSLongitud
int** generarMatrizInicial(int m,int n){
    int** matrizResultados=new int*[m]; 
    for(int i=0;i<m;++i){
        matrizResultados[i]=new int[n];
    }
    for(int f=0;f<m;++f){
        for(int c=0;c<n;++c){
            matrizResultados[f][c]=-1;
        }
    }
    return matrizResultados;
}
// Envia las subsecuencias de cada proceso al proceso 0(todos los procesos excepto el 0 llaman a este metodo)
void enviarSubsecuencias(){
    for(int i=0;i<counter;++i){
        string sub_str=subsecuencias[i];
        char char_array[sub_str.length()+1];
        strcpy(char_array,sub_str.c_str());
        MPI_Send(char_array,sub_str.length()+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
    }
}
//Este metodo solo lo llama el proceso 0, y se encarga de recibir las subsecuencias
//calculadas por todos los demas procesos
void recibirSubsecuencias(int length){
    int c;
    for(int i=1;i<numProcs;++i){
        MPI_Recv(&c,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int j=0;j<c;++j){
            char buffer[length+1];
            MPI_Recv(buffer,length+1,MPI_CHAR,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            subsecuencias.push_back(buffer);
        }
    }
}
//Prueba serial
void serialTest(string s1,string s2){
    printf("\t\t\tVERSION SERIAL: \n");
    string x,y;
    if(s1.length()>s2.length()){
        x=s2;
        y=s1;
    }
    else{
        x=s1;
        y=s2;
    }
    int m = x.length(); 
    int n = y.length();
    int** matrizResultados=generarMatrizInicial(m,n);
    struct timeval timerstart;
    double used;
    startTimer(&timerstart);
    int length;
    length=lcsLongitud(x, y, m, n, matrizResultados);
    printf("Length: %d\n",length);
    for(int i=0;i<x.length();++i){
        string newX=x.substr(i);
        subsequence(newX,length,0);
    }
    cout<<"LCS ES:"<<findLcs(y)<<endl;
    used=getTimer(timerstart);
    printf("Time used: %f\n",used);
}
//Prueba paralelo
void parallelTest(string s1,string s2,int rank){
    if(rank==0){
        printf("\t\t\tVERSION PARALELA: \n");
    }
    string x,y;
    if(s1.length()>s2.length()){
        x=s2;
        y=s1;
    }
    else{
        x=s1;
        y=s2;
    }
    int m = x.length(); 
    int n = y.length();
    int** matrizResultados=generarMatrizInicial(m,n);
    struct timeval timerstart;
    double used;
    startTimer(&timerstart);
    int length;
    if(rank==0){
        length=lcsLongitud(x, y, m, n, matrizResultados);
        printf("Length: %d\n",length);
    }
    MPI_Bcast(&length,1,MPI_INT,0,MPI_COMM_WORLD);
    int ultimoUtil=x.length()-length;
    int start=ultimoUtil/numProcs*rank;
    int end=ultimoUtil/numProcs*(rank+1);
    // printf("Proceso %d ,calcula subsecuencias que comienzan en indices [%d,%d]\n",rank,start,end);
    for(int i=start;i<end;++i){
        string newX=x.substr(i);
        subsequence(newX,length,rank);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank!=0){
        MPI_Send(&counter,1,MPI_INT,0,0,MPI_COMM_WORLD);
        enviarSubsecuencias();
    }
    else{
        recibirSubsecuencias(x.length());
    }
    if(rank==0){
        cout<<"LCS ES:"<<findLcs(y)<<endl;
        used=getTimer(timerstart);
        printf("Time used: %f\n",used);
    }
}
int main() 
{  
    int rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    string x = "ACGGGATACGATAAACG"; 
    string y = "AACGAAACGATCGA"; 
    if(rank==0){
        serialTest(x,y);
    }
    parallelTest(x,y,rank);
    MPI_Finalize();
    return 0; 
} 
