//ListaGenerica.h

#ifndef LISTA_GENERICA_H
#define LISTA_GENERICA_H

#include <iostream>
#include <fstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
///////////// Clase Nodo ///////////////////////
template <class T>
class Node
{
    public:
        Node();
        Node(T,T,T);//llave,valor,size
        ~Node();

        Node *next;

        T llave;
        T valor;
        T size;

        void delete_all();
        void print();
};

//////////////////////////////////////////////////////////////////////////////
///////////// Nodo ///////////////////////
// Constructor por defecto
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
    cout << llave << "-> ";
}

template<typename T>
Node<T>::~Node() {}

//////////////////////////////////////////////////////////////////////////////
///////////// Clase Lista ///////////////////////
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
        //string searchallData(T);
        string searchData(T); //busca un dato por llave para retornarlo junto con su tamano
        void sort();
        string iterarTodo(); //saca todos los datos de la lista         llave1@valor1&llave2@valor2&......
        string retornarTodo();

    private:
        Node<T> *m_head;
        int m_num_nodes;
};

//////////////////////////////////////////////////////////////////////////////
///////////// Lista ///////////////////////
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

// Eliminar por data(llave) del nodo
template<typename T>
void List<T>::del_by_data(T data_)
{
    Node<T> *temp = m_head;
    Node<T> *temp1 = m_head->next;

    int cont = 0;

    if (m_head->llave == data_) {
        m_head = temp->next;
    } else {
        while (temp1) {
            if (temp1->llave == data_) {
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



//(serv) Buscar todas las llaves iguales a la del parametro y retorna sus indices
template<typename T>
string List<T>::searchIndexes(T data_)
{
    Node<T> *temp = m_head;
    int cont = 1;
    int cont2 = 0;

    string posiciones="";

    while (temp) {
        if (temp->llave == data_) {//si la llave guardada es igual a la que le llega
            cout << "El dato se encuentra en la posición: " << cont << endl;
            posiciones+=to_string(cont);
            posiciones+="#";
            cont2++;
        }
        temp = temp->next;
        cont++;
    }

    if (cont2 == 0) {
        cout << "No existe el dato " << endl;
        return "no existe el dato";
    }else{
        return posiciones;
    }
    cout << endl << endl;
}





// Buscar el dato de un nodo por llave y retorna el valor y size
template<typename T>
string List<T>::searchData(T data_)
{
    Node<T> *temp = m_head;
    int cont = 1;
    int cont2 = 0;

    while (temp) {
        if (temp->llave == data_) {
            //cout << "El dato se encuentra en la posición: " << cont << endl;
            //cout << "El dato guardado es: " << temp->valor << endl;
            //cout << "El size del dato es : " << temp->size << endl;
            return temp->valor;//+"#"+temp->size;
            
            cont2++;
        }
        temp = temp->next;
        cont++;
    }

    if (cont2 == 0) {
        cout << "No existe el dato con la llave: "<<data_ << endl;
        return "false";
    }
    return "false";
}


//Busca y retorna todos los datos con sus llaves en formato
//string     llave@dato#llave@dato#llave@dato#
template<typename T>
string List<T>::iterarTodo()
{
    Node<T> *temp = m_head;
    int cont = 1;
    int cont2 = 0;

    //separadorNodo #
    //separa dato @
    string datos="";

    while (temp) {
        
            datos=datos+temp->llave+"&"+temp->valor+"@";
            
            cont2++;
        
        temp = temp->next;
       
    }
    return datos; //retorna todos los datos en un string
}





//Busca y retorna todos los datos con sus llaves en string acomodado
//string     llave: valor
//           llave2: valor2
template<typename T>
string List<T>::retornarTodo()
{
    Node<T> *temp = m_head;
    int cont = 1;
    int cont2 = 0;

    //separadorNodo #
    //separa dato @
    string datos="";
    string formato="-----------------\nLlave       Valor\n";
    while (temp) {
        
            datos=datos+temp->llave+": "+temp->valor+"\n";
            
            cont2++;
        
        temp = temp->next;
       
    }
    return formato+datos; //retorna todos los datos en un string
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

#endif // 
