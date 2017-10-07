//
// Created by edulubu on 28/08/17.
//

#ifndef SERVIDORACTIVO_SERVER_H
#define SERVIDORACTIVO_SERVER_H



#include <iostream>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include "Client.h"

#define PORT 30666

using namespace std;

class Server {

private:
    static vector<Client> clients;

    int client, servere;
    int portNum = 8889;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[1024];

    struct sockaddr_in server_addr;
    socklen_t size;

    char buff[1024];

    static void* playSocket(void* var);

    static void* iniciarSincro(void*);




public:
    Server();
    void aceptarEimprimir();

    int socketClient(int puerto);


};












class Sincronizacion{
public:
    //Sincronizacion();
    void sincronizar(); //el cliente que envia 
    void* socketServerSINC();  //para el server q escucha

    static void thread_socketServerSINC();
private:

    //verifica si el activo esta o no activo
    int verifServAct(); //arreglar
    string enviarDato(string dato);//no se usa en el pasivo
    string enviarTodo();//arreglar

    int socketClientSINC(int);
    int puertoPasSINC=7779;
    int client_SINC;
    char buffer_SINC[1024];
    int bufsize = 1024;
    int n;
    int flagPasivoisON=false;

    ///////////////////////////
    //para el thread que recibe los datos de sincronizacion del activo
    int puertoPasSINC_serv=7777;
    int client_serv;
    char buffer_SINC_serv[1024];
    int n_serv;

    struct sockaddr_in server_addr;
    socklen_t sizeSINC_serv;

    
    
    static void* playSocket_SINC_serv(void* var);
    
    
    
};



#endif //SERVIDORACTIVO_SERVER_H
