#include "SistemaElectoral.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <vector>

#include "MultilistaRegiones.h"
#include "Region.h"
#include "Ciudad.h"
#include "Candidato.h"
#include "Partido.h"
#include "Duo.h"
#include "NodoCandidato.h"
#include "TarjetonAlcaldia.h"
#include "TarjetonPresidencial.h"
#include "CiudadHelper.h"

namespace {
void asegurarSemilla() {
    static bool sembrada = false;
    if (!sembrada) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        sembrada = true;
    }
}
}

void SistemaElectoral::simularVotacion() {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones/cidades cargadas para simular.\n";
        return;
    }

    Region* regionCheck = regiones->getCabeza();
    bool datosPrevios = false;
    while (regionCheck && !datosPrevios) {
        Ciudad* ciudadCheck = regionCheck->getListaCiudades();
        while (ciudadCheck) {
            if (ciudadCheck->totalVotos() > 0) {
                datosPrevios = true;
                break;
            }
            ciudadCheck = ciudadCheck->getSigCiudad();
        }
        regionCheck = regionCheck->getSigRegion();
    }
    if (datosPrevios) {
        std::cout << "Ya existen votos registrados. Reinicia los datos antes de volver a simular.\n";
        return;
    }

    simulacionEnCurso = true;

    segundaVueltaPendiente = false;
    segundaVueltaRealizada = false;
    indicesSegundaVuelta[0] = indicesSegundaVuelta[1] = -1;
    votosSegundaVuelta[0] = votosSegundaVuelta[1] = 0;
    blancosSegundaVuelta = nulosSegundaVuelta = abstencionSegundaVuelta = 0;

    asegurarSemilla();

    for (int i = 0; i < 5; ++i) {
        formulas[i].setVotos(0);
    }

    TarjetonAlcaldia tarjetonMunicipal;
    TarjetonPresidencial tarjetonPresidencial;
    long long totalMunicipales = 0;
    totalVotosPresidenciales = 0;
    blancosPresidenciales = 0;
    nulosPresidenciales = 0;
    abstencionPresidencial = 0;

    Region* region = regiones->getCabeza();
    while (region) {
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            NodoCandidato* nodos[4];
            mapearNodosCiudad(ciudad, nodos);
            int censo = ciudad->getCenso();

            for (int persona = 0; persona < censo; ++persona) {
                int votoMunicipal = tarjetonMunicipal.votoAleatorio();
                if (votoMunicipal >= 0 && votoMunicipal < 4) {
                    if (nodos[votoMunicipal]) {
                        ciudad->registrarVotoCandidato(votoMunicipal);
                    } else {
                        ciudad->registrarVotoNulo();
                    }
                } else if (votoMunicipal == 4) {
                    ciudad->registrarVotoBlanco();
                } else if (votoMunicipal == 5) {
                    ciudad->registrarVotoNulo();
                } else {
                    ciudad->registrarAbstencion();
                }
                ++totalMunicipales;

                int votoPresidencial = tarjetonPresidencial.votoAleatorio();
                if (votoPresidencial >= 0 && votoPresidencial < 5) {
                    if (formulas[votoPresidencial].getPresidente()) {
                        formulas[votoPresidencial].setVotos(formulas[votoPresidencial].getVotos() + 1);
                        ciudad->registrarVotoPresidencial(votoPresidencial);
                    } else {
                        ciudad->registrarAbstencionPresidencial();
                        ++abstencionPresidencial;
                    }
                } else if (votoPresidencial == 5) {
                    ciudad->registrarVotoPresidencialBlanco();
                    ++blancosPresidenciales;
                } else if (votoPresidencial == 6) {
                    ciudad->registrarVotoPresidencialNulo();
                    ++nulosPresidenciales;
                } else {
                    ciudad->registrarAbstencionPresidencial();
                    ++abstencionPresidencial;
                }
                ++totalVotosPresidenciales;
            }

            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }

    long long votosFormulas = 0;
    for (int i = 0; i < 5; ++i) {
        votosFormulas += formulas[i].getVotos();
    }

    std::cout << "Simulacion completada.\n"
              << " - Votos municipales procesados: " << totalMunicipales << "\n"
              << " - Votos presidenciales procesados: " << totalVotosPresidenciales << "\n"
              << "   -> Formulas: " << votosFormulas
              << ", Blanco: " << blancosPresidenciales
              << ", Nulo: " << nulosPresidenciales
              << ", Abstencion: " << abstencionPresidencial << "\n";

    simulacionEnCurso = false;
    simulacionEjecutadaFlag = true;
    if (actualizarArchivosDesdeMemoria()) {
        std::cout << "Archivos de candidatos actualizados para reflejar los cambios.\n";
    } else {
        std::cout << "Advertencia: no se pudieron actualizar los archivos con el estado actual.\n";
    }
}

