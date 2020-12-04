#include <iostream>
#include <string>
#include <omp.h>
#include <vector>
using namespace std; 
//Cantidad de cores: 4
//Nota: El algoritmo serial funciona correctamente, el de OMP tiene condiciones
// de carrera que no logre encontrar 
vector<string> vectorPalindromos;   // se guardan los substrings que son palindromos aqui
//Metodo auxiliar para verficar si una hilera corresponde a un palindromo
bool isPalindrome(string s,int start,int end){
    if(start==end){
        return true;
    }
    if(s[start]!=s[end]){
        return false;
    }
    if(start<end+1){
        return isPalindrome(s,start+1,end-1);
    }
    return true;
}
//Inserta en el vector las substrings de s que corresponden a palindromos.
void computePalindromeSubstrings(string s,int threadNum){
    vectorPalindromos.clear();
   // string sub;
   //Se coloca el pragma en el for mas externo para maximizar la cantidad
   // de trabajos que realiza cada hilo, tiene condiciones de carrera 
   // posiblemente al insertar en el vector.
    #pragma omp parallel for num_threads(threadNum)
    for(int i=0;i<s.length();++i){
        //cout<<"Hello from thread: "<<omp_get_thread_num()<<endl;
        string sub=s.substr(i);
        for(int j=sub.length();j>0;--j){
            sub.erase(j,1);
            if(isPalindrome(sub,0,sub.length()-1)){
                cout<<sub<<endl;
                vectorPalindromos.push_back(sub);
            // cout<<sub<<endl;
            }
        }
    }
} 
void printLongest(){
    int mayorTamano=-1;
    //Se itera 1 vez para calcular la longitud del palindromo mas grande
    for(auto str:vectorPalindromos){
        int l=str.length();
        if(l>mayorTamano){
            mayorTamano=str.length();
        }
    }
    cout<<"Longitud de mayor palindromo: "<<mayorTamano<<endl;
    //Se itera una segunda vez para imprimir el palindromo de dicha longitud
    for(auto str:vectorPalindromos){
        if(str.length()==mayorTamano){
            cout<<str<<endl;
            break;
        }
    }
}
int main(){
    string s="reconocer";
    string s2="Los romanos tienen amor a roma";
    // computePalindromeSubstrings(s,2);
    // printLongest();
    double start,end;
    start=omp_get_wtime();
    computePalindromeSubstrings(s2,16);
    printLongest();
    end=omp_get_wtime();
    cout<<"Time spent: "<<end-start<<endl;
    return 0;
}