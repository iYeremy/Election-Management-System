#include "Duo.h"

Duo::Duo()
    : presidente(nullptr),
      vicepresidente(nullptr),
      partido(nullptr),
      votos(0),
      numeroPartido(0) {}

Duo::Duo(Candidato* presidente, Candidato* vicepresidente, int numeroPartido)
    : presidente(presidente),
      vicepresidente(vicepresidente),
      partido(nullptr),
      votos(0),
      numeroPartido(numeroPartido) {}

void Duo::setPresidente(Candidato* nuevoPresidente) {
    presidente = nuevoPresidente;
}

void Duo::setVicepresidente(Candidato* nuevoVicepresidente) {
    vicepresidente = nuevoVicepresidente;
}

void Duo::setPartido(Partido* nuevoPartido) {
    partido = nuevoPartido;
}

void Duo::setVotos(int nuevosVotos) {
    votos = nuevosVotos;
}

void Duo::setNumeroPartido(int numero) {
    numeroPartido = numero;
}

Candidato* Duo::getPresidente() const {
    return presidente;
}

Candidato* Duo::getVicepresidente() const {
    return vicepresidente;
}

Partido* Duo::getPartido() const {
    return partido;
}

int Duo::getVotos() const {
    return votos;
}

int Duo::getNumeroPartido() const {
    return numeroPartido;
}
