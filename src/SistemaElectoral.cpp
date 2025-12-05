#include "SistemaElectoral.h"

#include <iostream>
#include <iomanip>

#include "MultilistaRegiones.h"
#include "AVLCiudades.h"
#include "AVLCandidatos.h"
#include "Region.h"
#include "Ciudad.h"
#include "Candidato.h"
#include "Partido.h"
#include "Duo.h"
#include "SistemaElectoral_Util.h"
#include "Reportes.h"

using namespace SistemaElectoralInterno;

SistemaElectoral::SistemaElectoral()
    : regiones(new MultilistaRegiones()),
      arbolCiudades(new AVLCiudades()),
      arbolCandidatos(new AVLCandidatos()),
      segundaVueltaPendiente(false),
      segundaVueltaRealizada(false),
      blancosSegundaVuelta(0),
      nulosSegundaVuelta(0),
      abstencionSegundaVuelta(0),
      totalVotosPresidenciales(0),
      blancosPresidenciales(0),
      nulosPresidenciales(0),
      abstencionPresidencial(0),
      simulacionEnCurso(false),
      simulacionEjecutadaFlag(false),
      rutaPartidos(),
      rutaRegiones(),
      rutaCiudades(),
      rutaCandidatos(),
      rutaFormulas() {
    for (int i = 0; i < 5; i++) {
        partidos[i] = Partido();
        formulas[i] = Duo();
    }
    indicesSegundaVuelta[0] = indicesSegundaVuelta[1] = -1;
    votosSegundaVuelta[0] = votosSegundaVuelta[1] = 0;
}

SistemaElectoral::~SistemaElectoral() {
    delete regiones;
    delete arbolCiudades;
    delete arbolCandidatos;
}

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

void SistemaElectoral::generarReporteCiudades(const std::string& rutaSalida) {
    Reportes::generarReporteCiudades(regiones, formulas, rutaSalida);
}

void SistemaElectoral::generarReporteRegiones(const std::string& rutaSalida) {
    Reportes::generarReporteRegiones(regiones, rutaSalida);
}

void SistemaElectoral::generarReporteNacional(const std::string& rutaSalida) {
    Reportes::generarReporteNacional(regiones, formulas, rutaSalida);
}

void SistemaElectoral::reporteGeneralMunicipalPorRegion() const {
    Reportes::generarReporteGeneralMunicipalPorRegion(regiones, "reportes/reporte_general_municipal.txt");
}

void SistemaElectoral::reporteDetalladoMunicipalPorRegion() const {
    Reportes::generarReporteDetalladoMunicipalPorRegion(regiones, partidos, "reportes/reporte_detallado_municipal.txt");
}

void SistemaElectoral::reporteMunicipalTotalesNacionales() const {
    Reportes::generarReporteMunicipalTotalesNacionales(regiones, partidos, "reportes/reporte_municipal_totales.txt");
}

void SistemaElectoral::reporteGeneralPresidencialPorRegion() const {
    Reportes::reporteGeneralPresidencialPorRegion(regiones, "reportes/reporte_presidencial_general_region.txt");
}

void SistemaElectoral::reporteGeneralPresidencialNacional() const {
    Reportes::reporteGeneralPresidencialNacional(
        regiones, formulas, "reportes/reporte_presidencial_general_nacional.txt");
}

void SistemaElectoral::reporteDetalladoPresidencialPorDepartamento() const {
    Reportes::reporteDetalladoPresidencialPorDepartamento(
        regiones, partidos, formulas, "reportes/reporte_presidencial_detallado_departamento.txt");
}

void SistemaElectoral::reporteDetalladoPresidencialNacional() const {
    Reportes::reporteDetalladoPresidencialNacional(
        regiones, partidos, formulas, "reportes/reporte_presidencial_detallado_nacional.txt");
}

MultilistaRegiones* SistemaElectoral::getRegiones() const {
    return regiones;
}

AVLCiudades* SistemaElectoral::getArbolCiudades() const {
    return arbolCiudades;
}

AVLCandidatos* SistemaElectoral::getArbolCandidatos() const {
    return arbolCandidatos;
}

const Partido* SistemaElectoral::getPartidos() const {
    return partidos;
}

void SistemaElectoral::reiniciarEstructuras() {
    delete regiones;
    delete arbolCiudades;
    delete arbolCandidatos;

    regiones = new MultilistaRegiones();
    arbolCiudades = new AVLCiudades();
    arbolCandidatos = new AVLCandidatos();

    for (int i = 0; i < 5; i++) {
        partidos[i] = Partido();
        formulas[i] = Duo();
    }

    segundaVueltaPendiente = false;
    segundaVueltaRealizada = false;
    indicesSegundaVuelta[0] = indicesSegundaVuelta[1] = -1;
    votosSegundaVuelta[0] = votosSegundaVuelta[1] = 0;
    blancosSegundaVuelta = nulosSegundaVuelta = abstencionSegundaVuelta = 0;
    totalVotosPresidenciales = 0;
    blancosPresidenciales = 0;
    nulosPresidenciales = 0;
    abstencionPresidencial = 0;
    simulacionEnCurso = false;
    simulacionEjecutadaFlag = false;
}
