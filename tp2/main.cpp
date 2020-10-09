#include "FileReader.h"
using namespace std;
//Estructura map de la stl para llevar el conteo de las etiquetas html
map<string,int> m;
//Toma un string que consiste en varias palabras separadas por "," y lo transforma en un vector de enteros
vector<int> parseStringToArray(char* s,char delimiter){
   vector<int> vec;
   string token;
   stringstream stream(s);
   while(getline(stream,token,',')){
      vec.push_back(stoi(token));
   }
   return vec;
}
//Hace lo mismo que el metodo anterior pero genera un vector de strings(por el metodo stoi no fue posible el uso templates)
vector<string> parser2(char* s,char delimiter){
   vector<string> vec;
   string token;
   stringstream stream(s);
   while(getline(stream,token,',')){
      vec.push_back((token));
   }
   return vec;
}
//Muestra el mapa general que contiene las etiquetas html y su respectivo conteo.
void showMapa(map<string,int> mapa){
    map<string, int>::iterator iter;
    for (iter = mapa.begin(); iter != mapa.end(); ++iter){
        cout << '\t' << iter->first
            << '\t' << iter->second << '\n';
    }
    cout<<endl;
}
//Actualiza el mapa principal con los contenidos de otro mapa.
void updateMap(map<string,int>mapa){
   for(auto i=mapa.begin();i!=mapa.end();++i){
      map<string, int>::iterator iter;
      iter=m.find(i->first);
      if(iter==m.end()){
         m.insert({i->first,i->second});
      }
      else{
         m[i->first]+=i->second;
      }
   }
}
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
//Crea una instancia de FileReader, se llama una vez por cada archivo HTML que el usuario provee
void crearLector(string file,int workers,int stratNumber){
   struct timeval timerStart;
   double used;
   startTimer( & timerStart );
   FileReader reader(file,workers,stratNumber);
   reader.constructMap();
   used = getTimer( timerStart );
   map<string,int> subMapa=reader.getMapa();
   updateMap(subMapa);
}
//Verifica si un archivo HTML existe.
bool validateFileExistence(vector<string> files){
   bool validos=true;
   for(int i=0;i<files.size() && validos;++i){
      ifstream f(files[i].c_str());
      validos=f.good();
   }
   return validos;
}
//Verifica si la estrategia ingresada por el usuario es valida.
bool estrategiaEnRango(vector<int> estrategias){
   bool enRango=true;
   for(int i=0;i<3 && enRango;++i){
      if(estrategias[i]>4 || estrategias[i]<0){
         enRango=false;
      }
   }
   return enRango;
}
//Verifica la validez del input del usuario
bool validateInput(char* workersPerThread,vector<int> estrategias,vector<string> files){
   bool esValido=true;
   if(estrategias.size()!=files.size()){
      esValido=false;
      cout<<"La cantidad de estrategias no es la misma que la de archivos!!"<<endl;
   }
   if(atoi(workersPerThread)<=0){
      cout<<"Debe existir al menos un trabajador!!"<<endl;
      esValido=false;
   }
   if(!validateFileExistence(files)){
      cout<<"EL archivo no existe!!"<<endl;
      esValido=false;
   }
   if(!estrategiaEnRango(estrategias)){
      cout<<"Se selecciono una estrategia invalida!!"<<endl;
      esValido=false;
   }
   return esValido;
}
int main(int argc, char *argv[]) {
   //Pedir argumentos por terminal mediante el uso de la funcion getopt.
   int *estrategias;
   int option;
   char* workersPerThread=NULL;
   char *estrategia=NULL;
   char* files=NULL;   
   while((option = getopt(argc, argv, "t:e:f:")) != -1){ 
      switch(option){
        case 't':
            workersPerThread=optarg;
            break;
        case 'e':
            estrategia=optarg;
            break;
         case 'f':
            files=optarg;
            break;
      }
   }
   vector<string> vectorArchivos=parser2(files,',');
   int cantidadArchivos=vectorArchivos.size();
   vector<int> vectorEstrategias=parseStringToArray(estrategia,',');
   bool valid=validateInput(workersPerThread,vectorEstrategias,vectorArchivos);
   if(!valid){
      exit(0);
   }
   int stratNumber;
   string file;
   //Por cada archivo, se llama a crearLector
   for(int i=0;i<cantidadArchivos;++i){
      stratNumber=vectorEstrategias[i];   //Estrategia para un archivo en particular
      file=vectorArchivos[i];             //Nombre del archivo a procesar.
      crearLector(file,atoi(workersPerThread),stratNumber);
   }
   cout<<endl;
   cout<<"\t\t\t\t\tConteo de etiquetas HTML: "<<endl;
   cout<<endl;
   //Mostrar la estructura general con las etiquetas y su conteo
   for(auto i=m.begin();i!=m.end();++i){
        cout << '\t' << i->first
       << '\t' << i->second << '\n';
   }
   return 0;
}
  

