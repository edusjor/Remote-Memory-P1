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

    if (connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << portNum << endl;


    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, you are good to go..." << endl;

}

int rmlib::enviarDato(char* dato){
    cout<<"enviando dato"<<endl;

    if (socketActuar(dato)==false){
        cout<<"Conectandose al servidor Pasivo"<<endl;
        portAvailable=portPasivo;
        cout<<"Conectandose al servidor Pasivo"<<endl;
        socketClient();
        cout<<"Conectandodo al Pasivo"<<endl;
        if (socketActuar(dato)==false)
            cout<<"Ningun servidor esta activo"<<endl;

    } else
    cout<<"Conectandose al servidor Activo"<<endl;
        socketActuar(dato);

}
int rmlib::socketActuar(char* dato){

    //do {
        cout << "Client: ";
        //cin >> buffer;

        //send(client, dato, bufsize, 0);

        write(client , dato , strlen(dato));
        n=recv(client, buffer, bufsize, 0);
        if (n<=0){
            cerr <<"servidor desconectado"<<endl;
            cout << "servidor no conectado"<<endl;


            return 0;}

        cout << "Cl buffer: "<<buffer<<endl;
        cout << "Cl bufsize: "<<bufsize<<endl;
        
    //}while(true);




    /* ---------------- CLOSE CALL ------------- */
    /* ----------------- close() --------------- */

    /*
        Once the server presses # to end the connection,
        the loop will break and it will close the server
        socket connection and the client connection.
    */

    cout << "\n=> Connection terminated.\nGoodbye...\n";

    close(client);
    return 0;
}


#endif //PRUEBACALCULADORA_RMLIB_H
