//
// Created by edulubu on 28/08/17.
//

#ifndef ARRAY_SERVER_H
#define ARRAY_SERVER_H



#include <iostream>


using namespace std;



class Array{
    public:
        //array();

        string arr[3];
        void crearArray(string llave, string valor, string size){

            this->arr[0]=llave;
            this->arr[1]=valor;
            this->arr[2]=size;
        }

        string *returnArray(){
/*
            string array[3];
            for (int i=0; i<3; i++){
                array[i] = arr[i];
            }
*/
            string *parray=arr;

            return parray;
        }

        void printArray(){
            string array[3]=*returnArray();
            for (int i=0; i<3; i++){
                //cout<<array[i]<<", ";
            }
        }
        
    
};


#endif //ARRAY_SERVER_H
