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

    rmlib1->rm_init(ip,1500,ip,8888);

    interfaz();
}

int Monitor_De_Memoria::interfaz(){
    cout << "1-Memory Usage"<<endl;
    cout << "2-Salir"<<endl;


    int choice;
    cout <<"Que desea hacer?: "<<endl<<endl;
    cin>>choice;

    switch (choice){
        case 1:
        	memoryUsage();
            break;
        case 2:
            exit(true);
    }

    return 0;
}

void Monitor_De_Memoria::memoryUsage(){
	cout<<"lol"<<endl;
	while (1){ 
	    string getMemoryUsage = rmlib1->getAnythingFromServer("getMemoryUsage");
	    cout<<"uso de memoria: "<<getMemoryUsage<<endl;
	}
}
