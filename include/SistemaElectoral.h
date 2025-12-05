#ifndef SISTEMA_ELECTORAL_H
#define SISTEMA_ELECTORAL_H

#include <string>

#include "Partido.h"
#include "Duo.h"

class MultilistaRegiones;
class AVLCiudades;
class AVLCandidatos;
class Region;
class Ciudad;
class Candidato;

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
    bool segundaVueltaPendiente;
    bool segundaVueltaRealizada;
    int indicesSegundaVuelta[2];
    long long votosSegundaVuelta[2];
    long long blancosSegundaVuelta;
    long long nulosSegundaVuelta;
    long long abstencionSegundaVuelta;
    long long totalVotosPresidenciales;
    long long blancosPresidenciales;
    long long nulosPresidenciales;
    long long abstencionPresidencial;
    bool simulacionEnCurso;
    bool simulacionEjecutadaFlag;
    std::string rutaPartidos;
    std::string rutaRegiones;
    std::string rutaCiudades;
    std::string rutaCandidatos;
    std::string rutaFormulas;
    bool recargarDesdeArchivosInterno();
    bool actualizarArchivosDesdeMemoria();

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
    void simularSegundaVueltaPresidencial();

    // ---- REPORTES ----
    void generarReporteCiudades(const std::string& rutaSalida);
    void generarReporteRegiones(const std::string& rutaSalida);
    void generarReporteNacional(const std::string& rutaSalida);

    // ---- GETTERS ----
    MultilistaRegiones* getRegiones() const;
    AVLCiudades* getArbolCiudades() const;
    AVLCandidatos* getArbolCandidatos() const;
    const Partido* getPartidos() const;

    // reinicia estructuras para escenarios de prueba
    void reiniciarEstructuras();

    // ---- CONSULTAS COMPLEMENTARIAS ----
    void mostrarTarjetonAlcaldia(const std::string& nombreCiudad);
    void mostrarTarjetonPresidencial() const;
    void listarCandidatosMunicipalesPorPartido(const std::string& nombrePartido,
                                               const std::string& nombreRegion = "",
                                               const std::string& nombreCiudad = "");
    void listarCandidatosPresidenciales() const;
    void mostrarCandidatosCiudadPorPartido(const std::string& nombreCiudad);
    void mostrarCensoElectoral() const;
    void reporteGeneroPresidencialPorPartido() const;
    void reporteGeneralMunicipalPorRegion() const;
    void reporteDetalladoMunicipalPorRegion() const;
    void reporteMunicipalTotalesNacionales() const;
    void reporteGeneralPresidencialPorRegion() const;
    void reporteGeneralPresidencialNacional() const;
    void reporteDetalladoPresidencialPorDepartamento() const;
    void reporteDetalladoPresidencialNacional() const;
    void mostrarEstructuraDatos() const;
    void agregarCandidatoAlcaldia();
    void eliminarCandidato();
    void modificarCandidato();
    void guardarCambios();
    bool puedeEditar() const;
    bool simulacionEjecutada() const;
    long long getTotalVotosPresidenciales() const;
    long long getBlancosPresidenciales() const;
    long long getNulosPresidenciales() const;
    long long getAbstencionPresidencial() const;
};

#endif
