#include "SistemaElectoral.h"

#include "MultilistaRegiones.h"
#include "AVLCiudades.h"
#include "AVLCandidatos.h"
#include "Region.h"
#include "Ciudad.h"
#include "Candidato.h"
#include "Partido.h"
#include "Duo.h"

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
    //buscar en AVL Ciudades
    return nullptr;
}

Candidato* SistemaElectoral::buscarCandidato(long id) {
    //buscar en AVL Candidatos
    return nullptr;
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
