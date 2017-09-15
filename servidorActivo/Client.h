//
// Created by edulubu on 02/09/17.
//

#ifndef SERVIDORACTIVO_CLIENT_H
#define SERVIDORACTIVO_CLIENT_H




#include <iostream>

#include <cstdio>
#include <cstdlib>

#define MAX_NAME_LENGHT 20

using namespace std;

class Client {
public:
    char *name;
    int id;

    //Socket stuff
    int sock;

public:
    Client();
    void SetName(const char *name);
    void SetId(int id);
};

#endif //SERVIDORACTIVO_CLIENT_H
