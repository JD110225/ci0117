//Usage:./m.exe -t 3 -e 1,2 -f filename1.txt,filename2.txt
#include "FileReader.h"
using namespace std;
//TODO:
   // 1)Juntar los mapas. (CHECK)
   // 2)Revisar el searchLine de la primera estrategia(CHECK,CORREGIDO)
   // 3)Mapeo Dinamico
   // 4)Mapeo original
   // 5)Que actually cuente etiquetas y no cualquier cosa (CHECK)
   // 6) Manejar errores de entrada de datos
   // 7)Fixear problemas de merge en github.
   // 8)Corregir presentacion datos(mayuscula y sin <>) (CHECK)
// Como en los pthreads solo se puede pasar un argumento, se utilizan
// estas dos variables globales como si fueran argumentos de la funcion "lec",
// encargada de crear instancias de la clase FileaReader.
string file;
int stratNumber;
map<string,int> m;
vector<int> parseStringToArray(char* s,char delimiter){
   vector<int> vec;
   string token;
   stringstream stream(s);
   while(getline(stream,token,',')){
      vec.push_back(stoi(token));
   }
   return vec;
}
//Lamentablemente no se pueden usar templates por el metodo stoi...
vector<string> parser2(char* s,char delimiter){
   vector<string> vec;
   string token;
   stringstream stream(s);
   while(getline(stream,token,',')){
      vec.push_back((token));
   }
   return vec;
}

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
//Crea una instancia de FileReader, esta funcion se pasa como argumento a pthread_create.
void crearLector(int workers){
   FileReader reader(file,workers,stratNumber);
   reader.constructMap();
   map<string,int> subMapa=reader.getMapa();
   updateMap(subMapa);
}
int main(int argc, char *argv[]) {
   int *estrategias;
   int option;
   char* workersPerThread=NULL;
   char *estrategia=NULL;
   char* files=NULL;
   while((option = getopt(argc, argv, "t:e:f:")) != -1){ //get option from the getopt() method
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
   //  while(optind < argc){
   //      ++optind;    
   //       vectorArchivos.push_back(argv[optind]);
   //  }
   vector<int> vectorEstrategias=parseStringToArray(estrategia,','); 
   // cout<<"Cantidad Trabajadores/thread: "<<workersPerThread<<endl;
   // cout<<"Estrategias: "<<estrategia<<endl;
   // cout<<"Cantidad archivos: "<<cantidadArchivos<<endl;
   for(int i=0;i<cantidadArchivos;++i){
      stratNumber=vectorEstrategias[i];
      file=vectorArchivos[i];
      // FileReader reader(file,atoi(workersPerThread),stratNumber);
      crearLector(atoi(workersPerThread));
      // reader.constructMap();
      // reader.showMapa();
   }
   map<string,int> prueba;
   for(auto i=m.begin();i!=m.end();++i){
        cout << '\t' << i->first
       << '\t' << i->second << '\n';
   }
   //  pthread_t threads[cantidadArchivos]; 
   //  for (int i = 0; i < cantidadArchivos; i++) {
   //      stratNumber=vectorEstrategias[i];
   //      file=vectorArchivos[i];
   //      pthread_create(&threads[i],NULL,lec,(void*)((atoi)(workersPerThread))); 
   //      usleep(100);
   //  } 
   //  for (int i = 0; i < cantidadArchivos; i++)  
   //      pthread_join(threads[i], NULL); 
   return 0;
}
  
