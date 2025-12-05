#include "Reportes.h"

#include <fstream>
#include <iostream>
#include <cctype>
#include <iomanip>

#include "MultilistaRegiones.h"
#include "Region.h"
#include "Ciudad.h"
#include "Candidato.h"
#include "Partido.h"
#include "Duo.h"
#include "AVLCiudades.h"
#include "AVLCandidatos.h"
#include "NodoCandidato.h"
#include "CiudadHelper.h"

namespace Reportes {

namespace {
void asegurarCarpeta(const std::string&) {
    // Se asume que las carpetas de salida ya existen.
}
}

void generarReporteCiudades(MultilistaRegiones* regiones,
                            const Duo formulas[5],
                            const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay informacion cargada para el reporte de ciudades.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    archivo << "    REPORTE DETALLADO DE CIUDADES\n\n";

    Region* region = regiones->getCabeza();
    while (region) {
        archivo << "\n--- REGION: " << region->getNombre() << " ---\n\n";

        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            archivo << "Ciudad: " << ciudad->getNombre() << "\n";
            archivo << "  Censo Electoral: " << ciudad->getCenso() << "\n";
            archivo << "  Total Votos: " << ciudad->totalVotos() << "\n";
            archivo << "  Votos Validos: " << ciudad->totalVotosValidos() << "\n";
            archivo << "  Votos en Blanco: " << ciudad->getVotosBlanco() << "\n";
            archivo << "  Votos Nulos: " << ciudad->getVotosNulos() << "\n";
            archivo << "  Abstencion: " << ciudad->getAbstencion() << "\n";

            int indiceGanador = ciudad->ganadorAlcaldia();
            if (indiceGanador != -1) {
                Candidato* ganador = candidatoMunicipalPorIndice(ciudad, indiceGanador);
                if (ganador) {
                    const int* votos = ciudad->getVotosCandidato();
                    archivo << "  Ganador: " << ganador->getNombre() << " "
                            << ganador->getApellido() << " con "
                            << votos[indiceGanador] << " votos\n";
                }
            }

            int totalPres = ciudad->totalVotosPresidencialesCiudad();
            if (totalPres == 0) {
                archivo << "  Resultados presidenciales: sin votos registrados.\n";
            } else {
                const int* votosPres = ciudad->getVotosPresidenciales();
                archivo << "  Resultados presidenciales:\n";
                std::streamsize precisionAnterior = archivo.precision();
                std::ios::fmtflags flagsAnteriores = archivo.flags();
                archivo << std::fixed << std::setprecision(2);
                for (int i = 0; i < 5; ++i) {
                    const Duo& formula = formulas[i];
                    Candidato* presidente = formula.getPresidente();
                    Candidato* vice = formula.getVicepresidente();
                    if (!presidente || !vice) {
                        continue;
                    }
                    archivo << "    - " << presidente->getNombre() << " " << presidente->getApellido()
                            << " / " << vice->getNombre() << " " << vice->getApellido()
                            << ": " << votosPres[i] << " votos (" << ciudad->porcentajeVotoPresidencial(i) << "%)\n";
                }
                archivo << "    Blancos: " << ciudad->getVotosPresidencialesBlanco()
                        << " (" << ciudad->porcentajePresidencialBlanco() << "%)\n";
                archivo << "    Nulos: " << ciudad->getVotosPresidencialesNulos()
                        << " (" << ciudad->porcentajePresidencialNulo() << "%)\n";
                archivo << "    Abstencion: " << ciudad->getAbstencionPresidencial()
                        << " (" << ciudad->porcentajePresidencialAbstencion() << "%)\n";
                int ganadorPres = ciudad->ganadorPresidencialCiudad();
                if (ganadorPres != -1) {
                    const Duo& formulaGanadora = formulas[ganadorPres];
                    Candidato* presidente = formulaGanadora.getPresidente();
                    Candidato* vice = formulaGanadora.getVicepresidente();
                    if (presidente && vice) {
                        archivo << "    Ganador presidencial: "
                                << presidente->getNombre() << " " << presidente->getApellido()
                                << " con " << votosPres[ganadorPres] << " votos.\n";
                    }
                }
                archivo.flags(flagsAnteriores);
                archivo.precision(precisionAnterior);
            }

            archivo << "\n";
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }

    std::cout << "Reporte de ciudades generado en " << rutaSalida << std::endl;
}

void generarReporteRegiones(MultilistaRegiones* regiones, const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones registradas para reportar.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    archivo << "    REPORTE RESUMIDO DE REGIONES\n\n";

    Region* region = regiones->getCabeza();
    int totalRegiones = 0;

    while (region) {
        totalRegiones++;
        archivo << "Region: " << region->getNombre() << "\n";
        archivo << "  Censo total declarado: " << region->getCensoTotal() << "\n";

        long long votosProcesados = 0;
        long long votosValidos = 0;
        long long blancos = 0;
        long long nulos = 0;
        long long abstenciones = 0;
        int ciudadesReportadas = 0;

        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            ciudadesReportadas++;
            votosProcesados += ciudad->totalVotos();
            votosValidos += ciudad->totalVotosValidos();
            blancos += ciudad->getVotosBlanco();
            nulos += ciudad->getVotosNulos();
            abstenciones += ciudad->getAbstencion();
            ciudad = ciudad->getSigCiudad();
        }

        archivo << "  Ciudades reportadas: " << ciudadesReportadas << "\n";
        archivo << "  Votos procesados: " << votosProcesados << "\n";
        archivo << "  Votos validos: " << votosValidos << "\n";
        archivo << "  Blancos: " << blancos << "\n";
        archivo << "  Nulos: " << nulos << "\n";
        archivo << "  Abstencion: " << abstenciones << "\n";

        archivo << "  Detalle de ciudades:\n";
        ciudad = region->getListaCiudades();
        while (ciudad) {
            archivo << "    * " << ciudad->getNombre()
                    << " - Total votos: " << ciudad->totalVotos() << "\n";
            ciudad = ciudad->getSigCiudad();
        }

        archivo << "\n";
        region = region->getSigRegion();
    }

