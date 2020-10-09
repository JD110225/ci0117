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
#include "thread"
#include <mutex>
#include "sys/time.h"
using namespace std;
class FileReader{
    public:
    FileReader(string fileName,int workers,int strat);
    string parseTag(string & tag);
    void upper(string & s);
    void read(string);
    int numeroLineas(ifstream& file);
    void searchLine(int lineNum);
    void readStrat1(int);
    void readStrat2(int);
    void readStrat3(int);
    void readStratOriginal(int);
    bool isValidHtml(string);
    void showMapa();
    void addMapa(string palabra);
    map<string,int> getMapa();
    int constructMap();
    private:
    ifstream archivo;
    int workers;
    int strat;
    int cantidadLineas;
    std::mutex mut;
    Mutex m;
    map <string,int> mapa;

};