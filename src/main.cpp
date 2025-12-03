#include <iostream>
#include <limits>
#include <string>

#include "SistemaElectoral.h"
#include "AVLCiudades.h"
#include "AVLCandidatos.h"
#include "Ciudad.h"
#include "MultilistaRegiones.h"
#include "Region.h"
#include "NodoCandidato.h"
#include "Candidato.h"
#include "Partido.h"

namespace {

void mostrarMenu() {
    std::cout << "\n====== Simulador Electoral ======\n"
              << " 1. Cargar regiones\n"
              << " 2. Cargar ciudades\n"
              << " 3. Cargar partidos\n"
              << " 4. Cargar candidatos a alcaldia\n"
              << " 5. Cargar candidatos a presidencia\n"
              << " 6. Cargar formulas presidenciales\n"
              << " 7. Buscar ciudad\n"
              << " 8. Buscar candidato\n"
              << " 9. Simular votacion\n"
              << "10. Calcular ganadores municipales\n"
              << "11. Calcular ganadores regionales\n"
              << "12. Calcular ganador presidencial\n"
              << "13. Generar reporte de ciudades\n"
              << "14. Generar reporte de regiones\n"
              << "15. Generar reporte nacional\n"
              << "16. Datos de demostracion / prueba rapida\n"
              << " 0. Salir\n";
}

std::string leerLinea(const std::string& mensaje) {
    std::cout << mensaje;
    std::string linea;
    std::getline(std::cin, linea);
    return linea;
}

template <typename T>
bool leerNumero(const std::string& mensaje, T& numero) {
    std::cout << mensaje;
    if (!(std::cin >> numero)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada numerica invalida. Intenta nuevamente.\n";
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

void ejecutarEscenarioDemo(SistemaElectoral& sistema) {
    std::cout << "\n--- Preparando escenario de demostracion ---\n";
    sistema.cargarDatosDemostracion();

    std::cout << "Escenario creado:\n"
              << " - Multilista con 2 regiones:\n"
              << "     Region Andina -> Bogota, Medellin\n"
              << "     Region Caribe -> Barranquilla\n"
              << " - Partidos registrados: Renovacion Urbana y Union Caribe\n"
              << " - Candidatos municipales y presidenciales insertados en sus AVL correspondientes.\n";

    const char* ciudadesPrueba[3] = {"Bogota", "Medellin", "Barranquilla"};
    for (int i = 0; i < 3; i++) {
        Ciudad* ciudad = sistema.buscarCiudad(ciudadesPrueba[i]);
        if (ciudad) {
            std::cout << " * " << ciudadesPrueba[i]
                      << ": censo registrado " << ciudad->getCenso() << "\n";
        } else {
            std::cout << " * " << ciudadesPrueba[i] << ": no encontrada\n";
        }
    }

    const long candidatosPrueba[3] = {101001, 201001, 999999};
    for (int i = 0; i < 3; i++) {
        Candidato* resultado = sistema.buscarCandidato(candidatosPrueba[i]);
        if (resultado) {
            std::cout << " * ID " << candidatosPrueba[i] << ": presente en el arbol\n";
        } else {
            std::cout << " * ID " << candidatosPrueba[i] << ": no esta cargado\n";
        }
    }

    if (AVLCiudades* arbol = sistema.getArbolCiudades()) {
        std::cout << "\n[AVL de ciudades - recorrido inorden]\n";
        arbol->imprimir();
    }
    if (AVLCandidatos* arbolC = sistema.getArbolCandidatos()) {
        std::cout << "\n[AVL de candidatos - recorrido inorden]\n";
        arbolC->imprimir();
    }

    std::cout << "Escenario listo. Puedes ejecutar simulaciones o consultas.\n";
}

void mostrarResumenRegiones(const SistemaElectoral& sistema) {
    std::cout << "\nRegiones y ciudades disponibles:\n";
    MultilistaRegiones* lista = sistema.getRegiones();
    if (!lista || !lista->getCabeza()) {
        std::cout << " (sin regiones registradas)\n";
        return;
    }

    Region* region = lista->getCabeza();
    while (region) {
        std::cout << " - " << region->getNombre() << ":\n";
        Ciudad* ciudad = region->getListaCiudades();
        if (!ciudad) {
            std::cout << "     (sin ciudades)\n";
        }
        while (ciudad) {
            std::cout << "     * " << ciudad->getNombre()
                      << " (censo " << ciudad->getCenso() << ")\n";
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }
}

void imprimirListaDobleCiudad(Ciudad* ciudad) {
    NodoCandidato* cabeza = ciudad->getCandidatosAlcaldia();
    if (!cabeza) {
        std::cout << "     (sin candidatos)\n";
        return;
    }

    std::cout << "     Recorrido hacia adelante:\n";
    NodoCandidato* actual = cabeza;
    NodoCandidato* ultimo = nullptr;
    while (actual) {
        if (Candidato* c = actual->getCandidato()) {
            std::cout << "       -> " << c->getNombre()
                      << " " << c->getApellido()
                      << " (ID " << c->getId() << ")\n";
        }
        ultimo = actual;
        actual = actual->getSigCiudad();
    }

    std::cout << "     Recorrido inverso (usando punteros anteriores):\n";
    actual = ultimo;
    while (actual) {
        if (Candidato* c = actual->getCandidato()) {
            std::cout << "       <- " << c->getNombre()
                      << " " << c->getApellido()
                      << " (ID " << c->getId() << ")\n";
        }
        actual = actual->getAntCiudad();
    }
}

void mostrarCandidatosMunicipales(const SistemaElectoral& sistema) {
    MultilistaRegiones* lista = sistema.getRegiones();
    if (!lista) {
        return;
    }

    Region* region = lista->getCabeza();
    bool tituloMostrado = false;

    while (region) {
        Ciudad* ciudad = region->getListaCiudades();
        while (ciudad) {
            NodoCandidato* nodo = ciudad->getCandidatosAlcaldia();
            if (nodo && !tituloMostrado) {
                std::cout << "\nCandidatos a alcaldia por ciudad:\n";
                tituloMostrado = true;
            }
            if (!nodo) {
                ciudad = ciudad->getSigCiudad();
                continue;
            }

            std::cout << " - " << ciudad->getNombre() << ":\n";
            imprimirListaDobleCiudad(ciudad);
            ciudad = ciudad->getSigCiudad();
        }
        region = region->getSigRegion();
    }

    if (!tituloMostrado) {
        std::cout << "\nNo hay candidatos municipales cargados.\n";
    }
}

void imprimirListaDoblePartido(NodoCandidato* cabeza) {
    if (!cabeza) {
        std::cout << "     (sin candidatos asociados)\n";
        return;
    }

    std::cout << "     Recorrido por sigPartido:\n";
    NodoCandidato* actual = cabeza;
    NodoCandidato* ultimo = nullptr;
    while (actual) {
        if (Candidato* c = actual->getCandidato()) {
            std::cout << "       -> " << c->getNombre()
                      << " " << c->getApellido()
                      << " (ID " << c->getId() << ")\n";
        }
        ultimo = actual;
        actual = actual->getSigPartido();
    }

    std::cout << "     Recorrido inverso (antPartido):\n";
    actual = ultimo;
    while (actual) {
        if (Candidato* c = actual->getCandidato()) {
            std::cout << "       <- " << c->getNombre()
                      << " " << c->getApellido()
                      << " (ID " << c->getId() << ")\n";
        }
        actual = actual->getAntPartido();
    }
}

void mostrarCandidatosPorPartido(const SistemaElectoral& sistema) {
    const Partido* partidos = sistema.getPartidos();
    if (!partidos) {
        return;
    }

    bool titulo = false;
    for (int i = 0; i < 5; i++) {
        const Partido& partido = partidos[i];
        if (partido.getNombre().empty() && !partido.getListaCandidatosAlcaldia()) {
            continue;
        }
        if (!titulo) {
            std::cout << "\nCandidatos municipales organizados por partido:\n";
            titulo = true;
        }

        std::string nombrePartido = partido.getNombre().empty() ? "(sin nombre)" : partido.getNombre();
        std::cout << " - " << nombrePartido << " (representante: "
                  << (partido.getRepresentante().empty() ? "n/a" : partido.getRepresentante()) << ")\n";
        imprimirListaDoblePartido(partido.getListaCandidatosAlcaldia());
    }

    if (!titulo) {
        std::cout << "\nNo hay partidos con candidatos municipales cargados.\n";
    }
}

}  // namespace

int main() {
    SistemaElectoral sistema;
    bool salir = false;

    while (!salir) {
        mostrarMenu();

        int opcion = -1;
        if (!leerNumero("Selecciona una opcion: ", opcion)) {
            continue;
        }

        switch (opcion) {
        case 1: {
            std::string ruta = leerLinea("Ruta del archivo de regiones: ");
            if (ruta.empty()) {
                std::cout << "Ruta vacia. Operacion cancelada.\n";
                break;
            }
            sistema.cargarRegiones(ruta);
            std::cout << "Proceso de carga solicitado.\n";
            break;
        }
        case 2: {
            std::string ruta = leerLinea("Ruta del archivo de ciudades: ");
            if (ruta.empty()) {
                std::cout << "Ruta vacia. Operacion cancelada.\n";
                break;
            }
            sistema.cargarCiudades(ruta);
            std::cout << "Proceso de carga solicitado.\n";
            break;
        }
        case 3: {
            std::string ruta = leerLinea("Ruta del archivo de partidos: ");
            if (ruta.empty()) {
                std::cout << "Ruta vacia. Operacion cancelada.\n";
                break;
            }
            sistema.cargarPartidos(ruta);
            std::cout << "Proceso de carga solicitado.\n";
            break;
        }
        case 4: {
            std::string ruta = leerLinea("Ruta del archivo de candidatos a alcaldia: ");
            if (ruta.empty()) {
                std::cout << "Ruta vacia. Operacion cancelada.\n";
                break;
            }
            sistema.cargarCandidatosAlcaldia(ruta);
            std::cout << "Proceso de carga solicitado.\n";
            break;
        }
        case 5: {
            std::string ruta = leerLinea("Ruta del archivo de candidatos a presidencia: ");
            if (ruta.empty()) {
                std::cout << "Ruta vacia. Operacion cancelada.\n";
                break;
            }
            sistema.cargarCandidatosPresidencia(ruta);
            std::cout << "Proceso de carga solicitado.\n";
            break;
        }
        case 6: {
            std::string ruta = leerLinea("Ruta del archivo de formulas presidenciales: ");
            if (ruta.empty()) {
                std::cout << "Ruta vacia. Operacion cancelada.\n";
                break;
            }
            sistema.cargarFormulasPresidenciales(ruta);
            std::cout << "Proceso de carga solicitado.\n";
            break;
        }
        case 7: {
            std::string nombre = leerLinea("Nombre de la ciudad: ");
            if (nombre.empty()) {
                std::cout << "Nombre vacio. Operacion cancelada.\n";
                break;
            }
            Ciudad* ciudad = sistema.buscarCiudad(nombre);
            if (ciudad) {
                std::cout << "Ciudad encontrada: " << nombre << ".\n";
            } else {
                std::cout << "No se encontro una ciudad con ese nombre.\n";
            }
            break;
        }
        case 8: {
            long id = 0;
            if (!leerNumero("ID del candidato: ", id)) {
                break;
            }
            Candidato* candidato = sistema.buscarCandidato(id);
            if (candidato) {
                std::cout << "Candidato encontrado con ID " << id << ".\n";
            } else {
                std::cout << "No se encontro un candidato con ese ID.\n";
            }
            break;
        }
        case 9:
            sistema.simularVotacion();
            std::cout << "Simulacion solicitada.\n";
            break;
        case 10:
            sistema.calcularGanadoresMunicipales();
            std::cout << "Calculo de ganadores municipales solicitado.\n";
            break;
        case 11:
            sistema.calcularGanadoresRegionales();
            std::cout << "Calculo de ganadores regionales solicitado.\n";
            break;
        case 12:
            sistema.calcularGanadorPresidencial();
            std::cout << "Calculo de ganador presidencial solicitado.\n";
            break;
        case 13: {
            std::string ruta = leerLinea("Ruta del reporte de ciudades: ");
            if (ruta.empty()) {
                std::cout << "Ruta vacia. Operacion cancelada.\n";
                break;
            }
            sistema.generarReporteCiudades(ruta);
            std::cout << "Solicitud de reporte generada.\n";
            break;
        }
        case 14: {
            std::string ruta = leerLinea("Ruta del reporte de regiones: ");
            if (ruta.empty()) {
                std::cout << "Ruta vacia. Operacion cancelada.\n";
                break;
            }
            sistema.generarReporteRegiones(ruta);
            std::cout << "Solicitud de reporte generada.\n";
            break;
        }
        case 15: {
            std::string ruta = leerLinea("Ruta del reporte nacional: ");
            if (ruta.empty()) {
                std::cout << "Ruta vacia. Operacion cancelada.\n";
                break;
            }
            sistema.generarReporteNacional(ruta);
            std::cout << "Solicitud de reporte generada.\n";
            break;
        }
        case 16:
            ejecutarEscenarioDemo(sistema);
            mostrarResumenRegiones(sistema);
            mostrarCandidatosMunicipales(sistema);
            mostrarCandidatosPorPartido(sistema);
            break;
        case 0:
            salir = true;
            break;
        default:
            std::cout << "Opcion no valida. Intenta nuevamente.\n";
            break;
        }
    }

    std::cout << "Hasta pronto.\n";
    return 0;
}