    archivo << "Total de regiones reportadas: " << totalRegiones << "\n";
    std::cout << "Reporte de regiones generado en " << rutaSalida << std::endl;
}

void generarReporteNacional(MultilistaRegiones* regiones,
                            const Duo formulas[5],
                            const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay informacion cargada para el reporte nacional.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    archivo << "    REPORTE NACIONAL\n\n";

    Region* region = regiones->getCabeza();
    long long censoTotal = 0;
    long long votosProcesados = 0;
    long long votosValidos = 0;
    long long blanco = 0;
    long long nulo = 0;
    long long abstencion = 0;
    int regionesProcesadas = 0;

    while (region) {
        regionesProcesadas++;
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            censoTotal += ciudad->getCenso();
            votosProcesados += ciudad->totalVotos();
            votosValidos += ciudad->totalVotosValidos();
            blanco += ciudad->getVotosBlanco();
            nulo += ciudad->getVotosNulos();
            abstencion += ciudad->getAbstencion();
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }

    archivo << "Regiones procesadas: " << regionesProcesadas << "\n";
    archivo << "Censo nacional aproximado: " << censoTotal << "\n";
    archivo << "Votos procesados: " << votosProcesados << "\n";
    archivo << "Votos validos: " << votosValidos << "\n";
    archivo << "Votos en blanco: " << blanco << "\n";
    archivo << "Votos nulos: " << nulo << "\n";
    archivo << "Abstencion: " << abstencion << "\n\n";

    archivo << "Resultados presidenciales:\n";
    for (int i = 0; i < 5; ++i) {
        const Duo& formula = formulas[i];
        Candidato* presidente = formula.getPresidente();
        Candidato* vicepresidente = formula.getVicepresidente();
        if (!presidente || !vicepresidente) {
            continue;
        }

        archivo << "  Formula " << (i + 1) << ": "
                << presidente->getNombre() << " " << presidente->getApellido()
                << " + " << vicepresidente->getNombre() << " " << vicepresidente->getApellido()
                << " -> votos: " << formula.getVotos() << "\n";
    }

    std::cout << "Reporte nacional generado en " << rutaSalida << std::endl;
}

