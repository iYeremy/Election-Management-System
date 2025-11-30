#ifndef CANDIDATO_H
#define CANDIDATO_H

#include <string>

#include "Fecha.h"

class Ciudad;
class Partido;
class NodoCandidato;

class Candidato {
public:
    enum class Tipo { ALCALDIA, PRESIDENCIA };

private:
    std::string nombre;
    std::string apellido;
    long id;
    char sexo;
    Fecha fechaNacimiento;
    std::string ciudadNacimiento;
    Ciudad* ciudadResidencia;
    Partido* partido;
    Tipo tipo;
    NodoCandidato* nodo;

public:
    Candidato();
    Candidato(const std::string& nombre,
              const std::string& apellido,
              long id,
              char sexo,
              const Fecha& fechaNacimiento,
              const std::string& ciudadNacimiento,
              Ciudad* ciudadResidencia,
              Partido* partido,
              Tipo tipo,
              NodoCandidato* nodo);

    const std::string& getNombre() const;
    const std::string& getApellido() const;
    long getId() const;
    char getSexo() const;
    const Fecha& getFechaNacimiento() const;
    const std::string& getCiudadNacimiento() const;
    Ciudad* getCiudadResidencia() const;
    Partido* getPartido() const;
    Tipo getTipo() const;
    NodoCandidato* getNodo() const;
};

#endif
