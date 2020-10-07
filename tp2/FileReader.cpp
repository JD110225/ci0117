#include "FileReader.h"
// Como en los pthreads solo se puede pasar un argumento, se utilizan
// esta variable global como si fuera un argumento de una funcion de pthreads.
int numThread=0;
FileReader::FileReader(string fileName,int workers,int strat ){
    this->archivo=ifstream(fileName);
    this->workers=workers;
    this->strat=strat;
    this->m=Mutex();
    this->cantidadLineas=numeroLineas(archivo);
}
//Muestra el mapa
void FileReader::showMapa(){
    map<string, int>::iterator iter;
    for (iter = mapa.begin(); iter != mapa.end(); ++iter){
        cout << '\t' << iter->first
            << '\t' << iter->second << '\n';
    }
    cout<<endl;
}
void FileReader::upper(string &s){
   for(auto & c:s){
      c=toupper(c);
   }
}
string FileReader::parseTag(string& s){
   string newS;
   int counter=0;
   while(counter!=s.length() && s[counter]!='>' && s[counter]!=' '){
      newS+=s[counter++];
   }
   return newS;
}
//meme
bool FileReader::noEsIgualADocType(string s){
   return s!="!DOCTYPE";
}
//Agrega una palabra al mapa
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
void FileReader::read(string linea){
   // cout<<"Linea"<<": "<<linea<<endl;
   string word;
   stringstream s(linea);
   while(getline(s,word,'<')){
         if(isalnum(word[0]) || word[0]=='/' || word[0]=='!'){
            // cout<<"Palabra: "<<word<<endl;
         // if(isValidHtml(word)){
               addMapa(word);
         }

         // }
      }
}

 void* FileReader::readStrat1(void){
   m.Lock();
   string l;
   int lineasAsignadas=cantidadLineas/this->workers;
   if(numThread!=this->workers-1){
      string str;
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
 }
void* FileReader::readStrat1Helper(void *context){
    return ((FileReader*)context)->readStrat1();
}
void* FileReader::readStrat2(void){
   m.Lock();
   archivo.clear();
   archivo.seekg(0);
   intptr_t threadNum=(intptr_t)numThread;
   int counter=1;
   string str;
   while(getline(archivo,str)){
      stringstream s(str);
      string word;
      if((counter%workers)==threadNum){
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
void* FileReader::readStrat2Helper(void *context){
    return ((FileReader*)context)->readStrat2();
}
void* FileReader::readStrat3(void){
   string str;
   while(getline(archivo,str)){
      read(str);
   }
}
void* FileReader::readStrat3Helper(void* context){
   return ((FileReader*)context)->readStrat3();
}
void* FileReader::readStratOriginal(void){
   m.Lock();
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
}
void* FileReader::readStratOriginalHelper(void* context){
   return ((FileReader*)context)->readStratOriginal();
}
map<string,int> FileReader::getMapa(){
   return this->mapa;
}
int FileReader::constructMap(){
   archivo.clear();
   archivo.seekg(0);
    pthread_t threads[workers]; 
    srand(time(NULL));
    for (int i = 0; i < workers; i++) {
        numThread=i; 
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
        usleep(200);
    } 
    for (int i = 0; i < workers; i++)  
        pthread_join(threads[i], NULL); 
   return 0;
}