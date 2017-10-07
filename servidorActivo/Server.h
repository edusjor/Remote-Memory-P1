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
    int portNum = 8888;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[1024];

    struct sockaddr_in server_addr;
    socklen_t size;

    char buff[1024];

    static void* playSocket(void* var);

public:
    Server();
    void aceptarEimprimir();

    int socketClient(int puerto); 
    static void sincronizar();
    int verifServPas();
    string enviarDato(string dato);
    string enviarTodo();

};



class Sincronizacion{
public:
    //Sincronizacion();
    void sincronizar();
private:
    int verifServPas();
    string enviarDato(string dato);
    string enviarTodo();
    int socketClient(int);

    int puertoPasSINC=7777;   //este es el puerto del serverSINC en pasivo
    int client_SINC;
    char buffer_SINC[1024];
    int bufsize = 1024;
    int n;
    int flagPasivoisON=false;
};



#endif //SERVIDORACTIVO_SERVER_H
