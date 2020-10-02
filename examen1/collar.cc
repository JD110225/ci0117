/**
 * Represent a pearl collar
 * With Black, White, and Transparent pearls
 * Transparent pearl can be counted as White or Black pearls in sequences
 * @author	Programacion Concurrente
 * @version	2020/Oct/2
 * Primer examen parcial
 *
**/

#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

#include "collar.h"

/**
 * Default contructor
 * Create a random length collar and fill with (allowed) colored pearls
 *
**/
Collar::Collar( int length ) {
   int posicion;
   char perla = 'T';

   srand( getpid() );
 
   this->collar = (char *) calloc( length + 1, sizeof( char ) );
   for ( posicion = 0; posicion < length; posicion++ ) {
      switch ( random() % 3 ) {
         case 0:
            perla = 'B';
            break;
         case 1:
            perla = 'T';
            break;
         case 2:
            perla = 'N';
            break;
      }
      this->collar[ posicion ] = perla;
   }
}


/**
 * Parameter constructor
 * @param	char	an array of chars with collar pearl's distribution
 *
**/
Collar::Collar( const char * collar ) {
   char perla;
   int posicion;

   this->collar = (char *) calloc( strlen( collar ) + 1, sizeof( char ) ); 
   for (int posicion = 0; posicion < strlen( collar ); posicion++ ) {
      switch ( collar[ posicion ] ) {	// The collar can only have Black, White, and Transparent pearls, any other letter is changed to Transparent
         case 'B':
         case 'N':
         case 'T':
            this->collar[ posicion ] = collar[ posicion ];
            break;
         default:
            this->collar[ posicion ] = 'T';
            break;
      }
   }
}


/**
 *
 *
**/
Collar::~Collar() {

   free( this->collar );

}


/**
 * Return a String with collar composition
**/
char * Collar::toString() {
   return this->collar;
}

