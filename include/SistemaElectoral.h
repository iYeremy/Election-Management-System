#ifndef SISTEMA_ELECTORAL_H
#define SISTEMA_ELECTORAL_H

#include <string>

class MultilistaRegiones;
class AVL_Ciudades;
class AVL_Candidatos;
class Region;
class Ciudad;
class Candidato;
class Duo;
class Partido;

class SistemaElectoral {
private:
    MultilistaRegiones* regiones;
    AVL_Ciudades* arbolCiudades;
    AVL_Candidatos* arbolCandidatos;

    Partido partidos[5];
    Duo formulas[5];


public:
    // Constructor y destructor
    SistemaElectoral();
    ~SistemaElectoral();

    // ---- CARGA DE DATOS ----
    void cargarRegiones(const std::string& ruta);
    void cargarCiudades(const std::string& ruta);
    void cargarPartidos(const std::string& ruta);
    void cargarCandidatosAlcaldia(const std::string& ruta);
    void cargarCandidatosPresidencia(const std::string& ruta);
    void cargarFormulasPresidenciales(const std::string& ruta);

    // ---- CONSULTAS ----
    Ciudad* buscarCiudad(const std::string& nombre);
    Candidato* buscarCandidato(long id);

    // ---- PROCESOS ----
    void simularVotacion();
    void calcularGanadoresMunicipales();
    void calcularGanadoresRegionales();
    void calcularGanadorPresidencial();

    // ---- REPORTES ----
    void generarReporteCiudades(const std::string& rutaSalida);
    void generarReporteRegiones(const std::string& rutaSalida);
    void generarReporteNacional(const std::string& rutaSalida);

    // ---- GETTERS ----
    MultilistaRegiones* getRegiones() const;
    AVL_Ciudades* getArbolCiudades() const;
    AVL_Candidatos* getArbolCandidatos() const;
};

#endif