void generarReporteGeneralMunicipalPorRegion(MultilistaRegiones* regiones, const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones cargadas para el reporte general municipal.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    archivo << "REPORTE GENERAL MUNICIPAL POR REGION\n\n";
    archivo << std::fixed << std::setprecision(2);

    Region* region = regiones->getCabeza();
    while (region) {
        long long totalVotos = 0;
        long long blancos = 0;
        long long nulos = 0;
        long long abstencion = 0;

        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            totalVotos += ciudad->totalVotos();
            blancos += ciudad->getVotosBlanco();
            nulos += ciudad->getVotosNulos();
            abstencion += ciudad->getAbstencion();
            ciudad = ciudad->getSigCiudad();
        }

        auto porcentaje = [&](long long valor) -> double {
            return totalVotos > 0 ? (static_cast<double>(valor) * 100.0) / totalVotos : 0.0;
        };

        archivo << "Region: " << region->getNombre() << "\n";
        archivo << "  Total votos: " << totalVotos << " (" << (totalVotos > 0 ? 100.0 : 0.0) << "%)\n";
        archivo << "  Votos en blanco: " << blancos << " (" << porcentaje(blancos) << "%)\n";
        archivo << "  Votos nulos: " << nulos << " (" << porcentaje(nulos) << "%)\n";
        archivo << "  Abstencion: " << abstencion << " (" << porcentaje(abstencion) << "%)\n\n";

        region = region->getSigRegion();
    }

    std::cout << "Reporte general municipal generado en " << rutaSalida << std::endl;
}

void generarReporteDetalladoMunicipalPorRegion(MultilistaRegiones* regiones,
                                               const Partido partidos[5],
                                               const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay informacion cargada para el reporte detallado municipal.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    archivo << "REPORTE DETALLADO MUNICIPAL POR REGION\n\n";

    Region* region = regiones->getCabeza();
    while (region) {
        long long ganadoresPorPartido[5] = {0, 0, 0, 0, 0};
        long long ganadoresIndependientes = 0;
        long long totalGanadores = 0;
        long long hombres = 0;
        long long mujeres = 0;

        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            int indice = ciudad->ganadorAlcaldia();
            if (indice >= 0) {
                Candidato* ganador = candidatoMunicipalPorIndice(ciudad, indice);
                if (ganador) {
                    ++totalGanadores;
                    char sexo = static_cast<char>(std::toupper(static_cast<unsigned char>(ganador->getSexo())));
                    if (sexo == 'F') {
                        ++mujeres;
                    } else {
                        ++hombres;
                    }
                    Partido* partido = ganador->getPartido();
                    int idxPartido = indicePartido(partidos, partido);
                    if (idxPartido >= 0) {
                        ++ganadoresPorPartido[idxPartido];
                    } else {
                        ++ganadoresIndependientes;
                    }
                }
            }
            ciudad = ciudad->getSigCiudad();
        }

        auto porcentaje = [&](long long valor) -> double {
            return totalGanadores > 0 ? (static_cast<double>(valor) * 100.0) / totalGanadores : 0.0;
        };

        archivo << "Region: " << region->getNombre() << "\n";
        archivo << "  Total alcaldes definidos: " << totalGanadores << "\n";

        for (int i = 0; i < 5; ++i) {
            if (partidos[i].getNombre().empty()) {
                continue;
            }
            long long cantidad = ganadoresPorPartido[i];
            archivo << "    Partido " << partidos[i].getNombre() << ": " << cantidad
                    << " (" << std::fixed << std::setprecision(2) << porcentaje(cantidad) << "%)\n";
        }
        archivo << "    Independientes/Otros: " << ganadoresIndependientes
                << " (" << porcentaje(ganadoresIndependientes) << "%)\n";

        archivo << "  Totales por genero:\n";
        archivo << "    Hombres: " << hombres << " (" << porcentaje(hombres) << "%)\n";
        archivo << "    Mujeres: " << mujeres << " (" << porcentaje(mujeres) << "%)\n\n";

        region = region->getSigRegion();
    }

    std::cout << "Reporte detallado municipal generado en " << rutaSalida << std::endl;
}

