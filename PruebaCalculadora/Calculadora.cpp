//
// Created by edulubu on 03/09/17.
//


#include "rmlib.h"
#include "Calculadora.h"

#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

rmlib* rmlib1;
List<string>  list1;

int Calculadora::guiOperacion(string operacion){
    list1.add_head("1","2","3");
    int num1;
    int num2;
    cout <<"Digite el primer numero: "<<endl<<endl;
    cin>>num1;
    cout <<"Digite el segundo numero: "<<endl<<endl;
    cin>>num2;
    if (operacion == "+")
        int sum=num1+num2;
        cout <<"La suma es: "<< num1+num2 <<endl<<endl;

        string oper="guardarValor";
        string llave= "null";
        string valor=to_string(num1+num2);
        string size=to_string(sizeof(valor));

        cout<<"por aqui pasa 1"<<endl;

        string parametros = oper+"#"+llave+"#"+valor+"#"+size;
        char *chrParametros = &parametros[0u]; //convierte string a char

        string llaveDelServer=rmlib1->enviarDato(chrParametros);
        cout<<"por aqui pasa 1.5"<<endl;
        if (llaveDelServer=="0"){
            cout<<"error, servidores desconectados"<<endl;
        }
        else{cout<<"por aqui pasa 2"<<endl;
            rmlib1->savellaveEnListaLocal("sumas",llaveDelServer); //guarda la llave del server con una llave local
            cout<<"por aqui pasa 3"<<endl;
        }

        //rmlib1->savellaveEnListaLocal("sumas",llaveDelServer);
        cout<<"\nllave sumas: "<<rmlib1->getAllLlavesDelServerEnLocal("sumas")<<endl<<endl;
        
        

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

//muestra todas las sumas anteriores
void Calculadora::mostrarPrevSumas(){
 
    string keysDeSumas="";  //guarda todas las keys del servidor guardadas en local en la llave "sumas"
  
    keysDeSumas = rmlib1->getAllLlavesDelServerEnLocal("sumas"); //string con todas las llaves de la llave "sumas" //LOCAL
    cout<<"\nllavesServer en llaveLocal sumas: "<<keysDeSumas<<endl<<endl;
    
    string sumasPreviasDelServer = getValores(keysDeSumas);//llama la funcionLocal para que traiga los valores de las llavesServer del server en un solo string        

    cout<<"Todas los resultados de sumas anteriores: "<<sumasPreviasDelServer<<endl<<endl;
    cout<<"--------------------------------------------------------------------------- "<<endl;

    interfaz();

}

//recibe las llaves del servidor guardadas en local en string con separador: "#"
//retorna los datos de las llaves en servidor, en string con mismo separador
string Calculadora::getValores(string keysDeOperacion){
    string datosDelServer="";
    string separador="#";
    string key="";

    for(int i=0; i<keysDeOperacion.length(); i++){
            

        if ((keysDeOperacion[i])==separador[0]){
               
            char *chrKey = &key[0u]; //convierte string a char la llave q se mandara al server

            datosDelServer+="=>"+rmlib1->getDato(chrKey);//envia la llave a la funcion en rmlib para que retorne el valor guardado en server
            key="";
        }
        else{
            key=key+keysDeOperacion[i];
        }
    }
    return datosDelServer;
}

void Calculadora::MemoryUsage(){
    string getMemoryUsage = rmlib1->getAnythingFromServer("getMemoryUsage");
    cout<<"uso de memoria: "<<getMemoryUsage<<endl;

}

//trae todos los valores guardados en memoria con sus llaves
void Calculadora::getAllMemoryValue(){
    
    string AllMemoryValue = rmlib1->getAnythingFromServer("getAllMemoryValue");
    cout<<"todos los datos y llaves en memoria: "<<AllMemoryValue<<endl<<endl;

}



int Calculadora::interfaz(){
    cout << "1-Suma"<<endl;
    cout << "2-Restar"<<endl;
    cout << "3-Multiplicar"<<endl;
    cout << "4-Dividir"<<endl;
    cout << "5-Mostrar Previas Sumas"<<endl;
    cout << "6-Salir"<<endl;
    cout << "7-Memory Usage"<<endl<<endl;


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
        case 7:
            MemoryUsage();

    }

    return 0;
}

Calculadora::Calculadora()  {
    //iniciar socket
    char* ip = "127.0.0.1";
    char* ipPas = "127.0.0.2";

    int port=8888;
    int portPas=1500;

    rmlib1 = new rmlib();

    rmlib1->rm_init(ipPas,portPas,ip,port);





    interfaz();

}