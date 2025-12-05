#ifndef REPORTES_H
#define REPORTES_H

#include <string>

class MultilistaRegiones;
class AVLCiudades;
class AVLCandidatos;
class Duo;
class Partido;

namespace Reportes {

void generarReporteCiudades(MultilistaRegiones* regiones, const std::string& rutaSalida);
void generarReporteRegiones(MultilistaRegiones* regiones, const std::string& rutaSalida);
void generarReporteNacional(MultilistaRegiones* regiones,
                            const Duo formulas[5],
                            const std::string& rutaSalida);

void reporteGeneroPresidencialPorPartido(const Partido partidos[5], const Duo formulas[5]);
void mostrarEstructuraDatos(MultilistaRegiones* regiones,
                            AVLCiudades* arbolCiudades,
                            AVLCandidatos* arbolCandidatos);

}  // namespace Reportes

#endif
