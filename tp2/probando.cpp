#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream> 
#include "sstream"
#include "map"
#include "string"
#define THREADS 4
using namespace std;
map <string,int> mapa;
ifstream archivo("filename.txt");
int cantidadLineas;
// void writePrueba(){
//   ofstream MyFile("filename.txt");
//   MyFile << "Files can be tricky, but it is fun enough!";
//   MyFile.close();  
// }
void showMapa(){
    map<string, int>::iterator iter;
    for (iter = mapa.begin(); iter != mapa.end(); ++iter){
        cout << '\t' << iter->first
            << '\t' << iter->second << '\n';
    } 
}
void addMapa(string palabra){
    map<string, int>::iterator iter;
    iter=mapa.find(palabra);
    if(iter==mapa.end()){
       mapa.insert({palabra,1});
    }
    else{
       mapa[palabra]++;
    }
}

int numeroLineas(ifstream& file){
   int counter=0;
   string line;
   if ( file.is_open() ) {
      while ( getline( file, line ) ) {
        ++counter;
      }
      //file.close();
   }
   return counter;
}
void searchLine(int lineNum){
   string s;
   for(int i=0;i<lineNum;++i){
      getline(archivo,s);
   }
}
void* readStrat1(void* num){
   archivo.clear();
   archivo.seekg(0);
   intptr_t threadNum=(intptr_t)num;
   int start=threadNum * cantidadLineas / THREADS;
   int end= (threadNum + 1) * cantidadLineas / THREADS;
   cout<<"Thread: "<<num<<" start: "<<start<<" end: "<<end<<endl;
   searchLine(start);
   string str;
   for(int i=start;i<end;++i){
      getline(archivo,str);
      string word;
      stringstream s(str);
      while(getline(s,word,' ')){
         addMapa(word);
      }
   }  
}
int getNextValidLine(int start,int num){
   int lineNum=start;
   while(lineNum%THREADS!=num){
      lineNum++;
   }
   return lineNum;
}
void* readStrat2(void *num){
   archivo.clear();
   archivo.seekg(0);
   intptr_t threadNum=(intptr_t)num;
   int counter=1;
   string str;
   while(getline(archivo,str)){
      ++counter;
      stringstream s(str);
      string word;
      if((counter%THREADS)==threadNum){
         while(getline(s,word,' ')){
            addMapa(word);
         }
      }
   }
}
int main(int argc, char *argv[]) {
   cout<<3%4<<endl;
   cantidadLineas=numeroLineas(archivo);
  // writePrueba();
   // int option;
   // char* workersPerThread=NULL;
   // char *estrategia=NULL;
   // while((option = getopt(argc, argv, "t:e:")) != -1){ //get option from the getopt() method
   //    switch(option){
   //      case 't':
   //          workersPerThread=optarg;
   //          break;
   //      case 'e':
   //          estrategia=optarg;
   //          break;
   //    }
   // }
   // cout<<"Workers: "<<workersPerThread<<endl;
   // cout<<"Strat: "<<estrategia<<endl;
    pthread_t threads[THREADS]; 
    for (int i = 0; i < 1; i++) { 
        pthread_create(&threads[i], NULL, readStrat2, (void*)(i)); 
    } 
    for (int i = 0; i < 1; i++)  
        pthread_join(threads[i], NULL); 
   cout<<cantidadLineas<<endl;
   string linea;
   showMapa();
   // archivo.clear();
   // archivo.seekg(0);
   // Use a while loop together with the getline() function to read the file line by line
   // while (getline (archivo, linea)) {
   //    string word;
   //    stringstream s(linea);
   //    while(getline(s,word,' ')){
   //       addMapa(word);  
   //    }
   // }
   // showMapa();
   // archivo.close();  
   return 0;
}
  
