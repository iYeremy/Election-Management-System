#ifndef CIUDAD_H
#define CIUDAD_H

#include <string>

class Region;
class NodoCandidato;
class Candidato;

// representa una ciudad integrante de una region y que enlaza candidatos municipales (lista doble)
class Ciudad {
private:
    // nombre de la ciudad usado como clave en estructuras de busqueda
    std::string nombre;
    // region a la que pertenece en la multilista
    Region* region;
    // numero de ciudadanos habilitados para votar
    int censo;

    // acumuladores basicos de resultados municipales
    int votosCandidato[4];
    int votosBlanco;
    int votosNulos;
    int abstencion;
    // acumuladores para los resultados presidenciales por ciudad
    int votosPresidenciales[5];
    int votosPresidencialesBlanco;
    int votosPresidencialesNulos;
    int abstencionPresidencial;

    // cabecera de la lista doble de candidatos asociados a esta ciudad
    NodoCandidato* candidatosAlcaldia;
    // enlace simple usado por la multilista de regiones
    Ciudad* sigCiudad;

    // quita un nodo de la lista de candidatos de esta ciudad
    void removerNodoCandidato(NodoCandidato* nodo);

public:
    Ciudad();
    Ciudad(const std::string& nombre, int censo);

    // inserta un candidato de tipo alcaldia dentro de la lista de esta ciudad
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
    const int* getVotosPresidenciales() const;
    int getVotosPresidencialesBlanco() const;
    int getVotosPresidencialesNulos() const;
    int getAbstencionPresidencial() const;
    NodoCandidato* getCandidatosAlcaldia() const;
    Ciudad* getSigCiudad() const;

    // ---- Manejo de votos ----
    void registrarVotoCandidato(int idx);
    void registrarVotoBlanco();
    void registrarVotoNulo();
    void registrarAbstencion();
    void registrarVotoPresidencial(int idx);
    void registrarVotoPresidencialBlanco();
    void registrarVotoPresidencialNulo();
    void registrarAbstencionPresidencial();

    // ---- Estadisticas ----
    int totalVotos() const;
    int totalVotosValidos() const;
    int ganadorAlcaldia() const;
    double porcentajeCandidato(int idx) const;
    double porcentajeBlanco() const;
    double porcentajeNulo() const;
    double porcentajeAbstencion() const;
    int totalVotosPresidencialesCiudad() const;
    int totalVotosPresidencialesValidos() const;
    int ganadorPresidencialCiudad() const;
    double porcentajeVotoPresidencial(int idx) const;
    double porcentajePresidencialBlanco() const;
    double porcentajePresidencialNulo() const;
    double porcentajePresidencialAbstencion() const;
};

#endif
