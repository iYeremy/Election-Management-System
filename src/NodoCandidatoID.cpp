#include "NodoCandidatoID.h"

NodoCandidatoID::NodoCandidatoID() : candidato(nullptr), izq(nullptr), der(nullptr), altura(1) {}

void NodoCandidatoID::setCandidato(Candidato* nuevoCandidato) {
    candidato = nuevoCandidato;
}

Candidato* NodoCandidatoID::getCandidato() const {
    return candidato;
}

void NodoCandidatoID::setIzq(NodoCandidatoID* nodo) {
    izq = nodo;
}

NodoCandidatoID* NodoCandidatoID::getIzq() const {
    return izq;
}

void NodoCandidatoID::setDer(NodoCandidatoID* nodo) {
    der = nodo;
}

NodoCandidatoID* NodoCandidatoID::getDer() const {
    return der;
}

void NodoCandidatoID::setAltura(int nuevaAltura) {
    altura = nuevaAltura;
}

int NodoCandidatoID::getAltura() const {
    return altura;
}
