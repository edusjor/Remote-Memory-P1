//
// Created by edulubu on 03/09/17.
//

#ifndef PRUEBACALCULADORA_CALCULADORA_H
#define PRUEBACALCULADORA_CALCULADORA_H


#include <cmath>
#include <iostream>

using namespace std;

class Calculadora {

public:
    Calculadora();

private:
    int suma();
    int resta();
    int multiplicacion();
    int division();

    int guiOperacion(string);
    int interfaz();


};


#endif //PRUEBACALCULADORA_CALCULADORA_H
