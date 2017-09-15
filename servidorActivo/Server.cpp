//
// Created by edulubu on 28/08/17.
//

#include "Server.h"


#include <iostream>

#include "list.h"
#include "list.cpp"
#include "Client.h"


#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>



Server::Server() {

    int yes = 1;

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    client = socket(AF_INET, SOCK_STREAM, 0);
    //Client *c;
    if (client < 0) {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket server has been created..." << endl;


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    /* ---------- BINDING THE SOCKET ---------- */
    /* ---------------- bind() ---------------- */

    //Avoid bind error if the socket was not close()'d last time;
    setsockopt(client,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

    if ((bind(client, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        //return -1;
    }


    //size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;

    /* ------------- LISTENING CALL ------------- */
    /* ---------------- listen() ---------------- */

    listen(client, 5);
    cout << "Escuchado";
    //accept(client, (struct sockaddr *) &server_addr, &size);
    //se mantiene escuchando aqui hasta que se conecte algun cliente
}

// void *Server::playSocket(void *args);


Client *c;
void Server::aceptarEimprimir() {


    while (1) {

        c = new Client();




        int clientCount = 1;

        c->sock = accept(client, (struct sockaddr *) &server_addr, &size);
        cout << client << endl;
        cout<<c->sock<<endl<<endl<<endl;

        // first check if it is valid or not
        if (c->sock < 0) {
            cout << "=> Error on accepting..." << endl;
        } else {

            //strcpy(buffer, "=> Server connected...\n");

           // pthread_t t1;

            //pthread_create(&t1, NULL, &playSocket, NULL);
            //void* result;
            //pthread_join(t1,&result);

            //std::thread t(playSocket);
            //t.join();
            //playSocket()
        }
    }
}




void* Server::playSocket(void* var){

    //Client *c =ss(Client *) args;
    char buffer[1024], message[1024];
    int n;

    //mutex

    //Before adding the new client, calculate its id. (Now we have the lock)
    //c->SetId(Server::clients.size());
    //sprintf(buffer, "Client n.%d", c->id);
//    c->SetName(buffer);
//    cout << "Adding client with id: " << c->id << endl;
//    Server::clients.push_back(*c);

    //cout << "Yeeah baby1";

    //mutex fin
    send(c->sock, "Servidor listo para recibir datos", 1024, 0);
    while(1){
        send(c->sock, "confirmado ", 1024, 0);
        memset(buffer, 0, sizeof buffer);
        n = recv(c->sock, buffer, sizeof buffer, 0);

        if(n == 0) {
            cout << "Client " << " diconnected" << endl;
            close(c->sock);
            break;
        }
        else{
            if (n<0){
                cerr << "error mientras se recive el mensaje del cliente "<<endl;
            }
            else{
                //Mensaje received.
                snprintf(message, sizeof message, buffer);
                cout << "Will send to all: " << message << endl;

            }
        }
    }

    //End thread
    //return NULL;

}
////////////////////////////////////////////////