void SistemaElectoral::calcularGanadoresMunicipales() {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay informacion municipal para analizar.\n";
        return;
    }

    bool hayCiudades = false;
    Region* region = regiones->getCabeza();
    while (region) {
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            hayCiudades = true;
            std::cout << "\nCiudad: " << ciudad->getNombre()
                      << " (Region " << region->getNombre() << ")\n";

            if (ciudad->totalVotos() == 0) {
                std::cout << " - Aun no se han registrado votos.\n";
            } else {
                const int* votos = ciudad->getVotosCandidato();
                int total = ciudad->totalVotos();
                NodoCandidato* nodos[4];
                mapearNodosCiudad(ciudad, nodos);
                std::cout << "   Detalle de candidatos:\n";
                std::cout << "   Nombre                          Votos      %\n";
                for (int i = 0; i < 4; ++i) {
                    if (!nodos[i]) {
                        continue;
                    }
                    Candidato* candidato = nodos[i]->getCandidato();
                    double porcentaje = total > 0 ? (static_cast<double>(votos[i]) * 100.0) / total : 0.0;
                    std::string etiqueta = candidato ? (candidato->getNombre() + std::string(" ") + candidato->getApellido()) : "Candidato";
                    if (candidato && candidato->getPartido()) {
                        etiqueta += " (" + candidato->getPartido()->getNombre() + ")";
                    }
                    std::cout << "   " << etiqueta;
                    if (etiqueta.size() < 30) {
                        std::cout << std::string(30 - etiqueta.size(), ' ');
                    } else {
                        std::cout << " ";
                    }
                    std::cout << votos[i] << "    " << std::fixed << std::setprecision(2) << porcentaje << "%\n";
                }
                auto imprimirLinea = [&](const std::string& etiqueta, int cantidad) {
                    double porcentaje = total > 0 ? (static_cast<double>(cantidad) * 100.0) / total : 0.0;
                    std::cout << "   " << etiqueta << ": " << cantidad << " (" << std::fixed << std::setprecision(2)
                              << porcentaje << "%)\n";
                };
                imprimirLinea("Votos en blanco", ciudad->getVotosBlanco());
                imprimirLinea("Votos nulos", ciudad->getVotosNulos());
                imprimirLinea("Abstencion", ciudad->getAbstencion());

                int indiceGanador = ciudad->ganadorAlcaldia();
                if (indiceGanador == -1) {
                    std::cout << " - Sin ganador (empate o sin votos validos).\n";
                } else {
                    Candidato* ganador = candidatoMunicipalPorIndice(ciudad, indiceGanador);
                    double porcentaje = ciudad->porcentajeCandidato(indiceGanador);
                    if (ganador) {
                        std::cout << " - Ganador: " << ganador->getNombre() << " " << ganador->getApellido()
                                  << " con " << votos[indiceGanador] << " votos (" << porcentaje << "%).\n";
                    } else {
                        std::cout << " - Ganador identificado en indice " << indiceGanador
                                  << " con " << votos[indiceGanador] << " votos (" << porcentaje << "%).\n";
                    }
                }
            }

            std::cout << "   [Presidencia]\n";
            int totalPres = ciudad->totalVotosPresidencialesCiudad();
            if (totalPres == 0) {
                std::cout << " - Aun no se han registrado votos presidenciales.\n";
            } else {
                const int* votosPres = ciudad->getVotosPresidenciales();
                std::cout << "   Formula                          Votos      %\n";
                for (int i = 0; i < 5; ++i) {
                    const Duo& formula = formulas[i];
                    Candidato* presidente = formula.getPresidente();
                    Candidato* vice = formula.getVicepresidente();
                    if (!presidente || !vice) {
                        continue;
                    }
                    std::string etiqueta = presidente->getNombre() + std::string(" ") + presidente->getApellido()
                                            + " / " + vice->getNombre() + " " + vice->getApellido();
                    if (formula.getPartido()) {
                        etiqueta += " (" + formula.getPartido()->getNombre() + ")";
                    }
                    if (etiqueta.size() < 30) {
                        etiqueta += std::string(30 - etiqueta.size(), ' ');
                    }
                    std::cout << "   " << etiqueta << " " << votosPres[i] << "    "
                              << std::fixed << std::setprecision(2) << ciudad->porcentajeVotoPresidencial(i) << "%\n";
                }

                auto imprimirLineaPres = [&](const std::string& etiqueta, int cantidad, double porcentaje) {
                    std::cout << "   " << etiqueta << ": " << cantidad << " (" << std::fixed << std::setprecision(2)
                              << porcentaje << "%)\n";
                };
                imprimirLineaPres("Votos en blanco",
                                  ciudad->getVotosPresidencialesBlanco(),
                                  ciudad->porcentajePresidencialBlanco());
                imprimirLineaPres("Votos nulos",
                                  ciudad->getVotosPresidencialesNulos(),
                                  ciudad->porcentajePresidencialNulo());
                imprimirLineaPres("Abstencion",
                                  ciudad->getAbstencionPresidencial(),
                                  ciudad->porcentajePresidencialAbstencion());

                int ganadorPres = ciudad->ganadorPresidencialCiudad();
                if (ganadorPres == -1) {
                    std::cout << " - Sin ganador presidencial (empate o sin votos validos).\n";
                } else {
                    const Duo& formulaGanadora = formulas[ganadorPres];
                    Candidato* presidente = formulaGanadora.getPresidente();
                    Candidato* vice = formulaGanadora.getVicepresidente();
                    double porcentaje = ciudad->porcentajeVotoPresidencial(ganadorPres);
                    std::cout << " - Ganador presidencial: "
                              << (presidente ? presidente->getNombre() + std::string(" ") + presidente->getApellido()
                                             : "Formula " + std::to_string(ganadorPres + 1))
                              << " con " << votosPres[ganadorPres] << " votos (" << porcentaje << "%).\n";
                    if (vice) {
                        std::cout << "   Vicepresidencia: " << vice->getNombre() << " " << vice->getApellido() << "\n";
                    }
                }
            }

            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }

    if (!hayCiudades) {
        std::cout << "No hay ciudades registradas.\n";
    }
}

