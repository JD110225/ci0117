#include <stdio.h>
#include <stdlib.h>
#include "Socket.h"
Socket::Socket(int a, int boolValue){
    if ((this->socketID = socket(AF_INET, SOCK_STREAM, boolValue)) < 0) 
    { 
        printf("\n Error al crear el socket! \n"); 
    } 
}
Socket::Socket(int a){
    if ((this->socketID = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Error al crear el socket! \n"); 
    } 
}
Socket::~Socket(){

}
int Socket::Connect(char* addressServer,int portNumber){
    struct sockaddr_in serv_addr; 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(portNumber); 
    if(inet_pton(AF_INET, addressServer, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
    int val=0;
    if (connect(socketID, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\n Hmm Failed \n"); 
        val=-1; 
    } 
    return val;
}
void Socket::Close(){
    close(socketID);
}
int Socket::Read(void* buffer,int sizeBuffer){
    read(socketID,buffer,sizeBuffer);
}
int Socket::Write(void* request,int requestLength){
    write(socketID,request,requestLength);
}
int Socket::Listen(int server){
    int val=0;
   if (listen(server, 3) < 0) 
    { 
        perror("Error en listen"); 
        exit(0); 
    } 
    return val;
}
int Socket::Bind(int server){
    struct sockaddr_in address; 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
    if (bind(server, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("Error en bind"); 
        exit(0); 
    }  

}
Socket* Socket::Accept(){
    Socket* nuevoSocket=new Socket(20);
    // if ((nuevoSocket = accept(server_fd, (struct sockaddr *)&address,  
    //                    (socklen_t*)&addrlen))<0) 
    // { 
    //     perror("accept"); 
    //     exit(EXIT_FAILURE); 
    // } 
}