#include "Ciudad.h"

#include "Candidato.h"
#include "NodoCandidato.h"

Ciudad::Ciudad()
    : nombre(),
      region(nullptr),
      censo(0),
      votosCandidato{0, 0, 0, 0},
      votosBlanco(0),
      votosNulos(0),
      abstencion(0),
      candidatosAlcaldia(nullptr),
      sigCiudad(nullptr) {}

Ciudad::Ciudad(const std::string& nombre, int censo)
    : nombre(nombre),
      region(nullptr),
      censo(censo),
      votosCandidato{0, 0, 0, 0},
      votosBlanco(0),
      votosNulos(0),
      abstencion(0),
      candidatosAlcaldia(nullptr),
      sigCiudad(nullptr) {}

void Ciudad::agregarCandidatoAlcaldia(Candidato* candidato) {
    if (!candidato) {
        return;
    }

    NodoCandidato* nodo = candidato->getNodo();
    if (!nodo) {
        return;
    }

    nodo->setSigCiudad(candidatosAlcaldia);
    if (candidatosAlcaldia) {
        candidatosAlcaldia->setAntCiudad(nodo);
    }
    nodo->setSigCiudad(nullptr);
    nodo->setAntCiudad(nullptr);
    candidatosAlcaldia = nodo;
}

void Ciudad::setRegion(Region* nuevaRegion) {
    region = nuevaRegion;
}

void Ciudad::setSigCiudad(Ciudad* ciudad) {
    sigCiudad = ciudad;
}

const std::string& Ciudad::getNombre() const {
    return nombre;
}

Region* Ciudad::getRegion() const {
    return region;
}

int Ciudad::getCenso() const {
    return censo;
}

const int* Ciudad::getVotosCandidato() const {
    return votosCandidato;
}

int Ciudad::getVotosBlanco() const {
    return votosBlanco;
}

int Ciudad::getVotosNulos() const {
    return votosNulos;
}

int Ciudad::getAbstencion() const {
    return abstencion;
}

NodoCandidato* Ciudad::getCandidatosAlcaldia() const {
    return candidatosAlcaldia;
}

Ciudad* Ciudad::getSigCiudad() const {
    return sigCiudad;
}
