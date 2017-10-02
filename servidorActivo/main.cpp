

#include <iostream>

#include "Server.h"


using namespace std;

int main() {
    cout << "Running!" << endl;

    Server *s;
    s = new Server();

    //Main loop
    s->aceptarEimprimir();


    return 0;
}

