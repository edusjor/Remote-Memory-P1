//Monitor De Memoria.cpp


#include "rmlib.h"

#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

rmlib* rmlib1;
List<string>  list1;


Monitor_De_Memoria::Monitor_De_Memoria(){

	char* ip = "127.0.0.1";
    
    rmlib1 = new rmlib();

    rmlib1->rm_init(ip,8889,ip,8888);

    interfaz();
}

int Monitor_De_Memoria::interfaz(){
    cout<<"\n----------------------------------------"<<endl;
    cout << "1-Memory Usage"<<endl;
    cout << "2-Memory Values"<<endl;
    cout << "3-Salir"<<endl;


    int choice;
    cout <<"Que desea hacer?: "<<endl<<endl;
    cin>>choice;

    switch (choice){
        case 1:
        	memoryUsage();
            break;
        case 2:
            memoryValues();
            break;
        case 3:
            exit(true);
    }

    return 0;
}

void Monitor_De_Memoria::memoryUsage(){
	cout<<"Pidiendo Uso de memoria"<<endl;
	string getMemoryUsage = rmlib1->getAnythingFromServer("getMemoryUsage");
    cout<<"uso de memoria: "<<getMemoryUsage<<endl<<endl;
    interfaz();
	
}

void Monitor_De_Memoria::memoryValues(){
    string getMemoryValues = rmlib1->getAnythingFromServer("getAllMemoryValues");
    cout<<getMemoryValues<<endl;   
    interfaz(); 
}