void SistemaElectoral::calcularGanadoresRegionales() {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "Sin regiones para resumir resultados.\n";
        return;
    }

    Region* region = regiones->getCabeza();
    while (region) {
        std::cout << "\nRegion: " << region->getNombre() << "\n";
        Ciudad* ciudad = region->getListaCiudades();
        if (!ciudad) {
            std::cout << " - No tiene ciudades asociadas.\n";
            region = region->getSigRegion();
            continue;
        }

        Ciudad* lider = nullptr;
        int votosLider = -1;
        while (ciudad) {
            int votosValidos = ciudad->totalVotosValidos();
            std::cout << "   * " << ciudad->getNombre()
                      << ": votos validos " << votosValidos
                      << ", total procesado " << ciudad->totalVotos() << "\n";
            if (votosValidos > votosLider) {
                votosLider = votosValidos;
                lider = ciudad;
            }
            ciudad = ciudad->getSigCiudad();
        }

        if (!lider || votosLider <= 0) {
            std::cout << "   No hay ganador regional (sin votos validos suficientes).\n";
        } else {
            int indice = lider->ganadorAlcaldia();
            Candidato* ganador = candidatoMunicipalPorIndice(lider, indice);
            std::string nombreGanador = ganador ? (ganador->getNombre() + std::string(" ") + ganador->getApellido())
                                                : "(candidato no identificado)";
            std::cout << "   Ciudad con mayor respaldo: " << lider->getNombre()
                      << " - " << nombreGanador
                      << " con " << votosLider << " votos validos.\n";
        }

        region = region->getSigRegion();
    }
}

