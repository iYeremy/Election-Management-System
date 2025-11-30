#include "SistemaElectoral.h"

#include "MultilistaRegiones.h"
// #include "AVL_Ciudades.h"
// #include "AVL_Candidatos.h"
#include "Region.h"
#include "Ciudad.h"
#include "Candidato.h"
#include "Partido.h"
#include "Duo.h"

SistemaElectoral::SistemaElectoral()
    : regiones(new MultilistaRegiones()),
      arbolCiudades(nullptr),
      arbolCandidatos(nullptr)
{
    // Inicializar los arreglos fijos
    for (int i = 0; i < 5; i++) {
        partidos[i] = Partido();
        formulas[i] = Duo();
    }
}

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

AVL_Ciudades* SistemaElectoral::getArbolCiudades() const {
    return arbolCiudades;
}

AVL_Candidatos* SistemaElectoral::getArbolCandidatos() const {
    return arbolCandidatos;
}
