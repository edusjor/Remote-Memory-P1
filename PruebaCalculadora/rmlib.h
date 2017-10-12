//
// Created by edulubu on 04/09/17.
//

#ifndef PRUEBACALCULADORA_RMLIB_H
#define PRUEBACALCULADORA_RMLIB_H

#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>


using namespace std; 

///////////////////////////////////////////////////////////////
///////////////////clase nodo///////////////////////////
template <class T>
class Node
{
    public:
        Node();
        Node(T,T,T);
        ~Node();

        Node *next;

        T llave;
        T valor;
        T size;

        void delete_all();
        void print();
};

///////////////////////////////////////////////////////////////
///////////////////clase lista///////////////////////////

template <class T>

class List
{
    public:
        List();
        ~List();

        void add_head(T,T,T); //Agrega a la lista
        void add_end(T);
        void add_sort(T);
        void concat(List);
        void del_all();
        void del_by_data(T);
        void del_by_position(int);
        void fill_by_user(int);
        void fill_random(int);
        void intersection(List);
        void invert();
        void load_file(string);
        void print();
        void save_file(string);
        string searchIndexes(T);  //busca llaves iguales a un parametro y retorna los indices
        string searchallKeys(T data_); //busca llaves iguales a un parametro y retorna todos los datos asociados
        string searchData(T); //busca un dato por llave para retornarlo junto con su tamano
        void sort();

    private:
        Node<T> *m_head;
        int m_num_nodes;
};
























//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////Libreria/////////////////////
List<string> list_1; //lista global de estructura de control en libreria



//rmlib
class rmlib{
public:
    
    void rm_init(char* ip, int port, char* ipHA, int portHA);   //verifica si activo o pasivo estan disponibles llamando a la funcion socketClient
    int socketClient(int,string);                               // inicia el socket si el server esta disponible y envia uno si hay exito, 0 si no.
    string enviarDato(char* dato);                              //Envia dato al servidor

    void savellaveEnListaLocal(string llaveLocal,string llaveDelServer);                  //guarda la llave restornada desde el server en la lista de control local
    //string getLlaveDelServerEnLocal(string llaveLocal);         //(no me acuerdo)     
    string getAllLlavesDelServerEnLocal(string llaveLocal);                //trae todas las llaves  guardadas en local con la llave de control
    string rm_get(string llave);                                //retorna el valor en el server asociado a la llave guardada en la llave local de control
    string getAnythingFromServer(string request);               //hace "cualquier" peticion al server  ej: traer los bits usados, monitor de memoria

private:
    char* ipActivo;
    char* ipPasivo;
    int portActivo;
    int portPasivo;

    int n;                                                      //el booleano cuando se crea la conexion al server
    int client;                                                 //el numero de cliente al crear conexion al server
    int bufsize = 1024;
    char buffer[1024];

    string rm_new(char*  dato);                                 //envia un nuevo dato al server y recibe la llave del dato guardado
};



void rmlib::rm_init(char* ip, int port, char* ipHA, int portHA){  //pasivo,pasivo,activo,activo
    this->ipActivo = ipHA;
    this->ipPasivo = ip;
    this->portActivo = portHA;
    this->portPasivo = port;
  
    //inicia Socket
    cout<<"Conectando al servidor Activo"<<endl;
    if (socketClient(portActivo,ipActivo)==0){                  //si da 0 entonces no esta disponible
        cout<<"Servidor activo desconectado"<<endl;
        cout<<"Conectando al servidor Pasivo"<<endl;
        if (socketClient(portPasivo,ipPasivo)==0){
            cout<<"Error, ningun servidor disponible"<<endl;
            exit(1);
        }
    }

}


//crea el socket client segun el puerto dado y retorna 0 o 1 segun este o no disponible el server
int rmlib::socketClient(int puerto,string ip) {

    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

    // ---------- ESTABLISHING SOCKET CONNECTION ----------//
    // --------------- socket() function ------------------//

    if (client < 0) {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }
    cout << "\n=> Socket client has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(puerto);

    if (connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr))<0){ //si no se conecta al server del puerto actual retorna 0
        return 0;
    }
    else{
        return 1;
    }
}