void generarReporteMunicipalTotalesNacionales(MultilistaRegiones* regiones,
                                              const Partido partidos[5],
                                              const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones cargadas para el consolidado municipal.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    long long ganadoresPorPartido[5] = {0, 0, 0, 0, 0};
    long long ganadoresIndependientes = 0;
    long long totalGanadores = 0;
    long long hombres = 0;
    long long mujeres = 0;

    Region* region = regiones->getCabeza();
    while (region) {
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            int indice = ciudad->ganadorAlcaldia();
            if (indice >= 0) {
                Candidato* ganador = candidatoMunicipalPorIndice(ciudad, indice);
                if (ganador) {
                    ++totalGanadores;
                    char sexo = static_cast<char>(std::toupper(static_cast<unsigned char>(ganador->getSexo())));
                    if (sexo == 'F') {
                        ++mujeres;
                    } else {
                        ++hombres;
                    }
                    Partido* partido = ganador->getPartido();
                    int idxPartido = indicePartido(partidos, partido);
                    if (idxPartido >= 0) {
                        ++ganadoresPorPartido[idxPartido];
                    } else {
                        ++ganadoresIndependientes;
                    }
                }
            }
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }

    auto porcentaje = [&](long long valor) -> double {
        return totalGanadores > 0 ? (static_cast<double>(valor) * 100.0) / totalGanadores : 0.0;
    };

    archivo << "CONSOLIDADO NACIONAL DE ALCALDIAS\n\n";
    archivo << "Total alcaldes definidos: " << totalGanadores << "\n\n";

    archivo << "Totales por partido:\n";
    for (int i = 0; i < 5; ++i) {
        if (partidos[i].getNombre().empty()) {
            continue;
        }
        long long cantidad = ganadoresPorPartido[i];
        archivo << "  " << partidos[i].getNombre() << ": " << cantidad
                << " (" << std::fixed << std::setprecision(2) << porcentaje(cantidad) << "%)\n";
    }
    archivo << "  Independientes/Otros: " << ganadoresIndependientes
            << " (" << porcentaje(ganadoresIndependientes) << "%)\n\n";

    archivo << "Totales por genero:\n";
    archivo << "  Hombres: " << hombres << " (" << porcentaje(hombres) << "%)\n";
    archivo << "  Mujeres: " << mujeres << " (" << porcentaje(mujeres) << "%)\n";

    std::cout << "Reporte municipal nacional generado en " << rutaSalida << std::endl;
}


