#include "SistemaElectoral.h"

#include <iostream>
#include <iomanip>
#include <cctype>

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
#include "Reportes.h"
#include "SistemaElectoral_Util.h"

using namespace SistemaElectoralInterno;

void SistemaElectoral::mostrarTarjetonAlcaldia(const std::string& nombreCiudad) {
    Ciudad* ciudad = buscarCiudad(nombreCiudad);
    if (!ciudad) {
        std::cout << "Ciudad " << nombreCiudad << " no encontrada.\n";
        return;
    }

    std::cout << "\nTarjeton de alcaldia - " << ciudad->getNombre();
    if (ciudad->getRegion()) {
        std::cout << " (" << ciudad->getRegion()->getNombre() << ")";
    }
    std::cout << "\n--------------------------------------\n";

    std::cout << "0. Voto en blanco\n";
    NodoCandidato* nodo = ciudad->getCandidatosAlcaldia();
    int posicion = 1;
    if (!nodo) {
        std::cout << "Sin candidatos registrados.\n";
    } else {
        while (nodo) {
            Candidato* candidato = nodo->getCandidato();
            if (candidato) {
                Partido* partido = candidato->getPartido();
                std::cout << posicion << ". [" << candidato->getId() << "] "
                          << candidato->getNombre() << " " << candidato->getApellido()
                          << " - Partido: " << (partido ? partido->getNombre() : "Independiente") << "\n";
            }
            nodo = nodo->getSigCiudad();
            ++posicion;
        }
    }

    std::cout << posicion << ". Voto nulo\n";
    std::cout << (posicion + 1) << ". Abstencion\n";
}

void SistemaElectoral::mostrarTarjetonPresidencial() const {
    std::cout << "\nTarjeton presidencial nacional\n";
    std::cout << "--------------------------------------\n";

    std::cout << "0. Voto en blanco\n";
    bool hayFormulas = false;
    int posicion = 1;
    for (int i = 0; i < 5; ++i) {
        const Duo& formula = formulas[i];
        Candidato* presidente = formula.getPresidente();
        Candidato* vicepresidente = formula.getVicepresidente();
        if (!presidente || !vicepresidente) {
            continue;
        }
        hayFormulas = true;
        std::cout << posicion << ". #" << formula.getNumeroPartido()
                  << " - " << presidente->getNombre() << " " << presidente->getApellido()
                  << " / " << vicepresidente->getNombre() << " " << vicepresidente->getApellido();
        if (formula.getPartido()) {
            std::cout << " (" << formula.getPartido()->getNombre() << ")";
        }
        std::cout << "\n";
        ++posicion;
    }
    std::cout << posicion << ". Voto nulo\n";
    std::cout << (posicion + 1) << ". Abstencion\n";
    if (!hayFormulas) {
        std::cout << "No hay formulas presidenciales registradas.\n";
    }
}

void SistemaElectoral::listarCandidatosMunicipalesPorPartido(const std::string& nombrePartido,
                                                             const std::string& nombreRegion,
                                                             const std::string& nombreCiudad) {
    Partido* partido = buscarPartidoPorNombre(partidos, nombrePartido);
    if (!partido) {
        std::cout << "El partido " << nombrePartido << " no existe.\n";
        return;
    }

    NodoCandidato* nodo = partido->getListaCandidatosAlcaldia();
    bool encontrado = false;
    while (nodo) {
        Candidato* candidato = nodo->getCandidato();
        if (candidato && candidato->getTipo() == Candidato::Tipo::ALCALDIA) {
            Ciudad* ciudad = candidato->getCiudadResidencia();
            Region* region = ciudad ? ciudad->getRegion() : nullptr;
            bool coincide = true;
            if (!nombreRegion.empty()) {
                coincide = (region && region->getNombre() == nombreRegion);
            }
            if (coincide && !nombreCiudad.empty()) {
                coincide = (ciudad && ciudad->getNombre() == nombreCiudad);
            }
            if (coincide) {
                encontrado = true;
                std::cout << "- " << candidato->getNombre() << " " << candidato->getApellido()
                          << " (ID " << candidato->getId() << ")"
                          << " | Sexo: " << candidato->getSexo()
                          << " | Edad: " << calcularEdad(candidato->getFechaNacimiento());
                if (ciudad) {
                    std::cout << " | Ciudad: " << ciudad->getNombre();
                }
                if (region) {
                    std::cout << " | Region: " << region->getNombre();
                }
                std::cout << "\n";
            }
        }
        nodo = nodo->getSigPartido();
    }

    if (!encontrado) {
        std::cout << "No se encontraron candidatos que cumplan los filtros indicados.\n";
    }
}

