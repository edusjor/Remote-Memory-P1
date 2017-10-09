//
// Created by edulubu on 03/09/17.
//

#ifndef PRUEBACALCULADORA_CALCULADORA_H
#define PRUEBACALCULADORA_CALCULADORA_H

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>



#include <cmath>
#include <iostream>
//#include "rmlib.h"

using namespace std;

class Calculadora {

public:
    Calculadora();

private:
    void interfaz();                            //la "interfaz" que se muestra al usuario

    void suma();
    void resta();
    void multiplicacion();
    void division();

    void guiOperacion(string operacion);        //donde el usuario digita los numeros. Hace las operaciones y comunica con el server.
    void mostrarPrevOperaciones(string);        //muestra las operaciones anteriores de acuerdo a la operacion 
    string getValores(string );                 //recibe las llaves del servidor guardadas en local
    void MemoryUsage();                         //pide la cantidad de bytes utilizado en memoria 

};


#endif //PRUEBACALCULADORA_CALCULADORA_H