void reporteGeneroPresidencialPorPartido(const Partido partidos[5],
                                         const Duo formulas[5],
                                         const std::string& rutaSalida) {
    struct ConteoGenero {
        long long hombres = 0;
        long long mujeres = 0;
    };

    ConteoGenero conteos[5];

    auto registrar = [&](Candidato* candidato) {
        if (!candidato) {
            return;
        }
        Partido* partido = candidato->getPartido();
        int indice = indicePartido(partidos, partido);
        if (indice < 0) {
            return;
        }
        char sexo = static_cast<char>(std::toupper(static_cast<unsigned char>(candidato->getSexo())));
        if (sexo == 'F') {
            conteos[indice].mujeres++;
        } else {
            conteos[indice].hombres++;
        }
    };

    for (int i = 0; i < 5; ++i) {
        registrar(formulas[i].getPresidente());
        registrar(formulas[i].getVicepresidente());
    }

    long long totalParticipantes = 0;
    long long totalHombres = 0;
    long long totalMujeres = 0;
    for (int i = 0; i < 5; ++i) {
        totalParticipantes += conteos[i].hombres + conteos[i].mujeres;
        totalHombres += conteos[i].hombres;
        totalMujeres += conteos[i].mujeres;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    archivo << "REPORTE GENERO VS PARTIDO (FORMULAS PRESIDENCIALES)\n\n";
    archivo << std::fixed << std::setprecision(2);
    for (int i = 0; i < 5; ++i) {
        if (partidos[i].getNombre().empty()) {
            continue;
        }
        long long total = conteos[i].hombres + conteos[i].mujeres;
        if (total == 0) {
            archivo << "Partido: " << partidos[i].getNombre() << "\n";
            archivo << "  Sin participantes registrados en formulas.\n\n";
            continue;
        }

        auto porcentajePartido = [&](long long valor) -> double {
            return total > 0 ? (static_cast<double>(valor) * 100.0) / total : 0.0;
        };
        double porcentajeNacional = totalParticipantes > 0
                                        ? (static_cast<double>(total) * 100.0) / totalParticipantes
                                        : 0.0;

        archivo << "Partido: " << partidos[i].getNombre() << "\n";
        archivo << "  Total participantes: " << total << " (" << porcentajeNacional << "% del total nacional)\n";
        archivo << "  Hombres: " << conteos[i].hombres << " (" << porcentajePartido(conteos[i].hombres) << "%)\n";
        archivo << "  Mujeres: " << conteos[i].mujeres << " (" << porcentajePartido(conteos[i].mujeres) << "%)\n\n";
    }

    archivo << "Totales nacionales:\n";
    archivo << "  Total participantes: " << totalParticipantes << "\n";
    if (totalParticipantes > 0) {
        archivo << "  Hombres: " << totalHombres << " ("
                << (static_cast<double>(totalHombres) * 100.0) / totalParticipantes << "%)\n";
        archivo << "  Mujeres: " << totalMujeres << " ("
                << (static_cast<double>(totalMujeres) * 100.0) / totalParticipantes << "%)\n";
    }

    std::cout << "\nReporte de genero en formulas presidenciales\n";
    std::cout << "--------------------------------------\n";
    for (int i = 0; i < 5; ++i) {
        if (partidos[i].getNombre().empty()) {
            continue;
        }
        long long total = conteos[i].hombres + conteos[i].mujeres;
        if (total == 0) {
            continue;
        }
        std::cout << partidos[i].getNombre() << ": "
                  << total << " participantes (M "
                  << conteos[i].hombres << ", F " << conteos[i].mujeres << ")\n";
    }
    std::cout << "Reporte almacenado en " << rutaSalida << "\n";
}

namespace {
std::string descripcionFormula(const Duo& formula) {
    std::string etiqueta;
    Candidato* presidente = formula.getPresidente();
    Candidato* vice = formula.getVicepresidente();
    if (presidente) {
        etiqueta = presidente->getNombre() + std::string(" ") + presidente->getApellido();
    } else {
        etiqueta = "Formula #" + std::to_string(formula.getNumeroPartido());
    }
    if (vice) {
        etiqueta += " / " + vice->getNombre() + std::string(" ") + vice->getApellido();
    }
    Partido* partido = formula.getPartido();
    if (partido && !partido->getNombre().empty()) {
        etiqueta += " (" + partido->getNombre() + ")";
    }
    return etiqueta;
}
}  // namespace

void reporteGeneralPresidencialPorRegion(MultilistaRegiones* regiones, const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones cargadas para el reporte presidencial regional.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    archivo << "REPORTE GENERAL PRESIDENCIAL POR REGION\n\n";
    archivo << std::fixed << std::setprecision(2);

    Region* region = regiones->getCabeza();
    while (region) {
        long long total = 0;
        long long blancos = 0;
        long long nulos = 0;
        long long abstencion = 0;

        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            total += ciudad->totalVotosPresidencialesCiudad();
            blancos += ciudad->getVotosPresidencialesBlanco();
            nulos += ciudad->getVotosPresidencialesNulos();
            abstencion += ciudad->getAbstencionPresidencial();
            ciudad = ciudad->getSigCiudad();
        }

        auto porcentaje = [&](long long valor) -> double {
            return total > 0 ? (static_cast<double>(valor) * 100.0) / total : 0.0;
        };

        archivo << "Region: " << region->getNombre() << "\n";
        archivo << "  Total votos: " << total << " (" << (total > 0 ? 100.0 : 0.0) << "%)\n";
        archivo << "  Votos en blanco: " << blancos << " (" << porcentaje(blancos) << "%)\n";
        archivo << "  Votos nulos: " << nulos << " (" << porcentaje(nulos) << "%)\n";
        archivo << "  Abstencion: " << abstencion << " (" << porcentaje(abstencion) << "%)\n\n";

        region = region->getSigRegion();
    }

    std::cout << "Reporte presidencial regional generado en " << rutaSalida << std::endl;
}

