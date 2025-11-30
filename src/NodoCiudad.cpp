#include "NodoCiudad.h"

NodoCiudad::NodoCiudad() : ciudad(nullptr), izq(nullptr), der(nullptr), altura(1) {}

void NodoCiudad::setCiudad(Ciudad* nuevaCiudad) {
    ciudad = nuevaCiudad;
}

Ciudad* NodoCiudad::getCiudad() const {
    return ciudad;
}

void NodoCiudad::setIzq(NodoCiudad* nodo) {
    izq = nodo;
}

NodoCiudad* NodoCiudad::getIzq() const {
    return izq;
}

void NodoCiudad::setDer(NodoCiudad* nodo) {
    der = nodo;
}

NodoCiudad* NodoCiudad::getDer() const {
    return der;
}

void NodoCiudad::setAltura(int nuevaAltura) {
    altura = nuevaAltura;
}

int NodoCiudad::getAltura() const {
    return altura;
}