//verifica que halla algun servidor disponible y Envia dato al servidor
string rmlib::enviarDato(char* dato){

    if(socketClient(portActivo,ipActivo)==0){ //si el activo no esta disponible se  conecta al pasivo
        if (socketClient(portPasivo,ipPasivo)==0){
            cout<<"Ningun servidor esta disponible"<<endl;   
        }else
            cout<<"Activo no disponible, conectado al servidor Pasivo"<<endl;
    }else
        cout<<"Conectado al servidor Activo"<<endl;
    
    string llaveDelServer = rm_new(dato); 

    if (llaveDelServer!="0"){
        cout<<"Exito!, libreria y servidor han intercambiado datos exitosamente"<<endl<<endl<<endl;
        return llaveDelServer;

    }else
        cout<<"Hubo un error no hay conexion "<<endl;           
}

//prueba la conexion al sever, envia los datos y recibe la llave creada por el server para esos datos y la retorna
string rmlib::rm_new(char* dato){

        send(client,"pruebaConexion#null#null#null",1024,0); //envia datos para hacer prueba de conexion en formato:   "operacion a realizar#null#null#null"

        n = recv(client, buffer, bufsize, 0); //respuesta de la prueba 
        if (n<=0){   
            cout << "servidor no conectado"<<endl<<endl<<endl<<endl<<endl;
            return "0";
        }
              

        memset(buffer, 0, 1024);

        write(client , dato , strlen(dato));
        recv(client, buffer, bufsize, 0);
        
        string llaveEnServer=buffer;
        
        cout<<endl<<"_______conection succesful_______"<<endl<<endl;

    return llaveEnServer;
}


//para hacer peticion al server se manda: operacion a realizar, llave, valor, tamano.
//retorna el valor en el server asociado a la llave
string rmlib::rm_get(string llave){

    if (socketClient(portActivo,ipActivo)==0 ){ 
            if (socketClient(portPasivo,ipPasivo)==0){
            cout<<"Ningun servidor esta disponible"<<endl;
            return "NoServerFound";
            }   
            else
                cout<<"Activo no disponible, conectado al servidor Pasivo"<<endl;
        }//else
        //cout<<"Conectado al servidor Activo"<<endl;


    string operacion="getValor#";
    string datosize= "#null#null";
    string param=operacion+llave+datosize;      //getValor#llave#null#null

    char *chrParam = &param[0u]; //convierte string a char

    write(client , chrParam , strlen(chrParam));

    //Respuesta del server
    memset(buffer, 0, 1024);
    n=recv(client, buffer, bufsize, 0);

    if (n<=0){
        cout << "servidor no conectado"<<endl;
        return "NoServerFound";
    }

    if(buffer=="NoDataFound")
        return "NoDataFound";

    return buffer;   
}



//pide lo que sea al server y este responde con lo pedido
//
string rmlib::getAnythingFromServer(string request){
    
    char *chrParam = &request[0u]; //convierte string a char

    write(client , chrParam , strlen(chrParam));
    memset(buffer, 0, 1024);
    n=recv(client, buffer, bufsize, 0);

    string requestRespuesta=buffer;
    return buffer;

}



//guarda la llave proveida por el servidor en la variable dato segun una llave local
void rmlib::savellaveEnListaLocal(string llaveLocal,string llaveDelServer){
    list_1.add_head(llaveLocal,llaveDelServer,"null");
}


//busca segun una llave local, las llaves del server guardadas en la lista local
string rmlib::getAllLlavesDelServerEnLocal(string llaveLocal){
    string llaves = list_1.searchallKeys(llaveLocal);
    if (llaves!="0"){
        return llaves;
    }
    return "0"; //no existe esa llave en local
}










































//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// nodo
//funciones
template<typename T>

Node<T>::Node()
{
    llave = NULL;
    valor = NULL;
    size = NULL;
    next = NULL;
}

// Constructor por parámetro
template<typename T>
Node<T>::Node(T llave, T valor, T size)
{
    this->llave= llave;
    this->valor= valor;
    this->size=size;

    //data = data_;
    next = NULL;
}

// Eliminar todos los Nodos
template<typename T>
void Node<T>::delete_all()
{
    if (next)
        next->delete_all();
    delete this;
}

// Imprimir un Nodo
template<typename T>
void Node<T>::print()
{
    //cout << "Node-> " << "Dato: " << dato << " Direcion: " << this << " Siguiente: " << next << endl;
    cout << llave << "-> ";
}

template<typename T>
Node<T>::~Node() {}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//lista 
//funciones
// Constructor por defecto
template<typename T>
List<T>::List()
{
    m_num_nodes = 0;
    m_head = NULL;
}

