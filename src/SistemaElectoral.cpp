#include "SistemaElectoral.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

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
#include "TarjetonAlcaldia.h"
#include "TarjetonPresidencial.h"

namespace {

void mapearNodosCiudad(Ciudad* ciudad, NodoCandidato* nodos[4]) {
    for (int i = 0; i < 4; ++i) {
        nodos[i] = nullptr;
    }
    if (!ciudad) {
        return;
    }

    NodoCandidato* actual = ciudad->getCandidatosAlcaldia();
    int idx = 0;
    while (actual && idx < 4) {
        nodos[idx++] = actual;
        actual = actual->getSigCiudad();
    }
}

Candidato* candidatoMunicipalPorIndice(Ciudad* ciudad, int idx) {
    if (!ciudad || idx < 0 || idx >= 4) {
        return nullptr;
    }
    NodoCandidato* actual = ciudad->getCandidatosAlcaldia();
    int pos = 0;
    while (actual && pos < 4) {
        if (pos == idx) {
            return actual->getCandidato();
        }
        actual = actual->getSigCiudad();
        ++pos;
    }
    return nullptr;
}

}  // namespace

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
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "No hay regiones/cidades cargadas para simular.\n";
        return;
    }

    // evita que una segunda simulacion mezcle resultados
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

    static bool sembrada = false;
    if (!sembrada) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        sembrada = true;
    }

    for (int i = 0; i < 5; ++i) {
        formulas[i].setVotos(0);
    }

    TarjetonAlcaldia tarjetonMunicipal;
    TarjetonPresidencial tarjetonPresidencial;
    long long totalMunicipales = 0;
    long long totalPresidenciales = 0;
    long long blancosPresidenciales = 0;
    long long nulosPresidenciales = 0;
    long long abstencionesPresidenciales = 0;

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
                if (votoPresidencial >= 0 && votoPresidencial < 5 && formulas[votoPresidencial].getPresidente()) {
                    formulas[votoPresidencial].setVotos(formulas[votoPresidencial].getVotos() + 1);
                } else if (votoPresidencial == 5) {
                    ++blancosPresidenciales;
                } else if (votoPresidencial == 6) {
                    ++nulosPresidenciales;
                } else {
                    ++abstencionesPresidenciales;
                }
                ++totalPresidenciales;
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
              << " - Votos presidenciales procesados: " << totalPresidenciales << "\n"
              << "   -> Formulas: " << votosFormulas
              << ", Blanco: " << blancosPresidenciales
              << ", Nulo: " << nulosPresidenciales
              << ", Abstencion: " << abstencionesPresidenciales << "\n";
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
                int indiceGanador = ciudad->ganadorAlcaldia();
                if (indiceGanador == -1) {
                    std::cout << " - Sin ganador (empate o sin votos validos).\n";
                } else {
                    Candidato* ganador = candidatoMunicipalPorIndice(ciudad, indiceGanador);
                    const int* votos = ciudad->getVotosCandidato();
                    std::streamsize precisionAnterior = std::cout.precision();
                    std::cout << std::fixed << std::setprecision(2);
                    double porcentaje = ciudad->porcentajeCandidato(indiceGanador);
                    if (ganador) {
                        std::cout << " - Ganador: " << ganador->getNombre() << " " << ganador->getApellido()
                                  << " con " << votos[indiceGanador] << " votos (" << porcentaje << "%).\n";
                    } else {
                        std::cout << " - Ganador identificado en indice " << indiceGanador
                                  << " con " << votos[indiceGanador] << " votos (" << porcentaje << "%).\n";
                    }
                    std::cout.unsetf(std::ios::floatfield);
                    std::cout.precision(precisionAnterior);
                }
                std::cout << "   Blancos: " << ciudad->getVotosBlanco()
                          << ", Nulos: " << ciudad->getVotosNulos()
                          << ", Abstencion: " << ciudad->getAbstencion() << "\n";
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
    int totalVotos = 0;
    for (int i = 0; i < 5; ++i) {
        totalVotos += formulas[i].getVotos();
    }

    if (totalVotos == 0) {
        std::cout << "Aun no se han registrado votos presidenciales.\n";
        return;
    }

    Duo* duoGanador = nullptr;
    int mejorVotos = -1;
    bool empate = false;

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
        std::cout << " - " << presidente->getNombre() << " " << presidente->getApellido()
                  << " + " << vicepresidente->getNombre() << " " << vicepresidente->getApellido()
                  << ": " << votos << " votos.\n";

        if (votos > mejorVotos) {
            mejorVotos = votos;
            duoGanador = &formulas[i];
            empate = false;
        } else if (votos == mejorVotos) {
            empate = true;
        }
    }

    if (!duoGanador) {
        std::cout << "No se pudo determinar un ganador valido.\n";
        return;
    }

    if (empate) {
        std::cout << "Existe un empate en votos. Se requiere desempate externo.\n";
        return;
    }

    Candidato* pres = duoGanador->getPresidente();
    Candidato* vice = duoGanador->getVicepresidente();
    std::cout << "\nFormula ganadora: "
              << pres->getNombre() << " " << pres->getApellido()
              << " con " << mejorVotos << " votos. Vice: "
              << vice->getNombre() << " " << vice->getApellido() << ".\n";
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

const Partido* SistemaElectoral::getPartidos() const {
    return partidos;
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
