//
// Created by edulubu on 28/08/17.
//

#include "Server.h"


#include <iostream>


#include "list.h"
#include "list.cpp"
#include "Client.h"
#include "Client.cpp"

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

    string numero = "1234";
    int num =atoi(numero.c_str()); // convierte string a entero
    cout<<"numero: "<<num+2<<endl;

    //list_1.add_head(321123);
    //list_1.print();

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
    cout << "Escuchado"<<endl;
    //accept(client, (struct sockaddr *) &server_addr, &size);
    //se mantiene escuchando aqui hasta que se conecte algun cliente
}


Client *c;
void Server::aceptarEimprimir() {


    while (1) {
        
        c = new Client();
       cout << "aqui 1"<<endl;
        c->sock = accept(client, (struct sockaddr *) &server_addr, &size);//acepta al cliente que hace la peticion
       cout << "aqui 2"<<endl;
        // first check if it is valid or not
        if (c->sock < 0) {
            cout << "=> Error on accepting..." << endl;
        } else {
            //crea el nuevo thread que manejara al nuevo cliente
            pthread_t thread_id;
            pthread_create( &thread_id , NULL ,  playSocket , (void*) &c->sock );         
        }
    }
}

//entra aqui por cada nueva conexion
void* Server::playSocket(void* socket_desc){
//Get the socket descriptor
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
        cout <<endl<<endl<< "Mensaje del cliente: "<<client_message<<endl;
        
        

        //itera el string del char recibido en el mensaje 
        //para separar la llave, el valor y el tamano
        string strclient_message=string(client_message);
        string operacion;
        string llave;
        string valor;
        string size;

        int cont=0;
        string separador="#";
        cout<<"leenn: "<<strclient_message.length()<<endl;
        for(int i=0; i<strclient_message.length(); i++){
            
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
        memset(client_message, 0, 1024);
        
        }

        string operacion1="guardarValor";
        string operacion2="getValor";
        string operacion3="getMemoryUsage";
        string operacion4="getAllMemoryValue";

        //guardarValor
        if(operacion==operacion1){ 

            int nummRandom=rand();
            string llave=to_string(nummRandom); //llave aleatoria creada en el server

            list_1.add_head(llave,valor,size);   //guarda la llave, el valor y tamano del dato
            //list_1.print(); 
            //list_1.searchIndex(llave);
            
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