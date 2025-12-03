#ifndef SISTEMA_ELECTORAL_H
#define SISTEMA_ELECTORAL_H

#include <string>

class MultilistaRegiones;
class AVLCiudades;
class AVLCandidatos;
class Region;
class Ciudad;
class Candidato;
class Duo;
class Partido;

// orquesta la carga de datos y las consultas principales del sistema
class SistemaElectoral {
private:
    // multilista que agrupa regiones y sus ciudades
    MultilistaRegiones* regiones;
    // arbol que permite buscar ciudades por nombre
    AVLCiudades* arbolCiudades;
    // arbol que organiza los candidatos por ID
    AVLCandidatos* arbolCandidatos;

    Partido partidos[5];
    Duo formulas[5];


public:
    // constructor y destructor
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
    AVLCiudades* getArbolCiudades() const;
    AVLCandidatos* getArbolCandidatos() const;
};

#endif
