//
// Created by edulubu on 03/09/17.
//

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include "rmlib.h"



#include <iostream>
#include <unistd.h>
#include "Calculadora.h"

using namespace std;

rmlib* rmlib1;


int Calculadora::guiOperacion(string operacion){
    int num1;
    int num2;
    cout <<"Digite el primer numero: "<<endl<<endl;
    cin>>num1;
    cout <<"Digite el segundo numero: "<<endl<<endl;
    cin>>num2;
    if (operacion == "+")
        int sum=num1+num2;
        cout <<"La suma es: "<< num1+num2 <<endl<<endl;

        string llave= "Suma total";
        string valor=to_string(num1+num2);
        string size=to_string(sizeof(num1+num2));


        string parametros = llave+","+valor+","+size;
        char *chrParametros = &parametros[0u]; //convierte string a char

        rmlib1->enviarDato(chrParametros);
        interfaz();


    if (operacion == "-")
        cout <<"La resta es: "<< num1-num2<<endl<<endl;
    if (operacion == "*")
        cout <<"El producto es: "<< num1*num2<<endl<<endl;
    if (operacion == "/")
        cout <<"La division es: "<< num1/num2<<endl<<endl;
    interfaz();

}

int Calculadora::suma(){
    guiOperacion("+");
    return 0;
}
int Calculadora::resta(){
    guiOperacion("-");
    return 0;
}
int Calculadora::multiplicacion(){
    guiOperacion("*");
    return 0;
}
int Calculadora::division() {
    guiOperacion("/");
    return 0;
}
void Calculadora::mostrarPrevSumas(){
    cout<<"Estas son todas las operaciones de suma anteriores: "<<endl;
    //traer todas las anteriores sumas
    interfaz();

}
int Calculadora::interfaz(){
    cout << "1-Suma"<<endl;
    cout << "2-Restar"<<endl;
    cout << "3-Multiplicar"<<endl;
    cout << "4-Dividir"<<endl;
    cout << "5-Mostrar Previas Sumas"<<endl;
    cout << "6-Salir"<<endl<<endl;


    int choice;
    cout <<"Que desea hacer?: "<<endl<<endl;
    cin>>choice;

    switch (choice){
        case 1:
        suma();
            break;
        case 2:
            resta();
            break;
        case 3:
            multiplicacion();
            break;
        case 4:
            division();
            break;
        case 5:
            mostrarPrevSumas();
            break;
        case 6:
            exit(true);

    }

    return 0;
}

Calculadora::Calculadora()  {
    //iniciar socket
    char* ip = "127.0.0.1";
//rmlib* rmlib1;
    
    rmlib1 = new rmlib();

    rmlib1->rm_init(ip,1500,ip,8888);





    interfaz();

}