void reporteGeneralPresidencialNacional(MultilistaRegiones* regiones,
                                        const Duo formulas[5],
                                        const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay informacion cargada para el reporte presidencial nacional.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    long long total = 0;
    long long blancos = 0;
    long long nulos = 0;
    long long abstencion = 0;
    long long votosFormulas[5] = {0, 0, 0, 0, 0};

    Region* region = regiones->getCabeza();
    while (region) {
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            total += ciudad->totalVotosPresidencialesCiudad();
            blancos += ciudad->getVotosPresidencialesBlanco();
            nulos += ciudad->getVotosPresidencialesNulos();
            abstencion += ciudad->getAbstencionPresidencial();
            const int* votosCiudad = ciudad->getVotosPresidenciales();
            if (votosCiudad) {
                for (int i = 0; i < 5; ++i) {
                    votosFormulas[i] += votosCiudad[i];
                }
            }
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }

    long long totalValidos = 0;
    for (int i = 0; i < 5; ++i) {
        totalValidos += votosFormulas[i];
    }

    auto porcentajeTotal = [&](long long valor) -> double {
        return total > 0 ? (static_cast<double>(valor) * 100.0) / total : 0.0;
    };
    auto porcentajeValidos = [&](long long valor) -> double {
        return totalValidos > 0 ? (static_cast<double>(valor) * 100.0) / totalValidos : 0.0;
    };

    archivo << "REPORTE GENERAL PRESIDENCIAL NACIONAL\n\n";
    archivo << std::fixed << std::setprecision(2);
    archivo << "Total votos: " << total << " (" << (total > 0 ? 100.0 : 0.0) << "%)\n";
    archivo << "Votos en blanco: " << blancos << " (" << porcentajeTotal(blancos) << "%)\n";
    archivo << "Votos nulos: " << nulos << " (" << porcentajeTotal(nulos) << "%)\n";
    archivo << "Abstencion: " << abstencion << " (" << porcentajeTotal(abstencion) << "%)\n\n";

    archivo << "Votos por formula/partido (sobre votos validos):\n";
    for (int i = 0; i < 5; ++i) {
        if (votosFormulas[i] == 0) {
            continue;
        }
        archivo << "  " << descripcionFormula(formulas[i]) << ": " << votosFormulas[i]
                << " (" << porcentajeValidos(votosFormulas[i]) << "%)\n";
    }
    archivo << "\n";

    if (totalValidos == 0) {
        archivo << "No se registran votos validos para determinar segunda vuelta.\n";
        std::cout << "Reporte presidencial nacional generado en " << rutaSalida << std::endl;
        return;
    }

    long long mejorVotos = -1;
    for (int i = 0; i < 5; ++i) {
        if (votosFormulas[i] > mejorVotos) {
            mejorVotos = votosFormulas[i];
        }
    }

    if (mejorVotos <= 0) {
        archivo << "No hay votos para determinar ganadores.\n";
        std::cout << "Reporte presidencial nacional generado en " << rutaSalida << std::endl;
        return;
    }

    int mejores[5];
    int totalMejores = 0;
    for (int i = 0; i < 5; ++i) {
        if (votosFormulas[i] == mejorVotos) {
            mejores[totalMejores++] = i;
        }
    }

    archivo << "Analisis de segunda vuelta:\n";
    if (totalMejores > 1) {
        archivo << "  Existe empate en el primer lugar. Irian a segunda vuelta:\n";
        for (int i = 0; i < totalMejores; ++i) {
            archivo << "    - " << descripcionFormula(formulas[mejores[i]]) << "\n";
        }
    } else {
        int indiceGanador = mejores[0];
        double porcentajeGanador = porcentajeValidos(votosFormulas[indiceGanador]);
        if (porcentajeGanador > 50.0) {
            archivo << "  No se requiere segunda vuelta. Ganador: "
                    << descripcionFormula(formulas[indiceGanador])
                    << " con " << votosFormulas[indiceGanador] << " votos ("
                    << porcentajeGanador << "% de los validos).\n";
        } else {
            long long segundoVotos = -1;
            for (int i = 0; i < 5; ++i) {
                if (i == indiceGanador) {
                    continue;
                }
                if (votosFormulas[i] > segundoVotos) {
                    segundoVotos = votosFormulas[i];
                }
            }
            int segundos[5];
            int totalSegundos = 0;
            for (int i = 0; i < 5; ++i) {
                if (i == indiceGanador) {
                    continue;
                }
                if (votosFormulas[i] == segundoVotos && segundoVotos >= 0) {
                    segundos[totalSegundos++] = i;
                }
            }

            archivo << "  Se requiere segunda vuelta. Participan:\n";
            archivo << "    - " << descripcionFormula(formulas[indiceGanador]) << "\n";
            for (int i = 0; i < totalSegundos; ++i) {
                archivo << "    - " << descripcionFormula(formulas[segundos[i]]) << "\n";
            }
        }
    }

    std::cout << "Reporte presidencial nacional generado en " << rutaSalida << std::endl;
}

