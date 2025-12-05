#include "Candidato.h"

#include "NodoCandidato.h"

Candidato::Candidato()
    : nombre(),
      apellido(),
      id(0),
      sexo(' '),
      fechaNacimiento(),
      ciudadNacimiento(),
      ciudadResidencia(nullptr),
      partido(nullptr),
      tipo(Tipo::ALCALDIA),
      esVicepresidencial(false),
      nodo(nullptr) {}

Candidato::Candidato(const std::string& nombre,
                     const std::string& apellido,
                     long id,
                     char sexo,
                     const Fecha& fechaNacimiento,
                     const std::string& ciudadNacimiento,
                     Ciudad* ciudadResidencia,
                     Partido* partido,
                     Tipo tipo,
                     NodoCandidato* nodo,
                     bool esVicepresidente)
    : nombre(nombre),
      apellido(apellido),
      id(id),
      sexo(sexo),
      fechaNacimiento(fechaNacimiento),
      ciudadNacimiento(ciudadNacimiento),
      ciudadResidencia(ciudadResidencia),
      partido(partido),
      tipo(tipo),
      esVicepresidencial(esVicepresidente),
      nodo(nodo) {
    if (this->nodo) {
        this->nodo->setCandidato(this);
    }
}

const std::string& Candidato::getNombre() const {
    return nombre;
}

const std::string& Candidato::getApellido() const {
    return apellido;
}

long Candidato::getId() const {
    return id;
}

char Candidato::getSexo() const {
    return sexo;
}

const Fecha& Candidato::getFechaNacimiento() const {
    return fechaNacimiento;
}

const std::string& Candidato::getCiudadNacimiento() const {
    return ciudadNacimiento;
}

Ciudad* Candidato::getCiudadResidencia() const {
    return ciudadResidencia;
}

Partido* Candidato::getPartido() const {
    return partido;
}

Candidato::Tipo Candidato::getTipo() const {
    return tipo;
}

NodoCandidato* Candidato::getNodo() const {
    return nodo;
}

void Candidato::setEsVicepresidente(bool esVice) {
    esVicepresidencial = esVice;
}

bool Candidato::esVicepresidente() const {
    return esVicepresidencial;
}
