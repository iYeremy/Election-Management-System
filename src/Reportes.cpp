#include "Reportes.h"

#include <fstream>
#include <iostream>
#include <cctype>

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

void generarReporteCiudades(MultilistaRegiones* regiones, const std::string& rutaSalida) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay informacion cargada para el reporte de ciudades.\n";
        return;
    }

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

void reporteGeneralMunicipalPorRegion(MultilistaRegiones* regiones) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones cargadas para el reporte general municipal.\n";
        return;
    }

    std::cout << "\nREPORTE GENERAL DE ALCALDIAS POR REGION\n";
    Region* region = regiones->getCabeza();
    while (region) {
        long long votos = 0;
        long long blancos = 0;
        long long nulos = 0;
        long long abstencion = 0;
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            votos += ciudad->totalVotosValidos();
            blancos += ciudad->getVotosBlanco();
            nulos += ciudad->getVotosNulos();
            abstencion += ciudad->getAbstencion();
            ciudad = ciudad->getSigCiudad();
        }
        long long total = votos + blancos + nulos + abstencion;
        auto imprimir = [&](const std::string& etiqueta, long long cantidad) {
            double porcentaje = total > 0 ? (static_cast<double>(cantidad) * 100.0) / total : 0.0;
            std::cout << "  " << etiqueta << ": " << cantidad << " (" << std::fixed << std::setprecision(2)
                      << porcentaje << "%)\n";
        };
        std::cout << "\nRegion " << region->getNombre() << ":\n";
        imprimir("Votos validos", votos);
        imprimir("Votos en blanco", blancos);
        imprimir("Votos nulos", nulos);
        imprimir("Abstencion", abstencion);
        region = region->getSigRegion();
    }
}

void reporteDetalladoMunicipalPorRegion(MultilistaRegiones* regiones) {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones cargadas para el reporte detallado municipal.\n";
        return;
    }

    std::cout << "\nREPORTE DETALLADO DE ALCALDIAS POR REGION\n";
    Region* region = regiones->getCabeza();
    while (region) {
        int alcaldesPartido[5] = {0};
        int hombres = 0;
        int mujeres = 0;
        int total = 0;

        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            int indiceGanador = ciudad->ganadorAlcaldia();
            if (indiceGanador != -1) {
                NodoCandidato* nodos[4];
                mapearNodosCiudad(ciudad, nodos);
                if (nodos[indiceGanador]) {
                    Candidato* candidato = nodos[indiceGanador]->getCandidato();
                    ++total;
                    if (candidato->getSexo() == 'F' || candidato->getSexo() == 'f') {
                        ++mujeres;
                    } else {
                        ++hombres;
                    }
                    int idx = indicePartido(nullptr, candidato->getPartido()); // placeholder
                    (void)idx; // will fix
                }
            }
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }
}

void reporteGeneroPresidencialPorPartido(const Partido partidos[5], const Duo formulas[5]) {
    struct ConteoGenero {
        int hombres = 0;
        int mujeres = 0;
    };

    ConteoGenero conteos[5];

    auto registrar = [&](Candidato* candidato) {
        if (!candidato) {
            return;
        }
        const Partido* partido = candidato->getPartido();
        int indice = -1;
        for (int i = 0; i < 5; ++i) {
            if (&partidos[i] == partido) {
                indice = i;
                break;
            }
        }
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

    std::cout << "\nReporte de genero en formulas presidenciales\n";
    std::cout << "--------------------------------------\n";
    for (int i = 0; i < 5; ++i) {
        if (partidos[i].getNombre().empty()) {
            continue;
        }
        int total = conteos[i].hombres + conteos[i].mujeres;
        std::cout << partidos[i].getNombre() << ": "
                  << total << " participantes (M "
                  << conteos[i].hombres << ", F " << conteos[i].mujeres << ")\n";
    }
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
