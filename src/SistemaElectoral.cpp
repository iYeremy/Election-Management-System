#include "SistemaElectoral.h"

#include "MultilistaRegiones.h"
#include "AVLCiudades.h"
#include "AVLCandidatos.h"
#include "Region.h"
#include "Ciudad.h"
#include "Candidato.h"
#include "Partido.h"
#include "Duo.h"
#include "NodoCandidato.h"
#include "Fecha.h"

// constructor: prepara las estructuras base y arreglos fijos
SistemaElectoral::SistemaElectoral()
    : regiones(new MultilistaRegiones()),
      arbolCiudades(new AVLCiudades()),
      arbolCandidatos(new AVLCandidatos())
{
    // inicializar los arreglos fijos
    for (int i = 0; i < 5; i++) {
        partidos[i] = Partido();
        formulas[i] = Duo();
    }
}

// libera las estructuras principales
SistemaElectoral::~SistemaElectoral() {
    delete regiones;
    delete arbolCiudades;
    delete arbolCandidatos;
}

// -----------------------------
// CARGA DE DATOS
// -----------------------------
void SistemaElectoral::cargarRegiones(const std::string& ruta) {
    
}

void SistemaElectoral::cargarCiudades(const std::string& ruta) {
    
}

void SistemaElectoral::cargarPartidos(const std::string& ruta) {
    
}

void SistemaElectoral::cargarCandidatosAlcaldia(const std::string& ruta) {
    
}

void SistemaElectoral::cargarCandidatosPresidencia(const std::string& ruta) {
    
}

void SistemaElectoral::cargarFormulasPresidenciales(const std::string& ruta) {
    
}

// -----------------------------
// CONSULTAS
// -----------------------------
Ciudad* SistemaElectoral::buscarCiudad(const std::string& nombre) {
    if (!arbolCiudades) {
        return nullptr;
    }
    return arbolCiudades->buscar(nombre);
}

Candidato* SistemaElectoral::buscarCandidato(long id) {
    if (!arbolCandidatos) {
        return nullptr;
    }
    return arbolCandidatos->buscar(id);
}

// -----------------------------
// PROCESOS
// -----------------------------
void SistemaElectoral::simularVotacion() {
    
}

void SistemaElectoral::calcularGanadoresMunicipales() {
    
}

void SistemaElectoral::calcularGanadoresRegionales() {
    
}

void SistemaElectoral::calcularGanadorPresidencial() {
    
}

// -----------------------------
// REPORTES
// -----------------------------
void SistemaElectoral::generarReporteCiudades(const std::string& rutaSalida) {
    
}

void SistemaElectoral::generarReporteRegiones(const std::string& rutaSalida) {
    
}

void SistemaElectoral::generarReporteNacional(const std::string& rutaSalida) {
    
}

