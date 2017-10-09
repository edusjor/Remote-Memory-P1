//
// Created by edulubu on 28/08/17.
//
//Servidor ACTIVO
#ifndef SERVIDORACTIVO_SERVER_H
#define SERVIDORACTIVO_SERVER_H



#include <iostream>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Client.h"

#define PORT 30666

using namespace std;

//server activo
class Server {
private:
    static vector<Client> clients;

    int client, servere;
    int portNum = 8888; //puerto de activo 
    bool isExit = false;
    int bufsize = 1024;
    char buffer[1024];

    struct sockaddr_in server_addr;
    socklen_t size;

    char buff[1024];

    static void* playSocket(void* var);

    int socketClient(int puerto); 
    static void sincronizar();
    int verifServPas();
    string enviarDato(string dato);
    string enviarTodo();

public:
    Server();
    void aceptarEimprimir();

    

};



class Sincronizacion{
public:
    //Sincronizacion();
    void sincronizar(string);
private:
    int verifServPas();
    void enviarDato(string dato);
    void enviarTodo();
    int socketClient(int);

    int puertoPasivo=8889;   //este es el puerto del serverSINC en pasivo
    int client_SINC;
    char buffer_SINC[1024];
    int bufsize = 1024;
    int n;
    //int flagPasivoisON=false;
};



#endif //SERVIDORACTIVO_SERVER_H
