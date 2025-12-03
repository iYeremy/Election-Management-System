#include "AVLCandidatos.h"
#include "NodoCandidatoID.h"
#include "Candidato.h"
#include "Pila.h"
#include <iostream>

// Constructor: inicia el arbol vacio
AVLCandidatos::AVLCandidatos() : raiz(nullptr) {}

// Destructor iterativo basado en la pila generica
AVLCandidatos::~AVLCandidatos() {

    if (!raiz) return;

    Pila<NodoCandidatoID*> pila;
    pila.push(raiz);

    while (!pila.empty()) {
        NodoCandidatoID* actual = pila.pop();

        if (actual->getIzq()) pila.push(actual->getIzq());
        if (actual->getDer()) pila.push(actual->getDer());

        delete actual;
    }
}

// Obtiene la altura almacenada en el nodo
int AVLCandidatos::obtenerAltura(NodoCandidatoID* nodo) {
    if (!nodo) return 0;
    return nodo->getAltura();
}

// Calcula el factor de balance del nodo
int AVLCandidatos::obtenerBalance(NodoCandidatoID* nodo) {
    if (!nodo) return 0;
    return obtenerAltura(nodo->getIzq()) - obtenerAltura(nodo->getDer());
}

// Recalcula la altura segun los hijos
void AVLCandidatos::actualizarAltura(NodoCandidatoID* nodo) {
    if (!nodo) return;

    int alturaIzq = obtenerAltura(nodo->getIzq());
    int alturaDer = obtenerAltura(nodo->getDer());

    int nuevaAltura = (alturaIzq > alturaDer ? alturaIzq : alturaDer) + 1;
    nodo->setAltura(nuevaAltura);
}

// Rotacion simple derecha
NodoCandidatoID* AVLCandidatos::rotarDerecha(NodoCandidatoID* y) {
    NodoCandidatoID* x = y->getIzq();
    NodoCandidatoID* T2 = x->getDer();

    x->setDer(y);
    y->setIzq(T2);

    actualizarAltura(y);
    actualizarAltura(x);

    return x;
}

// Rotacion simple izquierda
NodoCandidatoID* AVLCandidatos::rotarIzquierda(NodoCandidatoID* x) {
    NodoCandidatoID* y = x->getDer();
    NodoCandidatoID* T2 = y->getIzq();

    y->setIzq(x);
    x->setDer(T2);

    actualizarAltura(x);
    actualizarAltura(y);

    return y;
}

// Determina y ejecuta la rotacion necesaria para equilibrar el nodo
NodoCandidatoID* AVLCandidatos::balancear(NodoCandidatoID* nodo) {
    if (!nodo) return nullptr;

    actualizarAltura(nodo);
    int balance = obtenerBalance(nodo);

    //Izquierda Izquierda
    if (balance > 1 && obtenerBalance(nodo->getIzq()) >= 0) {
        return rotarDerecha(nodo);
    }

    //Izquierda Derecha
    if (balance > 1 && obtenerBalance(nodo->getIzq()) < 0) {
        nodo->setIzq(rotarIzquierda(nodo->getIzq()));
        return rotarDerecha(nodo);
    }

    //Derecha Derecha
    if (balance < -1 && obtenerBalance(nodo->getDer()) <= 0) {
        return rotarIzquierda(nodo);
    }

    // Derecha Izquierda
    if (balance < -1 && obtenerBalance(nodo->getDer()) > 0) {
        nodo->setDer(rotarDerecha(nodo->getDer()));
        return rotarIzquierda(nodo);
    }

    return nodo;
}

// Inserta el candidato usando el ID como clave
void AVLCandidatos::insertar(Candidato* candidato) {
    if (!candidato) return;

    NodoCandidatoID* nuevo = new NodoCandidatoID();
    nuevo->setCandidato(candidato);
    nuevo->setAltura(1);

    if (!raiz) {
        raiz = nuevo;
        return;
    }

    Pila<NodoCandidatoID*> camino;
    NodoCandidatoID* actual = raiz;
    NodoCandidatoID* padre = nullptr;
    bool irIzquierda = false;

    while (actual) {
        camino.push(actual);
        padre = actual;

        if (candidato->getId() < actual->getCandidato()->getId()) {
            actual = actual->getIzq();
            irIzquierda = true;
        } else if (candidato->getId() > actual->getCandidato()->getId()) {
            actual = actual->getDer();
            irIzquierda = false;
        } else {
            // ID duplicado
            delete nuevo;
            return;
        }
    }

    //Insercion final
    if (irIzquierda) {
        padre->setIzq(nuevo);
    } else {
        padre->setDer(nuevo);
    }

    // Balanceo
    while (!camino.empty()) {
        NodoCandidatoID* nodo = camino.pop();

        NodoCandidatoID* balanceado = balancear(nodo);

        if (balanceado != nodo) {
            if (camino.empty()) {
                raiz = balanceado;
            } else {
                NodoCandidatoID* padreNodo = camino.top();
                if (padreNodo->getIzq() == nodo) {
                    padreNodo->setIzq(balanceado);
                } else {
                    padreNodo->setDer(balanceado);
                }
            }
        }
    }
}

// Recorrido iterativo simple para hallar un ID
Candidato* AVLCandidatos::buscar(long id) {
    NodoCandidatoID* actual = raiz;

    while (actual) {
        if (id == actual->getCandidato()->getId()) {
            return actual->getCandidato();
        } else if (id < actual->getCandidato()->getId()) {
            actual = actual->getIzq();
        } else {
            actual = actual->getDer();
        }
    }

    return nullptr;
}

// Recorre el arbol en inorden para mostrar los candidatos
void AVLCandidatos::imprimir() {
    if (!raiz) {
        std::cout << "Arbol vacio\n";
        return;
    }

    //Recorrido en forma de inorden a partir de la pila
    Pila<NodoCandidatoID*> pila;
    NodoCandidatoID* actual = raiz;

    std::cout << "\nOrganizacion de candidatos por ID\n";

    while (actual || !pila.empty()) {
        //Ir hasta el nodo mas a la izquierda
        while (actual) {
            pila.push(actual);
            actual = actual->getIzq();
        }

        //Procesar el nodo que esta arriba (tope)
        actual = pila.pop();
        Candidato* c = actual->getCandidato();
        std::cout << "ID: " << c->getId() << " - "
                  << c->getNombre() << " " << c->getApellido() << "\n";

        //Ir al subarbol derecho
        actual = actual->getDer();
    }
}
