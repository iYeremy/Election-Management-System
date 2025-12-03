#include "Partido.h"

#include "Candidato.h"
#include "Duo.h"
#include "NodoCandidato.h"

Partido::Partido() : nombre(), representante(), listaCandidatosAlcaldia(nullptr), formulaPresidencial(nullptr) {}

Partido::Partido(const std::string& nombre, const std::string& representante)
    : nombre(nombre), representante(representante), listaCandidatosAlcaldia(nullptr), formulaPresidencial(nullptr) {}

// inserta un candidato municipal en la lista del partido
void Partido::agregarCandidatoAlcaldia(Candidato* candidato) {
    if (!candidato || candidato->getTipo() != Candidato::Tipo::ALCALDIA) return;
    if (candidato->getPartido() && candidato->getPartido() != this) return;

    NodoCandidato* nodo = candidato->getNodo();
    if (!nodo) return;

    if (nodo->getPartidoLista() == this) {
        return;
    }

    if (Partido* partidoAnterior = nodo->getPartidoLista()) {
        partidoAnterior->removerCandidatoDeLista(nodo);
    }

    nodo->setAntPartido(nullptr);
    nodo->setSigPartido(listaCandidatosAlcaldia);
    if (listaCandidatosAlcaldia) {
        listaCandidatosAlcaldia->setAntPartido(nodo);
    }

    listaCandidatosAlcaldia = nodo;
    nodo->setPartidoLista(this);
}

// elimina un candidato de la lista local preservando enlaces
void Partido::removerCandidatoDeLista(NodoCandidato* nodo) {
    if (!nodo || nodo->getPartidoLista() != this) return;

    if (nodo->getAntPartido()) {
        nodo->getAntPartido()->setSigPartido(nodo->getSigPartido());
    } else if (listaCandidatosAlcaldia == nodo) {
        listaCandidatosAlcaldia = nodo->getSigPartido();
    }

    if (nodo->getSigPartido()) {
        nodo->getSigPartido()->setAntPartido(nodo->getAntPartido());
    }

    nodo->setAntPartido(nullptr);
    nodo->setSigPartido(nullptr);
    nodo->setPartidoLista(nullptr);
}


void Partido::asignarFormulaPresidencial(Duo* duo) {
    setFormulaPresidencial(duo);
    if (formulaPresidencial) {
        formulaPresidencial->setPartido(this);
    }
}

void Partido::setNombre(const std::string& nuevoNombre) {
    nombre = nuevoNombre;
}

void Partido::setRepresentante(const std::string& nuevoRepresentante) {
    representante = nuevoRepresentante;
}

void Partido::setListaCandidatosAlcaldia(NodoCandidato* lista) {
    listaCandidatosAlcaldia = lista;
}

void Partido::setFormulaPresidencial(Duo* duo) {
    formulaPresidencial = duo;
}

const std::string& Partido::getNombre() const {
    return nombre;
}

const std::string& Partido::getRepresentante() const {
    return representante;
}

NodoCandidato* Partido::getListaCandidatosAlcaldia() const {
    return listaCandidatosAlcaldia;
}

Duo* Partido::getFormulaPresidencial() const {
    return formulaPresidencial;
}
