//
// Created by edulubu on 04/09/17.
//

#ifndef PRUEBACALCULADORA_RMLIB_H
#define PRUEBACALCULADORA_RMLIB_H

#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

//estructura de control
#include "list.h"
#include "list.cpp"

//list<string> listaContr; // estructura de control, lista global

using  namespace std;
////////////////////////////////////////
//estrucura de control
class rmRef_H{

};


/////////////////////////////////
//rmlib
class rmlib{
public:

    int socketClient();
    void rm_init(char* ip, int port, char* ipHA, int portHA);
    int enviarDato(char* dato);
    string getDato(char* llave);
private:
    char* ipActivo;
    char* ipPasivo;
    int portActivo;
    int portPasivo;

    int port;

    int portAvailable;
    int n;

    int client;

    int bufsize = 2000;
    char buffer[2000];


    int socketActuar(char*  dato);
};



void rmlib::rm_init(char* ip, int port, char* ipHA, int portHA){
    this->ipActivo = ipHA;
    this->ipPasivo = ip;
    this->portActivo = portHA;
    this->portPasivo = port;
    this->portAvailable=portActivo;

    //iniciarSocket();
    socketClient();

}

/*void rmlib::rm_new (char* key, void* value, int value_size){

}

rmRef_H rmlib::rm_get(char* key){

}

void rmlib::rm_delete(rmRef_H* handler){

}
*/


int rmlib::socketClient() {

    int portNum=this->portAvailable;

    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    if (client < 0) {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }


    cout << "\n=> Socket client has been created..." << endl;


    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);



    /* ---------- CONNECTING THE SOCKET ---------- */
    /* ---------------- connect() ---------------- */

    if (connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr)) == 0){
        cout << "=> Connection to the server port number: " << portNum << endl;
    }else{
        cout <<"servidor no conectado"<<endl;
    }


    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    
    cout << "=> Connection confirmed, you are good to go..." << endl;

}

int rmlib::enviarDato(char* dato){
    

    cout<<"Comunicando con el servidor Activo"<<endl;
    if (socketActuar(dato)==1){
        cout<<"Exito!, libreria y servidor Activo han intercambiado datos exitosamente"<<endl<<endl<<endl;
        return 1;
    }

    else{
        cout<<"Servidor Activo desconectado"<<endl;
            
        cout<<"Intentando conectar al servidor Pasivo..."<<endl;
        portAvailable=portPasivo; //cambia el puerto disponible (pasivo a activo)            
        socketClient();
            
        cout<<"Comunicando con el servidor Pasivo"<<endl;
        if (socketActuar(dato)==1){                
            cout<<"Exito!, libreria y servidor Pasivo han intercambiado datos exitosamente"<<endl;
            return 1;}

        else{
            cout<<"Ningun servidor esta activo"<<endl;
            return 0;
            }

                
    }
        
        

}
int rmlib::socketActuar(char* dato){

        cout << "Client: ";
        //cin >> buffer;

        //send(client, dato, bufsize, 0);

        write(client , dato , strlen(dato));
        cout << "Tamano del dato antes de enviar: "<<strlen(dato)<<endl<<endl;
        
        //Respuesta del server
        n=recv(client, buffer, bufsize, 0);

        if (n<=0){
            cerr <<"servidor desconectado"<<endl;
            cout << "servidor no conectado"<<endl;
            return 0;
        }
        
        string llaveEnServer=buffer;

        cout<<"Llave en server: "<<buffer<<endl;
        cout << "Tamano del dato enviado: "<<bufsize<<endl;


        //guardar buffer(llave) en la estructura de control
        
        cout<<endl<<"__La conexion fue exitosa__"<<endl<<endl;


        
    //close(client);
    return 1;
}

string rmlib::getDato(char* llave){
    write(client , llave , strlen(llave));
    //Respuesta del server
    n=recv(client, buffer, bufsize, 0);

    if (n<=0){
        cerr <<"servidor desconectado"<<endl;
        cout << "servidor no conectado"<<endl;
        return "NoServerFound";
    }

    string valorEnServer=buffer;
    if(valorEnServer=="NoDataFound")
        return "NoDataFound";

    return valorEnServer;
}


#endif //PRUEBACALCULADORA_RMLIB_H
