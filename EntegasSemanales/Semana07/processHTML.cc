/**
 *  Programa para procesar un archivo de texto línea por línea
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Set/19
 *
**/

#include <stdio.h>
#include <string>
#include <regex>
#include <fstream>
#include <iostream>
using namespace std;
/**
 *  Divide la línea en sus componentes
 *  Utiliza la función strtok que es insegura para usar con PThreads
 *  Utiliza expresiones regulares para separar la línea en los tokens indicados
 *
**/
/*
   EL uso de regex_replace es reemplazar todos los caracteres < y > por "" , basicamente los elimina.
*/
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
void processLine( const char * line ) {
   char * token;
   std::regex texto( ">[^<]*<" );

   regex_replace( line, texto, "> <" );

   token = strtok( (char * ) line, "< >\t\n" );
   printf( "Tokens: " );
   while ( NULL != token ) {
      printf( "%s, ", token );
      token = strtok( NULL, "< >\t\n" );
   }
   printf( "\n" );

}


/**
 *  Abre un archivo que es pasado por parámetro y lo procesa línea por línea
 *
**/
int main( int count, char ** values ) {
   ifstream archivo( values[ 1 ] );
   std::string line;
   // printf("%d\n",numeroLineas(ifstream(values[1])));
   printf("\nNumero de lineas: %d\n\n",numeroLineas(archivo));
//"Sets a new value for the stream's internal error state flags."
   archivo.clear();
   //"Sets the position of the next character to be extracted from the input stream"
   archivo.seekg(0);
   if ( archivo.is_open() ) {
      while ( std::getline( archivo, line ) ) {
        processLine( line.c_str() );
      }
      archivo.close();
   }

}

