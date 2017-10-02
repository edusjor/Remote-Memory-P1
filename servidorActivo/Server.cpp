//
// Created by edulubu on 28/08/17.
//

#include "Server.h"
#include "ListaGenerica.h"
#include "Client.h"
#include "Client.cpp"

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <cstdlib>



List<string> list_1; //lista global
int usoDeMemoria=0;
Server::Server() {

    int yes = 1;

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    client = socket(AF_INET, SOCK_STREAM, 0);
    
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
    }

    cout << "=> Looking for clients..." << endl;

    /* ------------- LISTENING CALL ------------- */
    /* ---------------- listen() ---------------- */

    listen(client, 5);
}


Client *c;
void Server::aceptarEimprimir() {

    while (1) {
        
        c = new Client(); //reserva un nuevo "espacio" para el siguiente cliente que se conecte

        //espera aqui hasta que algun cliente se conecte entonces lo acepta y lo asigna al espacio reservado
        c->sock = accept(client, (struct sockaddr *) &server_addr, &size); 

        // first check if it is valid or not
        if (c->sock < 0) {
            cout << "=> Error on accepting..." << endl;
        } 
        else {
            //crea el nuevo thread que manejara al nuevo cliente y sigue en el loop esperando nuevos clientes
            pthread_t thread_id;
            pthread_create( &thread_id , NULL ,  playSocket , (void*) &c->sock );         
        }
    }
}

//Entra aqui por cada nueva conexion con un cliente
//Aqui se maneja todas las peticiones del cliente
void* Server::playSocket(void* socket_desc){

    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[1024];

    message = "Aceptado";
    write(sock , message , strlen(message));

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 1024 , 0)) > 0 )
    {

        //end of string marker
        client_message[read_size] = '\0';
        cout <<endl<<endl<< "Mensaje recibido del cliente: "<<client_message<<endl;
        
        

        //itera el string del char recibido en el mensaje 
        //para separar la llave, el valor y el tamano
        string strclient_message = string(client_message); //convierte el mensaje del cliente a string
        string operacion="",llave="",valor="",size="" ;
        
        int cont=0;
        string separador="#";
        for(int i=0; i < strclient_message.length(); i++){
            
            if ((strclient_message[i])==separador[0]){
                cont+=1;
            }
            else{
                if(cont==0)
                    operacion=operacion+strclient_message[i];

                if(cont==1)
                    llave=llave+strclient_message[i];

                if(cont==2)
                    valor=valor+strclient_message[i];

                if(cont==3)
                    size=size+strclient_message[i];
            }        
        }

        string operacion1="guardarValor";
        string operacion2="getValor";
        string operacion3="getMemoryUsage";
        string operacion4="getAllMemoryValues";

        ////////////////////////////////////////
        //guardarValor
        if(operacion==operacion1){ 
            //rev
            int nummRandom=rand();
            string llave=to_string(nummRandom); //llave aleatoria numerica creada en el server se pasa a string

            list_1.add_head(llave,valor,size);   //guarda la llave, el valor y tamano del dato
            
            char *chrLlave = &llave[0u]; //convierte la llave de string a char

            write(sock , chrLlave , strlen(chrLlave)); //envia la llave creada al cliente
 

            cout<<"//////////////////////////////////////////////"<<endl;
            cout << "Llave creada: "<<llave<<endl;
            cout << "Valor: "<<valor<<endl;
            cout << "Size of valor: "<< sizeof(valor) <<endl; 

            
            usoDeMemoria+=sizeof(valor);
            memset(chrLlave, 0, 1024);
        
        }




        //getValor
        if (operacion==operacion2){ 
            string datoRetornado=list_1.searchData(llave);
            cout<<"dato retornado: "<<datoRetornado<<endl;
            if (datoRetornado!="0"){
                char *chrDato = &datoRetornado[0u]; //convierte el dato de string a char
                write(sock , chrDato , strlen(chrDato)); //envia el dato pedido por llave al cliente
                cout<<"dato enviado al cliente: "<<chrDato<<endl;
                //clear the message buffer
            }
            else{
                string msj="NoDataFound";
                char *chrMsj = &msj[0u]; //convierte el dato de string a char
                write(sock , chrMsj , strlen(chrMsj)); //envia el dato pedido por llave al cliente

            }
        }






        //getMemoryUsage
        if (operacion==operacion3){ 

                string strUsoDeMemoria=to_string(usoDeMemoria);
                cout<<"Dato enviado al cliente MemoryUsage: "<<strUsoDeMemoria<<endl;
                char *chrDato = &strUsoDeMemoria[0u]; //convierte el dato de string a char
                write(sock , chrDato , strlen(chrDato)); //envia el dato pedido por llave al cliente
                cout<<"Dato enviado al cliente MemoryUsage: "<<chrDato<<endl;
                memset(chrDato, 0, 1024); 

            }
           
        }


        //clear the message buffer
        memset(client_message, 0, 1024);
        

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