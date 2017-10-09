//Monitor De Memoria.h

#ifndef MONITOR_DE_MEMORIA_H
#define MONITOR_DE_MEMORIA_H



#include <iostream>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>

using namespace std;

class Monitor_De_Memoria{
	public:
		Monitor_De_Memoria();
	private:
		int interfaz();
		void memoryUsage();
		void memoryValues();
};




#endif //MONITOR_DE_MEMORIA_H
