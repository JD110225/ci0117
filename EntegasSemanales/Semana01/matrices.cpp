#include <time.h>
#include <iostream>
using namespace std;
int* getColumna(int** m, int size,int num){
    int* col = new int[size];
    for (int i=0;i<size;++i){
        col[i] = m[i][num];
    }
    return col;
}
int* getFila(int **m,int size,int num){
    int* fila = new int[size];
    for (int i = 0; i < size; ++i) {
        fila[i] = m[num][i];
    }
    return fila;
}
int arrayMultiplication(int* a, int* b, int size) {
    int counter = 0;
    for (int i = 0; i < size; ++i) {
        counter += a[i] * b[i];
    }
    return counter;
}
int** matrixMultiplication(int** a, int** b,int size){
    int** matrizResultado = new int* [size];
    for (int f = 0; f < size; ++f) {
        matrizResultado[f] = new int[size];
    }
    for (int f = 0; f < size; ++f) {
        for (int c = 0; c < size; c++){
            int* fila = getFila(a,size,f);
            int* col = getColumna(b,size,c);
            matrizResultado[f][c] = arrayMultiplication(fila, col, size);

        }
    }
    return matrizResultado;
}
int** generarMatrizAleatoria(int size) {
    srand((int)time(0));
    int** matriz = new int* [size];
    for (int i = 0; i < size; ++i) {
        matriz[i] = new int[size];
    }
    for (int f = 0; f < size; ++f) {
        for (int c = 0; c < size; ++c) {
            matriz[f][c] = (rand() % 10)+1;   //valor random entre 0 y 10.

        }
    }
    return matriz;
}
void displayMatrix(int** m,int size){
    for (int f = 0; f < size; ++f) {
        for (int c = 0; c < size; ++c) {
            cout << m[f][c]<<" ";
        }
        cout << "\n";
}
}
int main()
{
    int size;
    cout << "Digite la dimension de las matrices cuadradas a multiplicar: " << endl;
    cin >> size;
    int** m1 = generarMatrizAleatoria(size);
    int** m2 = generarMatrizAleatoria(size);
    clock_t tStart = clock();
    matrixMultiplication(m1, m2, size);
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC); //stack overflow:https://stackoverflow.com/questions/876901/calculating-execution-time-in-c 
    // cout << "M1: "<<endl;
    // displayMatrix(m1, size);
    // cout << "M2: "<<endl;
    // displayMatrix(m2, size);
    // cout << "Matriz resultado: " << endl;
    // displayMatrix(matrixMultiplication(m1, m2, size),size);
}
