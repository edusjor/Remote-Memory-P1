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


Client *c;
void Server::aceptarEimprimir() {


    while (1) {

        c = new Client();

        int clientCount = 1;

        cout<<"inicio : "<<c->sock<<endl<<endl<<endl;
        c->sock = accept(client, (struct sockaddr *) &server_addr, &size);
        //cout << "cliente: "<<client << endl;
         cout<<"final : "<<c->sock<<endl<<endl<<endl;
        cout<<"cliente : "<<c->sock<<endl<<endl<<endl;

        // first check if it is valid or not
        if (c->sock < 0) {
            cout << "=> Error on accepting..." << endl;
        } else {

          
            pthread_t thread_id;
            pthread_create( &thread_id , NULL ,  playSocket , (void*) &c->sock );         

        }
    }
}


void* Server::playSocket(void* socket_desc){
//Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));

    message = "Now type something and i shall repeat what you type \n";
    write(sock , message , strlen(message));

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //end of string marker
        client_message[read_size] = '\0';
        cout << "Mensaje del cliente: "<<client_message<<endl;
        //Send the message back to client
        write(sock , client_message , strlen(client_message));
        cout << "Mensaje del servidor: "<<client_message<<endl;

        //clear the message buffer
        memset(client_message, 0, 2000);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)

    {
        perror("recv failed");
    }
    }
////////////////////////////////////////////////