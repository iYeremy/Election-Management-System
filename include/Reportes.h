#ifndef REPORTES_H
#define REPORTES_H

#include <string>

class MultilistaRegiones;
class AVLCiudades;
class AVLCandidatos;
class Duo;
class Partido;

namespace Reportes {

void generarReporteCiudades(MultilistaRegiones* regiones,
                            const Duo formulas[5],
                            const std::string& rutaSalida);
void generarReporteRegiones(MultilistaRegiones* regiones, const std::string& rutaSalida);
void generarReporteNacional(MultilistaRegiones* regiones,
                            const Duo formulas[5],
                            const std::string& rutaSalida);
void generarReporteGeneralMunicipalPorRegion(MultilistaRegiones* regiones, const std::string& rutaSalida);
void generarReporteDetalladoMunicipalPorRegion(MultilistaRegiones* regiones,
                                               const Partido partidos[5],
                                               const std::string& rutaSalida);
void generarReporteMunicipalTotalesNacionales(MultilistaRegiones* regiones,
                                              const Partido partidos[5],
                                              const std::string& rutaSalida);

void reporteGeneroPresidencialPorPartido(const Partido partidos[5],
                                         const Duo formulas[5],
                                         const std::string& rutaSalida);
void reporteGeneralPresidencialPorRegion(MultilistaRegiones* regiones, const std::string& rutaSalida);
void reporteGeneralPresidencialNacional(MultilistaRegiones* regiones,
                                        const Duo formulas[5],
                                        const std::string& rutaSalida);
void reporteDetalladoPresidencialPorDepartamento(MultilistaRegiones* regiones,
                                                 const Partido partidos[5],
                                                 const Duo formulas[5],
                                                 const std::string& rutaSalida);
void reporteDetalladoPresidencialNacional(MultilistaRegiones* regiones,
                                          const Partido partidos[5],
                                          const Duo formulas[5],
                                          const std::string& rutaSalida);
void mostrarEstructuraDatos(MultilistaRegiones* regiones,
                            AVLCiudades* arbolCiudades,
                            AVLCandidatos* arbolCandidatos);

}  // namespace Reportes

#endif