void SistemaElectoral::listarCandidatosPresidenciales() const {
    std::cout << "\nListado de candidatos presidenciales\n";
    std::cout << "--------------------------------------\n";

    auto imprimirCandidato = [](const Candidato* candidato, const char* rol) {
        if (!candidato) {
            return;
        }
        const Ciudad* residencia = candidato->getCiudadResidencia();
        std::cout << rol << ": [" << candidato->getId() << "] "
                  << candidato->getNombre() << " " << candidato->getApellido()
                  << " | Nac.: " << candidato->getCiudadNacimiento()
                  << " | Residencia: " << (residencia ? residencia->getNombre() : "Sin registro")
                  << " | Fecha: " << candidato->getFechaNacimiento().getDia() << "/"
                  << candidato->getFechaNacimiento().getMes() << "/"
                  << candidato->getFechaNacimiento().getYear()
                  << " | Edad: " << calcularEdad(candidato->getFechaNacimiento())
                  << " | Estado civil: " << candidato->getEstadoCivil()
                  << " | Partido: "
                  << (candidato->getPartido() ? candidato->getPartido()->getNombre() : "Independiente")
                  << "\n";
    };

    bool hayInfo = false;
    for (int i = 0; i < 5; ++i) {
        const Duo& formula = formulas[i];
        if (!formulas[i].getPresidente() || !formula.getVicepresidente()) {
            continue;
        }
        hayInfo = true;
        std::cout << "Formula #" << (i + 1) << " (Partido "
                  << (formula.getPartido() ? formula.getPartido()->getNombre() : "Sin partido")
                  << ")\n";
        imprimirCandidato(formula.getPresidente(), "  Presidente");
        imprimirCandidato(formula.getVicepresidente(), "  Vicepresidente");
        std::cout << "\n";
    }

    if (!hayInfo) {
        std::cout << "No hay formulas registradas.\n";
    }
}

void SistemaElectoral::mostrarCandidatosCiudadPorPartido(const std::string& nombreCiudad) {
    Ciudad* ciudad = buscarCiudad(nombreCiudad);
    if (!ciudad) {
        std::cout << "Ciudad no encontrada.\n";
        return;
    }

    std::cout << "\nCandidatos en " << ciudad->getNombre() << ":\n";
    bool hayIndependientes = false;
    NodoCandidato* nodo = ciudad->getCandidatosAlcaldia();
    while (nodo) {
        Candidato* candidato = nodo->getCandidato();
        if (candidato && candidato->getPartido()) {
            std::cout << " - " << candidato->getPartido()->getNombre()
                      << ": " << candidato->getNombre() << " " << candidato->getApellido() << "\n";
        } else if (candidato) {
            if (!hayIndependientes) {
                std::cout << " - Independientes: ";
                hayIndependientes = true;
            } else {
                std::cout << ", ";
            }
            std::cout << candidato->getNombre() << " " << candidato->getApellido();
        }
        nodo = nodo->getSigCiudad();
    }
    if (hayIndependientes) {
        std::cout << "\n";
    }
}

void SistemaElectoral::mostrarCensoElectoral() const {
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay informacion cargada.\n";
        return;
    }

    long long total = 0;
    std::cout << "\nCenso electoral por ciudad:\n";
    Region* region = regiones->getCabeza();
    while (region) {
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            std::cout << " - " << ciudad->getNombre()
                      << " (" << region->getNombre() << "): "
                      << ciudad->getCenso() << " habilitados\n";
            total += ciudad->getCenso();
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }
    std::cout << "Total nacional: " << total << " personas habilitadas.\n";
}

void SistemaElectoral::reporteGeneroPresidencialPorPartido() const {
    Reportes::reporteGeneroPresidencialPorPartido(
        partidos, formulas, "reportes/reporte_genero_partido_presidencial.txt");
}

void SistemaElectoral::mostrarEstructuraDatos() const {
    Reportes::mostrarEstructuraDatos(regiones, arbolCiudades, arbolCandidatos);
}
