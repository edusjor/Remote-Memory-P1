//
// Created by edulubu on 28/08/17.
//Servidor Activo

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
int flagpasivosinc=false;  // si tiene almenos un dato guardado se convierte en true

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
    string clientKeysControl=""; //string que lleva el conteo de llaves creadas por este cliente.
    string separador="#";           //en el conteo de llaves separa cada una con este separador

    
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[1024];


    string val=""; //agrega aqui llave&valor@llave2&valor2@.... 
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 1024 , 0)) > 0 )
    {
        //end of string marker
        client_message[read_size] = '\0';
        string operacion="",llave="",valor="",size="" ;
        

        //itera el string del char recibido en el mensaje 
        //para separar la llave, el valor y el tamano
        //despues de eso se hace su respectiva accion segun operacion
        string strclient_message = string(client_message); //convierte el mensaje del cliente a string
        
        int cont=0;
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

        val=llave;

       
        string operacion0="pruebaConexion";
        string operacion1="guardarValor";
        string operacion2="getValor";
        string operacion3="getMemoryUsage";
        string operacion4="getAllMemoryValues";
        string operacion5="pruebaConexionDesdeActivo";
        string operacion6="sincActivoToPasivo";
        string operacion7="sincPasivoToActivo";


        
        ////////////////////////////////////////
        //pruebaConexion desde libreria
        //hace la prueba de conexion
        if(operacion==operacion0){
            string respuesta="nosincronizado";
            if (flagpasivosinc == true) 
                respuesta="sincronizado";
            
            char *chrResp = &respuesta[0u];                //convierte la llave de string a char
            write(sock , chrResp , 1024);  //envia la llave creada al cliente
            cout<<"prueba de conexion"<<endl;
        }







        ////////////////////////////////////////
        //guardarValor
        //Guarda en la lista una la aleatoria, el valor y el tamano proveidas en el buffer
        if(operacion==operacion1){ 

            srand(time(NULL));
            int nummRandom=rand();                      //crea un numero random
            string llave=to_string(nummRandom);         //llave aleatoria numerica creada en el server, se pasa a string
            cout<<"Dato recibido: "<<valor<<endl;
            cout << "Llave creada: "<<llave<<endl;
            clientKeysControl+=(llave+separador);         //concatena a la variable de control de llaves de este usuario la nueva llave creada

            list_1.add_head(llave,valor,size);          //guarda en la lista la llave, el valor y tamano del dato
            
            char *chrLlave = &llave[0u];                //convierte la llave de string a char
            write(sock , chrLlave , strlen(chrLlave));  //envia la llave creada al cliente
            usoDeMemoria+=(sizeof(valor));            //Le suma a una variable de control de memoria el tamano del valor guardado

            if (flagpasivosinc==false)
                flagpasivosinc=true;
        }








        ////////////////////////////////////////
        //getValor
        //Busca en la lista el dato guardado en la llave proveida por el cliente y lo envia
        if (operacion==operacion2){ 
            string datoRetornado=list_1.searchData(llave);          //busca el dato y lo asigna a una variable
            
            if (datoRetornado!="false"){
                char *chrDato = &datoRetornado[0u];                 //convierte el dato de string a char
                write(sock , chrDato , strlen(chrDato));            //envia el dato pedido por llave al cliente
                cout<<"dato enviado al cliente: "<<chrDato<<endl;
            }
            else{
                string msj="NoDataFound";
                char *chrMsj = &msj[0u];                            //convierte el dato de string a char
                write(sock , chrMsj , strlen(chrMsj));              //envia el memsaje de dato no encontrado al cliente
            }
        }




        ////////////////////////////////////////
        //getMemoryUsage
        //envia al cliente el uso de memoria actual, en bytes
        if (operacion==operacion3){ 

                string str_UsoDeMemoria=to_string(usoDeMemoria)+" bits";       //convierte a string la var de control de memoria
                char *chrDato = &str_UsoDeMemoria[0u];                          //convierte de string a char
                write(sock , chrDato , strlen(chrDato));                        //envia el dato de control de memoria al cliente
            }

       



        ////////////////////////////////////////
        //getAllMemoryValues
        //envia al cliente todo lo que hay en memoria actual
        if (operacion==operacion4){ 

                string str_Memory_values = list_1.retornarTodo();
                char *chrDato = &str_Memory_values[0u];                          //convierte de string a char
                write(sock , chrDato , strlen(chrDato));                        //envia el dato de control de memoria al cliente
        }







        ////////////////////////////////////////
        //pruebaConexionDesdeActivo
        //hace la prueba de conexion
        if(operacion==operacion5){

            //string respuesta="pruebaConexionDesdeActivoSuccess";
            
            string flag=to_string(flagpasivosinc);
            flagpasivosinc=1;
            string respuesta="flagpasivosinc="+flag; //flag puede ser 0 o 1
            char *chrResp = &respuesta[0u];                //convierte la llave de string a char
            write(sock , chrResp , 1024);  //envia la respuesta 
            cout<<"prueba de conexion"<<endl;
        }




        ////////////////////////////////////////
        //sincActivoToPasivo
        //sincroniza los datos recibidos del activo al pasivo
        //recibe todo en formato llave&valor@llave1&valor@....
        if(operacion==operacion6){

            if (flagpasivosinc==false)
                flagpasivosinc=true;

            string separador1="&";
            string separador2="@";

            //itera el string del valor recibido en el mensaje 
            //string llave="";
            valor=llave;

            string llav="";
            string val="";
            string siz=""; //esta var la hace con sizeof en local

            int cont=0;
            int tamano=valor.length();

            int flag = 0;
            while (cont<=tamano){        //separa al formato  llave&valor@llave2&valor@
                if (valor[cont]==separador1[0]){
                    flag=5;
                }

                if(valor[cont] == separador2[0]){
                    flag=2;
                }

                if (flag==0){
                    llav+=valor[cont];
                }

                if(flag==1)
                    val+=valor[cont];
                    
                if (flag ==5){
                    flag =1;
                }
                if (flag == 2){
                    size=to_string(sizeof(val));
                    list_1.add_head(llav,val,siz);

                    usoDeMemoria+=(sizeof(val));

                    flag = 0;
                    cout <<"Dato recibido del activo: "<<val<<endl;
                    llav="";
                    val="";
                    size="";
                }
         
                cont +=1;
            }
            valor="";
        }

        //////////////////////////////////////
        //El activo le pide al pasivo los datos guardados anteriormente
        //sincPasivoToActivo
        if (operacion==operacion7){


            string datosTodos = "";
            datosTodos = list_1.iterarTodo();//trae todos los datos de la lista   

            string formatDatos=datosTodos;

            char* chrDato = &formatDatos[0u];
            write(sock , chrDato , strlen(chrDato));
            if (formatDatos!="")
                cout<<"datos enviados al activo:\n"<<formatDatos<<endl;
        }


         

        //clear the message buffer
        memset(client_message, 0, 1024);   
    
    }//termina el while
    

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




