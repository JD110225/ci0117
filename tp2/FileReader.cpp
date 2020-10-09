#include "FileReader.h"
// Como en los pthreads solo se puede pasar un argumento, se utilizan
// esta variable global como si fuera un argumento de una funcion de pthreads.
int numThread;
//Constructor de la clase FileReader,recibe nombre de archivo, cantidad de
// trabajadores y estrategia a utilizar.
FileReader::FileReader(string fileName,int workers,int strat ){
    numThread=0;
    this->archivo=ifstream(fileName);
    this->workers=workers;
    this->strat=strat;
    this->m=Mutex();
    this->cantidadLineas=numeroLineas(archivo);
}
//Muestra el mapa de un archivo HTML(para debuggear nada mas)
void FileReader::showMapa(){
    map<string, int>::iterator iter;
    for (iter = mapa.begin(); iter != mapa.end(); ++iter){
        cout << '\t' << iter->first
            << '\t' << iter->second << '\n';
    }
    cout<<endl;
}
//Recibe un string por referencia y convierte todas sus letras a mayusculas.
void FileReader::upper(string &s){
   for(auto & c:s){
      c=toupper(c);
   }
}
//Recibe un string con < y > al inicio y al final y averigua cual es la etiqueta HTML correcta
string FileReader::parseTag(string& s){
   string newS;
   int counter=0;
   while(counter!=s.length() && s[counter]!='>' && s[counter]!=' '){
      newS+=s[counter++];
   }
   return newS;
}
//El programa del profe dice que doctype no es una etiqueta, asi que...
bool FileReader::noEsIgualADocType(string s){
   return s!="!DOCTYPE";
}
//Agrega una palabra al mapa local
void FileReader::addMapa(string palabra){
    map<string, int>::iterator iter;
    string newP=parseTag(palabra);
    upper(newP);
    iter=this->mapa.find(newP);
    if(noEsIgualADocType(newP)){
      if(iter==this->mapa.end()){
         this->mapa.insert({(newP),1});
      }
      else{
         this->mapa[newP]++;
      }
    }
}
//Realiza un conteo de la cantidad total de lineas en el archivo.
int FileReader::numeroLineas(ifstream& file){
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
//Lee y agrega al mapa
void FileReader::read(string linea){
   // m.Lock();
   string word;
   stringstream s(linea);
   while(getline(s,word,'<')){
         if(isalnum(word[0]) || word[0]=='/' || word[0]=='!'){
               addMapa(word);
         }
      }
   // m.Unlock();
}
//Primera estrategia de lectura,mapeo estatico equitativo
 void* FileReader::readStrat1(void* arg){
   m.Lock();
   numThread++; 
   string l;
   int lineasAsignadas=cantidadLineas/this->workers;
   string str;
   if(numThread!=this->workers-1){
   for(int i=0;i<lineasAsignadas;++i){
      getline(archivo,l);
      read(l);
      }
   }
   else{
      while(getline(archivo,l)){
         read(l);
      }
   }
   
   m.Unlock();
   pthread_exit(0);
 }
 //No se puede llamar a pthread con un metodo de una clase,asi que
//  se usan funciones estaticas auxiliares que llaman a una estrategia especifica
void* FileReader::readStrat1Helper(void *context){
     return ((FileReader*)context)->readStrat1((FileReader*)context);
      pthread_exit(0);

   }
//Segunda estrategia de lectura(modulo)
void* FileReader::readStrat2(void){
   m.Lock();
   numThread++;
   archivo.clear();
   archivo.seekg(0);
   // intptr_t threadNum=(intptr_t)numThread;
   int counter=1;
   string str;
   getline(archivo,str);
   while(getline(archivo,str)){
      stringstream s(str);
      string word;
      if((counter%workers+1)==numThread){  //workers+1 para que la operacion de residuo este entre 1 y workers ya que threadNum empieza siendo uno
         while(getline(s,word,'<')){
              if(isalnum(word[0]) || word[0]=='/' || word[0]=='!')
               addMapa(word);
         }
      }
      ++counter;
   }
   m.Unlock();
   pthread_exit(0);
}
 //No se puede llamar a pthread con un metodo de una clase,asi que
//  se usan funciones estaticas auxiliares que llaman a una estrategia especifica
void* FileReader::readStrat2Helper(void *context){
    return ((FileReader*)context)->readStrat2();
}
//Tercera estrategia de lectura, mapeo dinamico, NECESARIO uso de Mutex 
void* FileReader::readStrat3(void){
   string str;
   while(getline(archivo,str)){
      m.Lock();
      cout<<"Leyo el hilo: "<<numThread<<endl;
      read(str);
      m.Lock();
   }
}
 //No se puede llamar a pthread con un metodo de una clase,asi que
//  se usan funciones estaticas auxiliares que llaman a una estrategia especifica
void* FileReader::readStrat3Helper(void* context){
   return ((FileReader*)context)->readStrat3();
}
//Estrategia original:
   // Es una variacion del mapeo estatico equitativo por bloque,con la diferencia
   // de que a cada hilo en vez de tocarle una cantidad de lineas igual a X, donde
   // X es el cociente de la cantidad de trabajadores y la cantidad de hilos, a los hilos
   // les corresponde una cantidad de lineas aleatoria entre 1 y X, esto implica que al
   // ultimo hilo en llegar a la funcion se le asignen todas las lineas restantes 
   //(e.g: Si en mapeo estatico a cada hilo le corresponden 5 lineas, entonces en esta estrategia
   // a cada hilo le corresponderian de 1 a 5 lineas y al ultimo todas las restantes)
void* FileReader::readStratOriginal(void){
   m.Lock();
   numThread++;
   string s;
   int cantidadMaxima=(cantidadLineas/this->workers);
   int lineasAsignadas=(rand()%cantidadMaxima)+1;   //EL +1 es para que a un thread le corresponda al menos 1 linea.
   if(numThread!=this->workers-1){
      for(int i=0;i<lineasAsignadas;++i){
         getline(archivo,s);
         read(s);
      }
   }
   else{
      while(getline(archivo,s)){
         read(s);
      }  
   }
   m.Unlock();
   pthread_exit(0);
}
 //No se puede llamar a pthread con un metodo de una clase,asi que
//  se usan funciones estaticas auxiliares que llaman a una estrategia especifica
void* FileReader::readStratOriginalHelper(void* context){
   return ((FileReader*)context)->readStratOriginal();
}
//Esta funcion es llamada desde el main para obtener la estructura mapa local de un archivo
map<string,int> FileReader::getMapa(){
   return this->mapa;
}
//Crea los hilos
int FileReader::constructMap(){
    archivo.clear();
    archivo.seekg(0);
    pthread_t threads[workers]; 
    srand(time(NULL));
    vector<pair<FileReader*,int>> vect;
    for(int i=0;i<3;++i){
       pair<FileReader*,int> aPair=make_pair(this,i);
       vect.push_back(aPair);
    }
    for (int i = 0; i < workers; i++) {
        std:pair<FileReader*,int> a(this,i);
        switch (this->strat)
        {
        case 1:
            pthread_create(&threads[i], NULL,readStrat1Helper,this); 
            break;
        case 2:
            pthread_create(&threads[i], NULL,readStrat2Helper,this);
            break;
         case 3:
            pthread_create(&threads[i], NULL,readStrat3Helper,this); 
            break;
         case 4:
            pthread_create(&threads[i], NULL,readStratOriginalHelper,this); 
            break;
        default:
            break;
        }
      usleep(2000);     //Es necesario ...entre mayor el sleep mas estable el programa..
    } 
    for (int i = 0; i < workers; i++){ 
        pthread_join(threads[i], NULL); 
    }
   return 0;
}
