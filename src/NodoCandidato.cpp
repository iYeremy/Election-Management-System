#include "NodoCandidato.h"

NodoCandidato::NodoCandidato()
    : cand(nullptr),
      sigCiudad(nullptr),
      antCiudad(nullptr),
      sigPartido(nullptr),
      antPartido(nullptr),
      ciudadLista(nullptr),
      partidoLista(nullptr) {}

void NodoCandidato::setCandidato(Candidato* candidato) {
    cand = candidato;
}

Candidato* NodoCandidato::getCandidato() const {
    return cand;
}

void NodoCandidato::setSigCiudad(NodoCandidato* nodo) {
    sigCiudad = nodo;
}

NodoCandidato* NodoCandidato::getSigCiudad() const {
    return sigCiudad;
}

void NodoCandidato::setAntCiudad(NodoCandidato* nodo) {
    antCiudad = nodo;
}

NodoCandidato* NodoCandidato::getAntCiudad() const {
    return antCiudad;
}

void NodoCandidato::setSigPartido(NodoCandidato* nodo) {
    sigPartido = nodo;
}

NodoCandidato* NodoCandidato::getSigPartido() const {
    return sigPartido;
}

void NodoCandidato::setAntPartido(NodoCandidato* nodo) {
    antPartido = nodo;
}

NodoCandidato* NodoCandidato::getAntPartido() const {
    return antPartido;
}

void NodoCandidato::setCiudadLista(Ciudad* ciudad) {
    ciudadLista = ciudad;
}

Ciudad* NodoCandidato::getCiudadLista() const {
    return ciudadLista;
}

void NodoCandidato::setPartidoLista(Partido* partido) {
    partidoLista = partido;
}

Partido* NodoCandidato::getPartidoLista() const {
    return partidoLista;
}
