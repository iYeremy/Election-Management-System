#include "AVLCiudades.h"
#include "NodoCiudad.h"
#include "Ciudad.h"
#include "Pila.h"
#include <iostream>

// Constructor: inicia el arbol vacio
AVLCiudades::AVLCiudades() : raiz(nullptr) {}

// Destructor iterativo apoyado en la pila generica
AVLCiudades::~AVLCiudades() {
    if (!raiz) return;

    Pila<NodoCiudad*> pila;
    pila.push(raiz);

    while (!pila.empty()) {
        NodoCiudad* actual = pila.pop();

        if (actual->getIzq()) pila.push(actual->getIzq());
        if (actual->getDer()) pila.push(actual->getDer());

        delete actual;
    }
}

// Devuelve la altura almacenada del nodo (0 si es nulo)
int AVLCiudades::obtenerAltura(NodoCiudad* nodo) {
    if (!nodo) return 0;
    return nodo->getAltura();
}

// Calcula el factor de balance del nodo
int AVLCiudades::obtenerBalance(NodoCiudad* nodo) {
    if (!nodo) return 0;
    return obtenerAltura(nodo->getIzq()) - obtenerAltura(nodo->getDer());
}

// Recalcula la altura usando las alturas de los hijos
void AVLCiudades::actualizarAltura(NodoCiudad* nodo) {
    if (!nodo) return;

    int alturaIzq = obtenerAltura(nodo->getIzq());
    int alturaDer = obtenerAltura(nodo->getDer());

    int nuevaAltura;

    if (alturaIzq > alturaDer) {
        nuevaAltura = alturaIzq + 1;
    } else {
        nuevaAltura = alturaDer + 1;
    }

    nodo->setAltura(nuevaAltura);
}


// Rotacion simple derecha
NodoCiudad* AVLCiudades::rotarDerecha(NodoCiudad* y) {
    NodoCiudad* x = y->getIzq();
    NodoCiudad* T2 = x->getDer();

    //rotaciones:
    x->setDer(y);
    y->setIzq(T2);

    //alturas nuevas despues de la rotacion
    actualizarAltura(y);
    actualizarAltura(x);

    return x;
}

// Rotacion simple izquierda
NodoCiudad* AVLCiudades::rotarIzquierda(NodoCiudad* x) {
    NodoCiudad* y = x->getDer();
    NodoCiudad* T2 = y->getIzq();

    //rotaciones:
    y->setIzq(x);
    x->setDer(T2);

    //alturas nuevas despues de la rotacion
    actualizarAltura(x);
    actualizarAltura(y);

    return y;
}

// Aplica la rotacion adecuada segun el factor de balance
NodoCiudad* AVLCiudades::balancear(NodoCiudad* nodo) {
    if (!nodo) return nullptr;

    actualizarAltura(nodo);
    int balance = obtenerBalance(nodo);

    //Caso rotacion izquierda izquierda
    if (balance > 1 && obtenerBalance(nodo->getIzq()) >= 0) {
        return rotarDerecha(nodo);
    }

    //Caso rotacion izquierda derecha
    if (balance > 1 && obtenerBalance(nodo->getIzq()) < 0) {
        nodo->setIzq(rotarIzquierda(nodo->getIzq()));
        return rotarDerecha(nodo);
    }

    //Caso rotacion derecha-derecha
    if (balance < -1 && obtenerBalance(nodo->getDer()) <= 0) {
        return rotarIzquierda(nodo);
    }

    //Caso Derecha-Izquierda
    if (balance < -1 && obtenerBalance(nodo->getDer()) > 0) {
        nodo->setDer(rotarDerecha(nodo->getDer()));
        return rotarIzquierda(nodo);
    }

    return nodo;
}

// Inserta una ciudad manteniendo el orden alfabetico
void AVLCiudades::insertar(Ciudad* ciudad) {
    if (!ciudad) return;

    //Se crea el nuevo nodo
    NodoCiudad* nuevo = new NodoCiudad();
    nuevo->setCiudad(ciudad);
    nuevo->setAltura(1); //La altura inicial sera 1

    //Si la raiz no existe, se crea una nueva
    if (!raiz) {
        raiz = nuevo;
        return;
    }

    Pila<NodoCiudad*> camino;
    NodoCiudad* actual = raiz;
    NodoCiudad* padre = nullptr;
    bool irIzquierda = false;

    //Se busca en que posicion es posible insertar el nodo (reglas de arboles binarios)
    while (actual) {
        camino.push(actual);
        padre = actual;

        if (ciudad->getNombre() < actual->getCiudad()->getNombre()) {
            actual = actual->getIzq();
            irIzquierda = true;
        } else if (ciudad->getNombre() > actual->getCiudad()->getNombre()) {
            actual = actual->getDer();
            irIzquierda = false;
        } else {
            //En este condicional se evalua si hay una ciudad repetida, se elimina
            delete nuevo;
            return;
        }
    }

    //insercion del nodo:
    if (irIzquierda) {
        padre->setIzq(nuevo);
    } else {
        padre->setDer(nuevo);
    }

    //Balanceo del arbol
    while (!camino.empty()) {
        NodoCiudad* nodo = camino.pop();

        NodoCiudad* balanceado = balancear(nodo);

        // Si hubo rotacion se actualiza al padre
        if (balanceado != nodo) {
            if (camino.empty()) {
                raiz = balanceado;
            } else {
                NodoCiudad* padreNodo = camino.top();
                if (padreNodo->getIzq() == nodo) {
                    padreNodo->setIzq(balanceado);
                } else {
                    padreNodo->setDer(balanceado);
                }
            }
        }
    }
}

// Busca una ciudad por nombre exacto
Ciudad* AVLCiudades::buscar(const std::string& nombre) {
    NodoCiudad* actual = raiz;

    while (actual) {
        if (nombre == actual->getCiudad()->getNombre()) {
            return actual->getCiudad();
        } else if (nombre < actual->getCiudad()->getNombre()) {
            actual = actual->getIzq();
        } else {
            actual = actual->getDer();
        }
    }
    return nullptr;
}

// Recorre el arbol en inorden y muestra las ciudades
void AVLCiudades::imprimir() {
    if (!raiz) {
        std::cout << "Arbol vacio\n";
        return;
    }

    //Aqui se realiza un recorrido en inorden utilizando la pila
    Pila<NodoCiudad*> pila;
    NodoCiudad* actual = raiz;

    std::cout << "\nA continuacion se presentan las Ciudades organizadas alfabeticamente\n";

    while (actual || !pila.empty()) {
        //Recorremos hasta el mas izquierdo
        while (actual) {
            pila.push(actual);
            actual = actual->getIzq();
        }

        //Se procesa el nodo del tope
        actual = pila.pop();
        std::cout << actual->getCiudad()->getNombre()
                  << " - Censo: " << actual->getCiudad()->getCenso() << "\n";

        //se continua el recorrido por la parte derecha
        actual = actual->getDer();
    }
}