void SistemaElectoral::calcularGanadorPresidencial() {
    long long totalActas = totalVotosPresidenciales;
    if (segundaVueltaRealizada) {
        int idxA = indicesSegundaVuelta[0];
        int idxB = indicesSegundaVuelta[1];
        if (idxA < 0 || idxB < 0) {
            std::cout << "Los datos de segunda vuelta no estan disponibles.\n";
            segundaVueltaRealizada = false;
            return;
        }

        Duo& duoA = formulas[idxA];
        Duo& duoB = formulas[idxB];
        Candidato* presA = duoA.getPresidente();
        Candidato* presB = duoB.getPresidente();
        Candidato* viceA = duoA.getVicepresidente();
        Candidato* viceB = duoB.getVicepresidente();
        if (!presA || !presB || !viceA || !viceB) {
            std::cout << "Los datos de segunda vuelta no son consistentes.\n";
            segundaVueltaRealizada = false;
            return;
        }

        std::cout << "\n[SEGUNDA VUELTA PRESIDENCIAL]\n";
        std::cout << "Formula 1: " << presA->getNombre() << " " << presA->getApellido()
                  << " + " << viceA->getNombre() << " " << viceA->getApellido()
                  << " -> " << votosSegundaVuelta[0] << " votos\n";
        std::cout << "Formula 2: " << presB->getNombre() << " " << presB->getApellido()
                  << " + " << viceB->getNombre() << " " << viceB->getApellido()
                  << " -> " << votosSegundaVuelta[1] << " votos\n";
        std::cout << "Blancos: " << blancosSegundaVuelta
                  << ", Nulos: " << nulosSegundaVuelta
                  << ", Abstencion: " << abstencionSegundaVuelta << "\n";

        if (votosSegundaVuelta[0] == votosSegundaVuelta[1]) {
            std::cout << "La segunda vuelta termino empatada. Requiere definicion externa.\n";
        } else {
            int ganador = (votosSegundaVuelta[0] > votosSegundaVuelta[1]) ? idxA : idxB;
            Duo& duoGanador = formulas[ganador];
            std::cout << "Ganador definitivo: "
                      << duoGanador.getPresidente()->getNombre() << " "
                      << duoGanador.getPresidente()->getApellido()
                      << " acompañado por "
                      << duoGanador.getVicepresidente()->getNombre() << " "
                      << duoGanador.getVicepresidente()->getApellido() << ".\n";
        }
        return;
    }

    int totalVotos = 0;
    for (int i = 0; i < 5; ++i) {
        totalVotos += formulas[i].getVotos();
    }

    if (totalActas == 0) {
        std::cout << "Aun no se han registrado votos presidenciales.\n";
        return;
    }

    Duo* duoGanador = nullptr;
    int mejorVotos = -1;
    std::vector<int> indicesLideres;

    std::cout << "\nResultados presidenciales:\n";
    for (int i = 0; i < 5; ++i) {
        Candidato* presidente = formulas[i].getPresidente();
        Candidato* vicepresidente = formulas[i].getVicepresidente();
        if (!presidente || !vicepresidente) {
            if (formulas[i].getVotos() > 0) {
                std::cout << " - Formula #" << (i + 1) << " incompleta registro "
                          << formulas[i].getVotos() << " votos.\n";
            }
            continue;
        }

        int votos = formulas[i].getVotos();
        double porcentaje = totalActas > 0 ? (static_cast<double>(votos) * 100.0) / totalActas : 0.0;
        std::cout << " - " << presidente->getNombre() << " " << presidente->getApellido()
                  << " + " << vicepresidente->getNombre() << " " << vicepresidente->getApellido()
                  << ": " << votos << " votos (" << std::fixed << std::setprecision(2) << porcentaje << "%).\n";

        if (votos > mejorVotos) {
            mejorVotos = votos;
            duoGanador = &formulas[i];
            indicesLideres.clear();
            indicesLideres.push_back(i);
        } else if (votos == mejorVotos) {
            indicesLideres.push_back(i);
        }
    }

    if (!duoGanador) {
        std::cout << "No se pudo determinar un ganador valido.\n";
        return;
    }

    if (indicesLideres.size() > 1) {
        segundaVueltaPendiente = true;
        segundaVueltaRealizada = false;
        indicesSegundaVuelta[0] = indicesLideres[0];
        indicesSegundaVuelta[1] = indicesLideres[1];
        std::cout << "Existe un empate entre las formulas "
                  << (indicesSegundaVuelta[0] + 1) << " y "
                  << (indicesSegundaVuelta[1] + 1) << ".\n"
                  << "Use la opcion de segunda vuelta para definir el ganador.\n";
        return;
    }

    segundaVueltaPendiente = false;
    segundaVueltaRealizada = false;
    indicesSegundaVuelta[0] = indicesSegundaVuelta[1] = -1;

    Candidato* pres = duoGanador->getPresidente();
    Candidato* vice = duoGanador->getVicepresidente();
    std::cout << "\nFormula ganadora: "
              << pres->getNombre() << " " << pres->getApellido()
              << " con " << mejorVotos << " votos. Vice: "
              << vice->getNombre() << " " << vice->getApellido() << ".\n";

    auto imprimirLinea = [&](const std::string& etiqueta, long long cantidad) {
        double porcentaje = totalActas > 0 ? (static_cast<double>(cantidad) * 100.0) / totalActas : 0.0;
        std::cout << etiqueta << ": " << cantidad << " (" << std::fixed << std::setprecision(2)
                  << porcentaje << "%)\n";
    };
    imprimirLinea("Votos en blanco", blancosPresidenciales);
    imprimirLinea("Votos nulos", nulosPresidenciales);
    imprimirLinea("Abstencion", abstencionPresidencial);
}

