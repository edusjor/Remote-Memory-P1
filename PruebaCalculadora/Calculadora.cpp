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

Calculadora::Calculadora()  {
    //iniciar socket
    char* ipAct = "127.0.0.1";
    char* ipPas = "127.0.0.1";

    int portAct=8888;
    int portPas=8889;

    rmlib1 = new rmlib();
    
    rmlib1->rm_init(ipPas,portPas,ipAct,portAct);

    interfaz();
}


void Calculadora::interfaz(){
    cout <<endl<<endl<< "1-Suma"<<endl;
    cout << "2-Restar"<<endl;
    cout << "3-Multiplicar"<<endl;
    cout << "4-Dividir"<<endl;
    cout << "5-Mostrar Previas Sumas"<<endl;
    cout << "6-Mostrar Previas Restas"<<endl;
    cout << "7-Mostrar Previas Multiplicaciones"<<endl;
    cout << "8-Mostrar Previas Divisiones"<<endl;
    cout << "9-Memory Usage"<<endl;
    cout << "0-Salir"<<endl<<endl<<endl;


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
            mostrarPrevOperaciones("sumas");
            break;
        case 6:
            mostrarPrevOperaciones("restas");
            break;
        case 7:
            mostrarPrevOperaciones("multiplicaciones");
            break;
        case 8:
            mostrarPrevOperaciones("divisiones");
            break;
        case 9:
            MemoryUsage();
            break;

        case 0:
            //Borrar datos de las llaves (implementar)
            exit(true);
    }
}



void Calculadora::suma(){
    guiOperacion("sumas");
}
void Calculadora::resta(){
    guiOperacion("restas");
}
void Calculadora::multiplicacion(){
    guiOperacion("multiplicaciones");
}
void Calculadora::division() {
    guiOperacion("divisiones");   
}




void Calculadora::guiOperacion(string operacion){ // muestra los cin para la entrada del usuario. hace la operacion y el envio al server con la libreria
    int num1;
    int num2;
    cout <<"Digite el primer numero: "<<endl<<endl;
    cin>>num1;
    cout <<"Digite el segundo numero: "<<endl<<endl;
    cin>>num2;

    
    string oper="guardarValor";        
    string llave= "null";
    string valor="";

    if (operacion == "sumas"){ 
        valor=to_string(num1+num2);
        cout <<"La suma es: "<< valor <<endl<<endl;
    }
    if (operacion == "restas"){ 
        valor=to_string(num1-num2);
        cout <<"La resta es: "<< valor <<endl<<endl;
    }
    if (operacion == "multiplicaciones"){ 
        valor=to_string(num1*num2);
        cout <<"La multiplicacion es: "<< valor <<endl<<endl;
    }
    if (operacion == "divisiones"){ 
        valor=to_string(num1/num2);
        cout <<"La division es: "<< valor <<endl<<endl;
    }

    
    string size=to_string(sizeof(valor)); //manda el tamano en bits


    string parametros = oper+"#"+llave+"#"+valor+"#"+size;
    char *chrParametros = &parametros[0u]; //convierte string a char

    string llaveDelServer=rmlib1->enviarDato(chrParametros); //envia el dato en formato "operacion#null#valor#tamano" y resive un string con la llave que crea el server

    if (llaveDelServer=="0"){
        cout<<"error, servidores desconectados"<<endl;
        //reconectar
    }
    else{
        rmlib1->savellaveEnListaLocal(operacion,llaveDelServer); //guarda la llave del server con una llave local
    }

    cout<<"\nllaves de operacion " <<operacion<<": "<<rmlib1->getAllLlavesDelServerEnLocal(operacion)<<endl<<endl;//trae todas las llaves guardadas en local del server.
    
    interfaz();

}



//muestra las operaciones anteriores asociadas a la llave en oper
void Calculadora::mostrarPrevOperaciones(string localKey){

    string keysDeOper = rmlib1->getAllLlavesDelServerEnLocal(localKey); //string con todas las llaves de la llave en el parametro oper //LOCAL
    //string operacionesPreviasDelServer = getValores(keysDeOper);//llama la funcionLocal para que traiga los valores de las llavesServer del server en un solo string        
    string resultados = getValores(keysDeOper);//llama la funcionLocal para que traiga los valores de las llavesServer del server en un solo string        

    if (resultados =="noServerFound"){
        cout<<"servidores no available"<<endl;
        interfaz();
    }    
    cout<<"Todas los resultados de "<<localKey<<" anteriores: "<<resultados<<endl;
    
    cout<<"--------------------------------------------------------------------------- "<<endl<<endl;

    interfaz();
}


//recibe las llaves del servidor guardadas en local en string con separador: "#"
//retorna los datos de las llaves en servidor, en string con mismo separador
string Calculadora::getValores(string keysDeOperacion){
    string datosDelServer="\n";
    string separador="#";
    string key="";

    for(int i=0; i<keysDeOperacion.length(); i++){

        if ((keysDeOperacion[i])==separador[0]){
               
            char *chrKey = &key[0u]; //convierte string a char la llave q se mandara al server


            string dataGot =rmlib1->rm_get(chrKey);
            if (dataGot=="noServerFound")
                return "noServerFound";
            datosDelServer+="=>"+dataGot+"\n";//envia la llave a la funcion en rmlib para que retorne el valor guardado en server
            key="";
        }
        else{
            key=key+keysDeOperacion[i];
        }
    }
    return datosDelServer;
}



//pide la cantidad de bytes utilizado en memoria 
void Calculadora::MemoryUsage(){   
    string getMemoryUsage = rmlib1->getAnythingFromServer("getMemoryUsage");
    cout<<endl<<"Uso de memoria: "<<getMemoryUsage<<endl<<endl<<endl;
    interfaz();
}





