#include "Partido.h"

#include "Candidato.h"
#include "Duo.h"
#include "NodoCandidato.h"

Partido::Partido() : nombre(), representante(), listaCandidatosAlcaldia(nullptr), formulaPresidencial(nullptr) {}

Partido::Partido(const std::string& nombre, const std::string& representante)
    : nombre(nombre), representante(representante), listaCandidatosAlcaldia(nullptr), formulaPresidencial(nullptr) {}

void Partido::agregarCandidatoAlcaldia(Candidato* candidato) {
    if (!candidato) return;

    NodoCandidato* nodo = candidato->getNodo();
    if (!nodo) return;

    // Antes de insertar, LIMPIAR punteros previos
    nodo->setSigPartido(nullptr);  
    nodo->setAntPartido(nullptr);  

    // Insertar al inicio
    nodo->setSigPartido(listaCandidatosAlcaldia);
    if (listaCandidatosAlcaldia) {
        listaCandidatosAlcaldia->setAntPartido(nodo);
    }

    listaCandidatosAlcaldia = nodo;
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
