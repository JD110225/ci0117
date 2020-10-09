#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fstream> 
#include "sstream"
#include "map"
#include "string"
#include <vector>
#include "Mutex.h"
#include <regex>
#include "utility"
using namespace std;
class FileReader{
    public:
    FileReader(string fileName,int workers,int strat);
    string parseTag(string & tag);
    void upper(string & s);
    void read(string);
    bool noEsIgualADocType(string s);  //Por alguna razon !DOCTYPE no pasa los test cases
    int numeroLineas(ifstream& file);
    void searchLine(int lineNum);
    void* readStrat1(void*);
    void* readStrat2(void);
    void* readStrat3(void);
    void* readStratOriginal(void);
    static void* readStrat1Helper(void *);
    static void* readStrat2Helper(void *);
    static void* readStrat3Helper(void *);
    static void* readStratOriginalHelper(void*);
    bool isValidHtml(string);
    void showMapa();
    void addMapa(string palabra);
    map<string,int> getMapa();
    int constructMap();
     ifstream archivo;
     int workers;
     int strat;
     int cantidadLineas;
    private:
     Mutex m;
     map <string,int> mapa;

};