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
    int suma();
    int resta();
    int multiplicacion();
    int division();

    void mostrarPrevSumas();

    int guiOperacion(string);
    int interfaz();


};


#endif //PRUEBACALCULADORA_CALCULADORA_H