// -----------------------------
// DEMOSTRACION
// -----------------------------
void SistemaElectoral::cargarDatosDemostracion() {
    reiniciarEstructuras();

    Region* regionAndina = new Region("Region Andina", 0);
    Region* regionCaribe = new Region("Region Caribe", 0);
    regiones->insertarRegion(regionCaribe);
    regiones->insertarRegion(regionAndina);

    Ciudad* bogota = new Ciudad("Bogota", 1200);
    Ciudad* medellin = new Ciudad("Medellin", 800);
    Ciudad* barranquilla = new Ciudad("Barranquilla", 650);

    regionAndina->agregarCiudad(bogota);
    regionAndina->agregarCiudad(medellin);
    regionCaribe->agregarCiudad(barranquilla);

    arbolCiudades->insertar(bogota);
    arbolCiudades->insertar(medellin);
    arbolCiudades->insertar(barranquilla);

    partidos[0].setNombre("Renovacion Urbana");
    partidos[0].setRepresentante("Laura Diaz");
    partidos[1].setNombre("Union Caribe");
    partidos[1].setRepresentante("Mateo Suarez");

    NodoCandidato* nodoAna = new NodoCandidato();
    Candidato* ana = new Candidato("Ana",
                                   "Rojas",
                                   101001,
                                   'F',
                                   Fecha(12, 5, 1980),
                                   "Bogota",
                                   bogota,
                                   &partidos[0],
                                   Candidato::Tipo::ALCALDIA,
                                   nodoAna);
    bogota->agregarCandidatoAlcaldia(ana);
    partidos[0].agregarCandidatoAlcaldia(ana);
    arbolCandidatos->insertar(ana);

    NodoCandidato* nodoDiego = new NodoCandidato();
    Candidato* diego = new Candidato("Diego",
                                     "Torres",
                                     101002,
                                     'M',
                                     Fecha(3, 9, 1978),
                                     "Bogota",
                                     bogota,
                                     &partidos[1],
                                     Candidato::Tipo::ALCALDIA,
                                     nodoDiego);
    bogota->agregarCandidatoAlcaldia(diego);
    partidos[1].agregarCandidatoAlcaldia(diego);
    arbolCandidatos->insertar(diego);

    NodoCandidato* nodoLucia = new NodoCandidato();
    Candidato* lucia = new Candidato("Lucia",
                                     "Mejia",
                                     101003,
                                     'F',
                                     Fecha(27, 1, 1985),
                                     "Medellin",
                                     medellin,
                                     &partidos[0],
                                     Candidato::Tipo::ALCALDIA,
                                     nodoLucia);
    medellin->agregarCandidatoAlcaldia(lucia);
    partidos[0].agregarCandidatoAlcaldia(lucia);
    arbolCandidatos->insertar(lucia);

    NodoCandidato* nodoMarco = new NodoCandidato();
    Candidato* marco = new Candidato("Marco",
                                     "Lopez",
                                     101004,
                                     'M',
                                     Fecha(2, 11, 1982),
                                     "Medellin",
                                     medellin,
                                     &partidos[1],
                                     Candidato::Tipo::ALCALDIA,
                                     nodoMarco);
    medellin->agregarCandidatoAlcaldia(marco);
    partidos[1].agregarCandidatoAlcaldia(marco);
    arbolCandidatos->insertar(marco);

    NodoCandidato* nodoElena = new NodoCandidato();
    Candidato* elena = new Candidato("Elena",
                                     "Pardo",
                                     101005,
                                     'F',
                                     Fecha(15, 8, 1987),
                                     "Barranquilla",
                                     barranquilla,
                                     &partidos[1],
                                     Candidato::Tipo::ALCALDIA,
                                     nodoElena);
    barranquilla->agregarCandidatoAlcaldia(elena);
    partidos[1].agregarCandidatoAlcaldia(elena);
    arbolCandidatos->insertar(elena);

    NodoCandidato* nodoCarlos = new NodoCandidato();
    Candidato* carlos = new Candidato("Carlos",
                                      "Perez",
                                      201001,
                                      'M',
                                      Fecha(20, 6, 1970),
                                      "Bogota",
                                      nullptr,
                                      &partidos[0],
                                      Candidato::Tipo::PRESIDENCIA,
                                      nodoCarlos);
    arbolCandidatos->insertar(carlos);

    NodoCandidato* nodoJulia = new NodoCandidato();
    Candidato* julia = new Candidato("Julia",
                                     "Marin",
                                     201002,
                                     'F',
                                     Fecha(4, 2, 1975),
                                     "Cali",
                                     nullptr,
                                     &partidos[0],
                                     Candidato::Tipo::PRESIDENCIA,
                                     nodoJulia);
    arbolCandidatos->insertar(julia);

    formulas[0].setPresidente(carlos);
    formulas[0].setVicepresidente(julia);
    formulas[0].setNumeroPartido(1);
    partidos[0].asignarFormulaPresidencial(&formulas[0]);

    NodoCandidato* nodoMateo = new NodoCandidato();
    Candidato* mateo = new Candidato("Mateo",
                                     "Suarez",
                                     201003,
                                     'M',
                                     Fecha(8, 3, 1972),
                                     "Barranquilla",
                                     nullptr,
                                     &partidos[1],
                                     Candidato::Tipo::PRESIDENCIA,
                                     nodoMateo);
    arbolCandidatos->insertar(mateo);

    NodoCandidato* nodoLaura = new NodoCandidato();
    Candidato* laura = new Candidato("Laura",
                                     "Herrera",
                                     201004,
                                     'F',
                                     Fecha(9, 12, 1976),
                                     "Cartagena",
                                     nullptr,
                                     &partidos[1],
                                     Candidato::Tipo::PRESIDENCIA,
                                     nodoLaura);
    arbolCandidatos->insertar(laura);

    formulas[1].setPresidente(mateo);
    formulas[1].setVicepresidente(laura);
    formulas[1].setNumeroPartido(2);
    partidos[1].asignarFormulaPresidencial(&formulas[1]);
}

// -----------------------------
// GETTERS
// -----------------------------
MultilistaRegiones* SistemaElectoral::getRegiones() const {
    return regiones;
}

AVLCiudades* SistemaElectoral::getArbolCiudades() const {
    return arbolCiudades;
}

AVLCandidatos* SistemaElectoral::getArbolCandidatos() const {
    return arbolCandidatos;
}

void SistemaElectoral::reiniciarEstructuras() {
    if (regiones) {
        delete regiones;
    }
    if (arbolCiudades) {
        delete arbolCiudades;
    }
    if (arbolCandidatos) {
        delete arbolCandidatos;
    }

    regiones = new MultilistaRegiones();
    arbolCiudades = new AVLCiudades();
    arbolCandidatos = new AVLCandidatos();

    for (int i = 0; i < 5; i++) {
        partidos[i] = Partido();
        formulas[i] = Duo();
    }
}