void reporteDetalladoPresidencialPorDepartamento(MultilistaRegiones* regiones,
                                                 const Partido partidos[5],
                                                 const Duo formulas[5],
                                                 const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones cargadas para el reporte detallado presidencial.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    archivo << "REPORTE DETALLADO PRESIDENCIAL POR DEPARTAMENTO\n\n";
    archivo << std::fixed << std::setprecision(2);

    Region* region = regiones->getCabeza();
    while (region) {
        long long votosPorPartido[5] = {0, 0, 0, 0, 0};
        long long votosOtros = 0;
        long long hombres = 0;
        long long mujeres = 0;
        long long totalValidos = 0;

        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            const int* votosCiudad = ciudad->getVotosPresidenciales();
            if (votosCiudad) {
                for (int i = 0; i < 5; ++i) {
                    long long votos = votosCiudad[i];
                    if (votos <= 0) {
                        continue;
                    }
                    totalValidos += votos;
                    const Duo& formula = formulas[i];
                    Candidato* presidente = formula.getPresidente();
                    char sexo = presidente ? static_cast<char>(std::toupper(static_cast<unsigned char>(presidente->getSexo())))
                                           : 'M';
                    if (sexo == 'F') {
                        mujeres += votos;
                    } else {
                        hombres += votos;
                    }
                    Partido* partido = formula.getPartido();
                    int idx = indicePartido(partidos, partido);
                    if (idx >= 0) {
                        votosPorPartido[idx] += votos;
                    } else {
                        votosOtros += votos;
                    }
                }
            }
            ciudad = ciudad->getSigCiudad();
        }

        auto porcentaje = [&](long long valor) -> double {
            return totalValidos > 0 ? (static_cast<double>(valor) * 100.0) / totalValidos : 0.0;
        };

        archivo << "Region: " << region->getNombre() << "\n";
        archivo << "  Total votos validos: " << totalValidos << "\n";
        archivo << "  Votos por partido:\n";
        for (int i = 0; i < 5; ++i) {
            if (partidos[i].getNombre().empty()) {
                continue;
            }
            archivo << "    " << partidos[i].getNombre() << ": " << votosPorPartido[i]
                    << " (" << porcentaje(votosPorPartido[i]) << "%)\n";
        }
        if (votosOtros > 0) {
            archivo << "    Otros/Independientes: " << votosOtros
                    << " (" << porcentaje(votosOtros) << "%)\n";
        }

        archivo << "  Votos por genero:\n";
        archivo << "    Hombres: " << hombres << " (" << porcentaje(hombres) << "%)\n";
        archivo << "    Mujeres: " << mujeres << " (" << porcentaje(mujeres) << "%)\n\n";

        region = region->getSigRegion();
    }

    std::cout << "Reporte detallado presidencial regional generado en " << rutaSalida << std::endl;
}

