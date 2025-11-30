#ifndef CIUDAD_H
#define CIUDAD_H

#include <string>

class Region;
class NodoCandidato;
class Candidato;

class Ciudad {
private:
    std::string nombre;
    Region* region;
    int censo;
    int votosCandidato[4];
    int votosBlanco;
    int votosNulos;
    int abstencion;
    NodoCandidato* candidatosAlcaldia;
    Ciudad* sigCiudad;

public:
    Ciudad();
    Ciudad(const std::string& nombre, int censo);

    void agregarCandidatoAlcaldia(Candidato* candidato);
    void setRegion(Region* region);
    void setSigCiudad(Ciudad* ciudad);

    const std::string& getNombre() const;
    Region* getRegion() const;
    int getCenso() const;
    const int* getVotosCandidato() const;
    int getVotosBlanco() const;
    int getVotosNulos() const;
    int getAbstencion() const;
    NodoCandidato* getCandidatosAlcaldia() const;
    Ciudad* getSigCiudad() const;
};

#endif