// Insertar al inicio
template<typename T>
void List<T>::add_head(T llave,T valor, T size)
{
    Node<T> *new_node = new Node<T> (llave,valor,size);
    Node<T> *temp = m_head;

    if (!m_head) {
        m_head = new_node;
    } else {
        new_node->next = m_head;
        m_head = new_node;

        while (temp) {
            temp = temp->next;
        }
    }
    m_num_nodes++;
}

// Insertar al final
template<typename T>
void List<T>::add_end(T data_)
{
    Node<T> *new_node = new Node<T> (data_);
    Node<T> *temp = m_head;

    if (!m_head) {
        m_head = new_node;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
    m_num_nodes++;
}

// Insertar de manera ordenada
template<typename T>
void List<T>::add_sort(T data_)
{
    Node<T> *new_node = new Node<T> (data_);
    Node<T> *temp = m_head;

    if (!m_head) {
        m_head = new_node;
    } else {
        if (m_head->data > data_) {
            new_node->next = m_head;
            m_head = new_node;
        } else {
            while ((temp->next != NULL) && (temp->next->data < data_)) {
                temp = temp->next;
            }
            new_node->next = temp->next;
            temp->next = new_node;
        }
    }
    m_num_nodes++;
}

// Concatenar a otra List
template<typename T>
void List<T>::concat(List list)
{
    Node<T> *temp2 = list.m_head;

    while (temp2) {
        add_end(temp2->data);
        temp2 = temp2->next;
    }
}

// Eliminar todos los nodos
template<typename T>
void List<T>::del_all()
{
    m_head->delete_all();
    m_head = 0;
}

// Eliminar por data del nodo
template<typename T>
void List<T>::del_by_data(T data_)
{
    Node<T> *temp = m_head;
    Node<T> *temp1 = m_head->next;

    int cont = 0;

    if (m_head->data == data_) {
        m_head = temp->next;
    } else {
        while (temp1) {
            if (temp1->data == data_) {
                Node<T> *aux_node = temp1;
                temp->next = temp1->next;
                delete aux_node;
                cont++;
                m_num_nodes--;
            }
            temp = temp->next;
            temp1 = temp1->next;
        }
    }

    if (cont == 0) {
        cout << "No existe el dato " << endl;
    }
}

// Eliminar por posición del nodo
template<typename T>
void List<T>::del_by_position(int pos)
{
    Node<T> *temp = m_head;
    Node<T> *temp1 = temp->next;

    if (pos < 1 || pos > m_num_nodes) {
        cout << "Fuera de rango " << endl;
    } else if (pos == 1) {
        m_head = temp->next;
    } else {
        for (int i = 2; i <= pos; i++) {
            if (i == pos) {
                Node<T> *aux_node = temp1;
                temp->next = temp1->next;
                delete aux_node;
                m_num_nodes--;
            }
            temp = temp->next;
            temp1 = temp1->next;
        }
    }
}

// Llenar la Lista por teclado
template<typename T>
void List<T>::fill_by_user(int dim)
{
    T ele;
    for (int i = 0; i < dim; i++) {
        cout << "Ingresa el elemento " << i + 1 << endl;
        cin >> ele;
        add_end(ele);
    }
}

// Llenar la Lista aleatoriamente para enteros
template<typename T>
void List<T>::fill_random(int dim)
{
    srand(time(NULL));
    for (int i = 0; i < dim; i++) {
        add_end(rand() % 100);
    }
}

// Usado por el método intersección
template<typename T>
void insert_sort(T a[], int size)
{
    T temp;
    for (int i = 0; i < size; i++) {
        for (int j = i-1; j>= 0 && a[j+1] < a[j]; j--) {
            temp = a[j+1];
            a[j+1] = a[j];
            a[j] = temp;
        }
    }
}

// Números que coinciden en 2 Lists
template<typename T>
void List<T>::intersection(List list_2)
{
    Node<T> *temp = m_head;
    Node<T> *temp2 = list_2.m_head;

    // Creo otra Lista
    List intersection_list;

    int num_nodes_2 = list_2.m_num_nodes;
    int num_inter = 0;

    // Creo 2 vectores dinámicos
    T *v1 = new T[m_num_nodes];
    T *v2 = new T[num_nodes_2];

    // Lleno los vectores v1 y v2 con los datas de la lista original y segunda lista respectivamente
    int i = 0;

    while (temp) {
        v1[i] = temp->data;
        temp = temp->next;
        i++;
    }

    int j = 0;

    while (temp2) {
        v2[j] = temp2->data;
        temp2 = temp2->next;
        j++;
    }

    // Ordeno los vectores
    insert_sort(v1, m_num_nodes);
    insert_sort(v2, num_nodes_2);

    // Índice del 1er vector (v1)
    int v1_i = 0;

    // Índice del 2do vector (v2)
    int v2_i = 0;

  // Mientras no haya terminado de recorrer ambas Lists
  while (v1_i < m_num_nodes && v2_i < num_nodes_2) {
      if (v1[v1_i] == v2[v2_i]) {
          intersection_list.add_end(v1[v1_i]);
          v1_i++;
          v2_i++;
          num_inter++;
      } else if (v1[v1_i] < v2[v2_i]) {
          v1_i++;
      } else {
          v2_i++;
      }
  }

  // Solo si hay alguna intersección imprimo la nueva lista creada
  if (num_inter > 0) {
      cout << "Existen " << num_inter << " intersecciones " << endl;
      intersection_list.print();
  } else {
      cout << "No hay intersección en ambas listas" << endl;
  }
}

// Invertir la List
template<typename T>
void List<T>::invert()
{
    Node<T> *prev = NULL;
    Node<T> *next = NULL;
    Node<T> *temp = m_head;

    while (temp) {
        next = temp->next;
        temp->next = prev;
        prev = temp;
        temp = next;
    }
    m_head = prev;
}

// Cargar una lista desde un archivo
template<typename T>
void List<T>::load_file(string file)
{
    T line;
    ifstream in;
    in.open(file.c_str());

    if (!in.is_open()) {
        cout << "No se puede abrir el archivo: " << file << endl << endl;
    } else {
        while (in >> line) {
            add_end(line);
        }
    }
    in.close();
}

// Imprimir la Lista
template<typename T>
void List<T>::print()
{
    Node<T> *temp = m_head;
    if (!m_head) {
        cout << "La Lista está vacía " << endl;
    } else {
        while (temp) {
            temp->print();
            if (!temp->next) cout << "NULL";
                temp = temp->next;
        }
  }
  cout << endl << endl;
}




//(lib) Buscar todas las llaves iguales a la del parametro y retorna sus indices
template<typename T>
string List<T>::searchIndexes(T data_)
{
    //no implementado
}




//(lib) Buscar todas las llaves iguales a la del parametro (local) y retorna sus datos(llaves del server)
template<typename T>
string List<T>::searchallKeys(T data_)
{
    Node<T> *temp = m_head;
    int cont = 1;
    int cont2 = 0;

    string Datos="";

    while (temp) {
        if (temp->llave == data_) {//si la llave guardada es igual a la que le llega
            //cout << "El dato se encuentra en la posición: " << cont << endl;
            Datos+=temp->valor;
            Datos+="#";           //# es el separador de los datos (llaves)
            cont2++;
        }
        temp = temp->next;
        cont++;
    }

    if (cont2 == 0) {
        cout << "No existe el dato " << endl;
        return "no existe el dato";
    }else{
        return Datos;
    }
    cout << endl << endl;
}




// Buscar el dato de un nodo por llave y retorna el valor 
template<typename T>
string List<T>::searchData(T data_)
{
    Node<T> *temp = m_head;
    int cont = 1;
    int cont2 = 0;

    while (temp) {
        if (temp->llave == data_) {

            return temp->valor;  // retorna solo el valor que va a ser la llave que busca el dato en el server
            
            cont2++;
        }
        temp = temp->next;
        cont++;
    }

    if (cont2 == 0) {
        cout << "No existe el dato con la llave: "<<data_ << endl;
        return "0";
    }
    return "0";
}


// Ordenar de manera ascendente
template<typename T>
void List<T>::sort()
{
    T temp_data;
    Node<T> *aux_node = m_head;
    Node<T> *temp = aux_node;

    while (aux_node) {
        temp = aux_node;

        while (temp->next) {
            temp = temp->next;

            if (aux_node->data > temp->data) {
                temp_data = aux_node->data;
                aux_node->data = temp->data;
                temp->data = temp_data;
            }
        }

        aux_node = aux_node->next;
    }
}

// Guardar una lista en un archivo
template<typename T>
void List<T>::save_file(string file)
{
    Node<T> *temp = m_head;
    ofstream out;
    out.open(file.c_str());

    if (!out.is_open()) {
        cout << "No se puede guardar el archivo " << endl;
    } else {
        while (temp) {
            out << temp->data;
            out << " ";
            temp = temp->next;
        }
    }
    out.close();
}

template<typename T>
List<T>::~List() {}












#endif //PRUEBACALCULADORA_RMLIB_H