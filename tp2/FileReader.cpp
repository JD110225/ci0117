#include "FileReader.h"
//Constructor de la clase FileReader,recibe nombre de archivo, cantidad de
// trabajadores y estrategia a utilizar.
FileReader::FileReader(string fileName,int workers,int strat ){
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
//Agrega una palabra al mapa local
void FileReader::addMapa(string palabra){
    map<string, int>::iterator iter;
    string newP=parseTag(palabra);
    upper(newP);
    iter=this->mapa.find(newP);
    if(newP[0]!='!'){
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
   string word;
   stringstream s(linea);
   while(getline(s,word,'<')){
         if(isalnum(word[0]) || word[0]=='/' || word[0]=='!'){
               addMapa(word);
         }
      }
}
//Primera estrategia de lectura,mapeo estatico equitativo
 void FileReader::readStrat1(int x){
   mut.lock();
   string s;
   int cantidadMaxima=(cantidadLineas/this->workers);
   int lineasAsignadas=(rand()%cantidadMaxima)+1;   //EL +1 es para que a un thread le corresponda al menos 1 linea.
   if(x!=this->workers-1){
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
   mut.unlock();
 }
//Segunda estrategia de lectura(modulo)
void FileReader::readStrat2(int numThread){
   mut.lock();
   archivo.clear();
   archivo.seekg(0);
   int counter=1;
   string str;
   getline(archivo,str);
   while(getline(archivo,str)){
      stringstream s(str);
      string word;
      if((counter%workers)==numThread){  
         while(getline(s,word,'<')){
              if(isalnum(word[0]) || word[0]=='/' || word[0]=='!')
               addMapa(word);
         }
      }
      ++counter;
   }
   mut.unlock();
}
//Tercera estrategia de lectura, mapeo dinamico
void FileReader::readStrat3(int numThread){
   string str;
   while(getline(archivo,str)){
      mut.lock();
      read(str);
      mut.unlock();
   }
}
//Estrategia original:
   // Es una variacion del mapeo estatico equitativo por bloque,con la diferencia
   // de que a cada hilo en vez de tocarle una cantidad de lineas igual a X, donde
   // X es el cociente de la cantidad de trabajadores y la cantidad de hilos, a los hilos
   // les corresponde una cantidad de lineas aleatoria entre 1 y X, esto implica que al
   // ultimo hilo en llegar a la funcion se le asignen todas las lineas restantes 
   //(e.g: Si en mapeo estatico a cada hilo le corresponden 5 lineas, entonces en esta estrategia
   // a cada hilo le corresponderian de 1 a 5 lineas y al ultimo todas las restantes)
void FileReader::readStratOriginal(int numThread){
   mut.lock();
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
   mut.unlock();
}
//Esta funcion es llamada desde el main para obtener la estructura mapa local de un archivo
map<string,int> FileReader::getMapa(){
   return this->mapa;
}
//Crea los hilos y llama a la estrategia correspondiente.
int FileReader::constructMap(){
    archivo.clear();
    archivo.seekg(0);
    pthread_t threads[workers]; 
    srand(time(NULL));
   for (int i = 0; i < workers; i++) {
        switch (this->strat)
        {
        case 1:{
            std::thread hilo(&FileReader::readStrat1,this,i);
            hilo.join();
            break;
        }
        case 2:{
            std::thread hilo(&FileReader::readStrat2,this,i);
            hilo.join();
            break;
        }
         case 3:{
            std::thread hilo(&FileReader::readStrat3,this,i);
            hilo.join();         
            break;
         }
         case 4:{
            std::thread hilo(&FileReader::readStratOriginal,this,i);
            hilo.join(); 
            break;
         }
         default:
            break;
         }
   }
   return 0;
}
