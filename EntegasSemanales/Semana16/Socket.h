/**
 *   Socket class interface
 *   CI0117 Programación paralela y concurrente
 *   Grupos 2 y 3
 *
**/
#ifndef Socket_h
#define Socket_h
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 
class Socket{

   public:
      Socket();
      Socket( int, int );
      Socket( int );
      ~Socket();
      int Connect( char *, int );
      void Close();
      int Read( void *, int );
      int Write( void *, int );
      int Listen( int );
      int Bind( int );
      Socket * Accept();

   private:
      int socketID;
      bool ipv6;
};

#endif

