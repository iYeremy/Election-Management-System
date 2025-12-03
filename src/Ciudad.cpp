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

// inserta un candidato municipal dentro de la lista de esta ciudad
void Ciudad::agregarCandidatoAlcaldia(Candidato* candidato) {
    if (!candidato || candidato->getTipo() != Candidato::Tipo::ALCALDIA) return;

    NodoCandidato* nodo = candidato->getNodo();
    if (!nodo) return;

    if (nodo->getCiudadLista() == this) {
        return;
    }

    if (Ciudad* ciudadAnterior = nodo->getCiudadLista()) {
        ciudadAnterior->removerNodoCandidato(nodo);
    }

    nodo->setAntCiudad(nullptr);
    nodo->setSigCiudad(candidatosAlcaldia);
    if (candidatosAlcaldia) {
        candidatosAlcaldia->setAntCiudad(nodo);
    }

    candidatosAlcaldia = nodo;
    nodo->setCiudadLista(this);
}

// elimina el nodo de la lista manteniendo enlaces consistentes
void Ciudad::removerNodoCandidato(NodoCandidato* nodo) {
    if (!nodo || nodo->getCiudadLista() != this) return;

    if (nodo->getAntCiudad()) {
        nodo->getAntCiudad()->setSigCiudad(nodo->getSigCiudad());
    } else if (candidatosAlcaldia == nodo) {
        candidatosAlcaldia = nodo->getSigCiudad();
    }

    if (nodo->getSigCiudad()) {
        nodo->getSigCiudad()->setAntCiudad(nodo->getAntCiudad());
    }

    nodo->setAntCiudad(nullptr);
    nodo->setSigCiudad(nullptr);
    nodo->setCiudadLista(nullptr);
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

// Incrementa el contador del candidato indicado si el censo lo permite
void Ciudad::registrarVotoCandidato(int idx) {
    if (idx < 0 || idx >= 4) {
        return;
    }

    if (totalVotos() >= censo) {
        return;
    }

    votosCandidato[idx]++;
}

void Ciudad::registrarVotoBlanco() {
    if (totalVotos() >= censo) {
        return;
    }

    votosBlanco++;
}

void Ciudad::registrarVotoNulo() {
    if (totalVotos() >= censo) {
        return;
    }

    votosNulos++;
}

void Ciudad::registrarAbstencion() {
    if (totalVotos() >= censo) {
        return;
    }

    abstencion++;
}

// Devuelve el total de papeletas procesadas
int Ciudad::totalVotos() const {
    int total = votosBlanco + votosNulos + abstencion;
    for (int i = 0; i < 4; i++) {
        total += votosCandidato[i];
    }
    return total;
}

int Ciudad::totalVotosValidos() const {
    int total = 0;
    for (int i = 0; i < 4; i++) {
        total += votosCandidato[i];
    }
    return total;
}

// Determina el indice del candidato con mayor votacion valida
int Ciudad::ganadorAlcaldia() const {
    if (totalVotosValidos() == 0) {
        return -1;
    }

    int mejorIndice = -1;
    int mejorVotos = -1;
    bool empate = false;

    for (int i = 0; i < 4; i++) {
        if (votosCandidato[i] > mejorVotos) {
            mejorVotos = votosCandidato[i];
            mejorIndice = i;
            empate = false;
        } else if (votosCandidato[i] == mejorVotos && mejorVotos != -1) {
            empate = true;
        }
    }

    if (empate) {
        return -1;
    }

    return mejorIndice;
}

double Ciudad::porcentajeCandidato(int idx) const {
    if (idx < 0 || idx >= 4) {
        return 0.0;
    }

    int total = totalVotos();
    if (total == 0) {
        return 0.0;
    }

    return (static_cast<double>(votosCandidato[idx]) * 100.0) / total;
}

double Ciudad::porcentajeBlanco() const {
    int total = totalVotos();
    if (total == 0) {
        return 0.0;
    }

    return (static_cast<double>(votosBlanco) * 100.0) / total;
}

double Ciudad::porcentajeNulo() const {
    int total = totalVotos();
    if (total == 0) {
        return 0.0;
    }

    return (static_cast<double>(votosNulos) * 100.0) / total;
}

double Ciudad::porcentajeAbstencion() const {
    int total = totalVotos();
    if (total == 0) {
        return 0.0;
    }

    return (static_cast<double>(abstencion) * 100.0) / total;
}