void reporteDetalladoPresidencialNacional(MultilistaRegiones* regiones,
                                          const Partido partidos[5],
                                          const Duo formulas[5],
                                          const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay informacion cargada para el reporte presidencial nacional detallado.\n";
        return;
    }

    asegurarCarpeta(rutaSalida);
    std::ofstream archivo(rutaSalida.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << rutaSalida << std::endl;
        return;
    }

    long long votosPorPartido[5] = {0, 0, 0, 0, 0};
    long long votosOtros = 0;
    long long hombres = 0;
    long long mujeres = 0;
    long long totalValidos = 0;

    Region* region = regiones->getCabeza();
    while (region) {
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            const int* votosCiudad = ciudad->getVotosPresidenciales();
            if (votosCiudad) {
                for (int i = 0; i < 5; ++i) {
                    long long votos = votosCiudad[i];
                    if (votos <= 0) {
                        continue;
                    }
                    totalValidos += votos;
                    const Duo& formula = formulas[i];
                    Candidato* presidente = formula.getPresidente();
                    char sexo = presidente ? static_cast<char>(std::toupper(static_cast<unsigned char>(presidente->getSexo())))
                                           : 'M';
                    if (sexo == 'F') {
                        mujeres += votos;
                    } else {
                        hombres += votos;
                    }
                    Partido* partido = formula.getPartido();
                    int idx = indicePartido(partidos, partido);
                    if (idx >= 0) {
                        votosPorPartido[idx] += votos;
                    } else {
                        votosOtros += votos;
                    }
                }
            }
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }

    auto porcentaje = [&](long long valor) -> double {
        return totalValidos > 0 ? (static_cast<double>(valor) * 100.0) / totalValidos : 0.0;
    };

    archivo << "REPORTE DETALLADO PRESIDENCIAL NACIONAL\n\n";
    archivo << std::fixed << std::setprecision(2);
    archivo << "Total votos validos: " << totalValidos << "\n";
    archivo << "Votos por partido:\n";
    for (int i = 0; i < 5; ++i) {
        if (partidos[i].getNombre().empty()) {
            continue;
        }
        archivo << "  " << partidos[i].getNombre() << ": " << votosPorPartido[i]
                << " (" << porcentaje(votosPorPartido[i]) << "%)\n";
    }
    if (votosOtros > 0) {
        archivo << "  Otros/Independientes: " << votosOtros
                << " (" << porcentaje(votosOtros) << "%)\n";
    }
    archivo << "\nVotos por genero:\n";
    archivo << "  Hombres: " << hombres << " (" << porcentaje(hombres) << "%)\n";
    archivo << "  Mujeres: " << mujeres << " (" << porcentaje(mujeres) << "%)\n";

    std::cout << "Reporte detallado presidencial nacional generado en " << rutaSalida << std::endl;
}

void mostrarEstructuraDatos(MultilistaRegiones* regiones,
                            AVLCiudades* arbolCiudades,
                            AVLCandidatos* arbolCandidatos) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones cargadas para mostrar.\n";
    } else {
        std::cout << "\n[Multilista Regiones -> Ciudades -> Candidatos]\n";
        Region* region = regiones->getCabeza();
        while (region) {
            std::cout << "Region: " << region->getNombre() << "\n";
            Ciudad* ciudad = region->getListaCiudades();
            while (ciudad) {
                std::cout << "  Ciudad: " << ciudad->getNombre() << " (Censo " << ciudad->getCenso() << ")\n";
                NodoCandidato* nodo = ciudad->getCandidatosAlcaldia();
                while (nodo) {
                    Candidato* candidato = nodo->getCandidato();
                    if (candidato) {
                        std::cout << "    - " << candidato->getNombre() << " " << candidato->getApellido() << "\n";
                    }
                    nodo = nodo->getSigCiudad();
                }
                ciudad = ciudad->getSigCiudad();
            }
            region = region->getSigRegion();
        }
    }

    if (arbolCiudades) {
        std::cout << "\n[AVL Ciudades]\n";
        arbolCiudades->imprimir();
    }
    if (arbolCandidatos) {
        std::cout << "\n[AVL Candidatos]\n";
        arbolCandidatos->imprimir();
    }
}

}  // namespace Reportes