void SistemaElectoral::simularSegundaVueltaPresidencial() {
    if (segundaVueltaRealizada) {
        std::cout << "La segunda vuelta ya fue simulada. Consulte el resultado.\n";
        return;
    }
    if (!segundaVueltaPendiente) {
        std::cout << "No hay empate pendiente para segunda vuelta.\n";
        return;
    }
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones cargadas para simular.\n";
        return;
    }

    int idxA = indicesSegundaVuelta[0];
    int idxB = indicesSegundaVuelta[1];
    if (idxA < 0 || idxB < 0) {
        std::cout << "No se identificaron formulas empatadas.\n";
        return;
    }

    Duo& formulaA = formulas[idxA];
    Duo& formulaB = formulas[idxB];
    if (!formulaA.getPresidente() || !formulaA.getVicepresidente() ||
        !formulaB.getPresidente() || !formulaB.getVicepresidente()) {
        std::cout << "Las formulas de segunda vuelta estan incompletas.\n";
        return;
    }

    asegurarSemilla();

    votosSegundaVuelta[0] = votosSegundaVuelta[1] = 0;
    blancosSegundaVuelta = nulosSegundaVuelta = abstencionSegundaVuelta = 0;

    Region* region = regiones->getCabeza();
    while (region) {
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            int censo = ciudad->getCenso();
            for (int persona = 0; persona < censo; ++persona) {
                int voto = std::rand() % 5;
                if (voto == 0) {
                    ++votosSegundaVuelta[0];
                } else if (voto == 1) {
                    ++votosSegundaVuelta[1];
                } else if (voto == 2) {
                    ++blancosSegundaVuelta;
                } else if (voto == 3) {
                    ++nulosSegundaVuelta;
                } else {
                    ++abstencionSegundaVuelta;
                }
            }
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }

    segundaVueltaPendiente = false;
    segundaVueltaRealizada = true;

    std::cout << "Segunda vuelta simulada correctamente.\n";
    calcularGanadorPresidencial();
}
