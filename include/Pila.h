//
// Created by PC on 30/11/2025.
//

#ifndef PILA_H
#define PILA_H

template <typename T>
class NodoPila {
public:
    T dato;
    NodoPila* siguiente;

    NodoPila(T d) : dato(d), siguiente(nullptr) {}
};

template <typename T>
class Pila {
private:
    NodoPila<T>* tope;
    int tam;

public:
    Pila();
    ~Pila();
    //Funcion para agregar elemento
    void push(T dato);
    //Funcion para sacar elemento
    T pop();
    //Esta funcion es como un metodo getter para ver el tope sin sacarlo
    T top();
    //Verificacion de si esta vacia
    bool empty();
    int size();
};


template <typename T>
Pila<T>::Pila() : tope(nullptr), tam(0) {}

template <typename T>
Pila<T>::~Pila() {
    while (!empty()) {
        pop();
    }
}

template <typename T>
void Pila<T>::push(T dato) {
    NodoPila<T>* nuevo = new NodoPila<T>(dato);
    nuevo->siguiente = tope;
    tope = nuevo;
    tam++;
}

template <typename T>
T Pila<T>::pop() {
    if (empty()) {
        return nullptr;
    }

    NodoPila<T>* temp = tope;
    T dato = tope->dato;
    tope = tope->siguiente;
    delete temp;
    tam--;

    return dato;
}

template <typename T>
T Pila<T>::top() {
    if (empty()) {
        return nullptr;
    }
    return tope->dato;
}

template <typename T>
bool Pila<T>::empty() {
    return tope == nullptr;
}

template <typename T>
int Pila<T>::size() {
    return tam;
}
#endif //PROYECTOCC_PILA_H

// NOTA IMPORTANTE:
// Esta pila está diseñada solo para tipos puntero (ej: NodoCiudad*, NodoCandidatoID*).
// pop() y top() retornan nullptr cuando está vacía.
// No usar con tipos no puntero.
