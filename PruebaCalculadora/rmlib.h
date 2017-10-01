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

List<string> list_1; //lista global de estructura de control en libreria

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
    string enviarDato(char* dato);
    //string getDato(char* llave);

    string savellaveEnListaLocal(string llaveLocal,string llaveDelServer);
    string getLlaveDelServerEnLocal(string llaveLocal);
    string getAllLlavesDelServerEnLocal(string);
    string getDato(string llave);

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


    string socketActuar(char*  dato);
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

string rmlib::enviarDato(char* dato){
    

    cout<<"Comunicando con el servidor Activo"<<endl;

    string llaveDelServer = socketActuar(dato);

    if (llaveDelServer!="0"){
        cout<<"Exito!, libreria y servidor Activo han intercambiado datos exitosamente"<<endl<<endl<<endl;
        return llaveDelServer;
    }

    else{
        cout<<"Servidor Activo desconectado"<<endl;
            
        cout<<"Intentando conectar al servidor Pasivo..."<<endl;
        portAvailable=portPasivo; //cambia el puerto disponible (pasivo a activo)            
        socketClient();
            
        cout<<"Comunicando con el servidor Pasivo"<<endl;
        string llaveDelServer = socketActuar(dato);
        if (llaveDelServer!="0"){                
            cout<<"Exito!, libreria y servidor Pasivo han intercambiado datos exitosamente"<<endl;
            return llaveDelServer;}

        else{
            cout<<"Ningun servidor esta activo"<<endl;
            return "0";
            }

                
    }
        
        

}
string rmlib::socketActuar(char* dato){

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
            return "0";
        }
        
        string llaveEnServer=buffer;

        cout<<"Llave en server: "<<buffer<<endl;
        cout << "Tamano del dato enviado: "<<bufsize<<endl;


        //guardar buffer(llave) en la estructura de control
        
        cout<<endl<<"__La conexion fue exitosa__"<<endl<<endl;


        
    //close(client);
    return llaveEnServer;
}

//para hacer peticion al server se manda: operacion a realizar, llave, valor, tamano.
//retorna el valor asociado a la llave
string rmlib::getDato(string llave){


    string operacion="getValor#";
    string datosize= "#null#null";
    string param=operacion+llave+datosize;
    cout<<"param: "<<param<<endl;
    char *chrParam = &param[0u]; //convierte string a char

    


    write(client , chrParam , strlen(chrParam));
    //Respuesta del server
    cout<<"Llaveeee: "<<chrParam<<endl;
    memset(buffer, 0, 1024);
    n=recv(client, buffer, bufsize, 0);
    cout << "buffer: "<< buffer<<endl;

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

//guarda la llave proveida por el servidor en la variable dato segun una llave local
string rmlib::savellaveEnListaLocal(string llaveLocal,string llaveDelServer){
    list_1.add_head(llaveLocal,llaveDelServer,"null");

}

//busca segun una llave local, la llave del server guardada en la lista local
string rmlib::getLlaveDelServerEnLocal(string llaveLocal){
    string dato = list_1.searchData(llaveLocal);
    if (dato!="0"){
        return dato;
    }
    return "0"; //no existe esa llave en local
}

//busca segun una llave local, la llave del server guardada en la lista local
string rmlib::getAllLlavesDelServerEnLocal(string llaveLocal){
    string llaves = list_1.searchallKeys(llaveLocal);
    if (llaves!="0"){
        return llaves;
    }
    return "0"; //no existe esa llave en local
}


/*
string rmlib::buscarIndicesDeLlave(string llaveLocal){

}*/

#endif //PRUEBACALCULADORA_RMLIB_H
