#include <iostream>
#include <limits>
#include "SistemaElectoral.h"
#include "Ciudad.h"
#include "Candidato.h"

using namespace std;

//Definicion de funciones auxiliares utilizadas en el main
void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausar() {
    cout << "\nPresione Enter para continuar...";
    limpiarBuffer();
}

bool datosDisponibles(bool datosCargados) {
    if (!datosCargados) {
        cout << "\n[ADVERTENCIA] Debe cargar los datos primero (Menu principal -> Gestion de datos).\n";
        return false;
    }
    return true;
}

void mostrarMenuPrincipal() {
    cout << "\n========================================\n";
    cout << "     SISTEMA ELECTORAL NACIONAL\n";
    cout << "========================================\n";
    cout << "1.  Gestion de datos\n";
    cout << "2.  Consultas basicas\n";
    cout << "3.  Simulacion y resultados\n";
    cout << "4.  Tarjetones y candidatos\n";
    cout << "5.  Reportes y estadisticas\n";
    cout << "6.  Herramientas y debug\n";
    cout << "0.  Salir\n";
    cout << "========================================\n";
    cout << "Opcion: ";
}

// funcion para cargar todos los archivos en orden especifico
void cargarDatosCompletos(SistemaElectoral& sistema) {
    cout << "\n| CARGANDO DATOS DEL SISTEMA |\n";

    sistema.cargarPartidos("partidos.txt");
    sistema.cargarRegiones("regiones.txt");
    sistema.cargarCiudades("ciudades.txt");
    sistema.cargarCandidatosAlcaldia("candidatos.txt");
    sistema.cargarCandidatosPresidencia("candidatos.txt");
    sistema.cargarFormulasPresidenciales("formulas.txt");

    cout << "\n| Datos cargados correctamente |\n";
}

// funcion para buscar una ciudad por su respectivo nombre
void buscarCiudadMenu(SistemaElectoral& sistema) {
    cout << "\n--- BUSCAR CIUDAD ---\n";
    cout << "Ingrese el nombre de la ciudad: ";

    string nombre;
    limpiarBuffer();
    getline(cin, nombre);

    Ciudad* ciudad = sistema.buscarCiudad(nombre);

    if (ciudad) {
        cout << "\n[CIUDAD ENCONTRADA]\n";
        cout << "Nombre: " << ciudad->getNombre() << "\n";
        cout << "Censo electoral: " << ciudad->getCenso() << "\n";
        cout << "Total votos registrados: " << ciudad->totalVotos() << "\n";
        cout << "Votos validos: " << ciudad->totalVotosValidos() << "\n";
        cout << "Votos en blanco: " << ciudad->getVotosBlanco() << "\n";
        cout << "Votos nulos: " << ciudad->getVotosNulos() << "\n";
        cout << "Abstencion: " << ciudad->getAbstencion() << "\n";
    } else {
        cout << "\nNo se ha podido encontrar la ciudad...\n";
    }
}

// Buscar un candidato por ID
void buscarCandidatoMenu(SistemaElectoral& sistema) {
    cout << "\n--- BUSCAR CANDIDATO ---\n";
    cout << "Ingrese el ID del candidato: ";

    long id;
    cin >> id;

    if (cin.fail()) {
        cout << "\n ID invalido.\n";
        limpiarBuffer();
        return;
    }

    Candidato* candidato = sistema.buscarCandidato(id);

    if (candidato) {
        cout << "\n[CANDIDATO ENCONTRADO]\n";
        cout << "Nombre: " << candidato->getNombre() << " " << candidato->getApellido() << "\n";
        cout << "ID: " << candidato->getId() << "\n";
        cout << "Sexo: " << candidato->getSexo() << "\n";
        cout << "Ciudad de nacimiento: " << candidato->getCiudadNacimiento() << "\n";

        if (candidato->getCiudadResidencia()) {
            cout << "Ciudad de residencia: " << candidato->getCiudadResidencia()->getNombre() << "\n";
        }

        if (candidato->getPartido()) {
            cout << "Partido: " << candidato->getPartido()->getNombre() << "\n";
        }

        cout << "Tipo: " << (candidato->getTipo() == Candidato::Tipo::ALCALDIA ? "Alcaldia" : "Presidencia") << "\n";
    } else {
        cout << "\n Candidato no encontrado.\n";
    }
}

// Simular elecciones
void simularElecciones(SistemaElectoral& sistema) {
    cout << "\n--- SIMULACION DE ELECCIONES ---\n";
    cout << "Esta accion generara votos aleatorios para todas las ciudades.\n";
    cout << "¿Desea continuar? (S/N): ";

    char respuesta;
    cin >> respuesta;

    if (respuesta == 'S' || respuesta == 's') {
        sistema.simularVotacion();
    } else {
        cout << "Simulacion cancelada.\n";
    }
}

// Ver ganadores municipales
void verGanadoresMunicipales(SistemaElectoral& sistema) {
    cout << "\n--- GANADORES MUNICIPALES ---\n";
    sistema.calcularGanadoresMunicipales();
}

// Ver ganadores regionales
void verGanadoresRegionales(SistemaElectoral& sistema) {
    cout << "\n--- GANADORES REGIONALES ---\n";
    sistema.calcularGanadoresRegionales();
}

// Ver ganador presidencial
void verGanadorPresidencial(SistemaElectoral& sistema) {
    cout << "\n--- GANADOR PRESIDENCIAL ---\n";
    sistema.calcularGanadorPresidencial();
}

// Generar reportes
void generarReporteCiudades(SistemaElectoral& sistema) {
    cout << "\n--- GENERAR REPORTE DE CIUDADES ---\n";
    sistema.generarReporteCiudades("reporte_ciudades.txt");
}

void generarReporteRegiones(SistemaElectoral& sistema) {
    cout << "\n--- GENERAR REPORTE DE REGIONES ---\n";
    sistema.generarReporteRegiones("reporte_regiones.txt");
}

void generarReporteNacional(SistemaElectoral& sistema) {
    cout << "\n--- GENERAR REPORTE NACIONAL ---\n";
    sistema.generarReporteNacional("reporte_nacional.txt");
}

// Reiniciar sistema
void reiniciarSistema(SistemaElectoral& sistema) {
    cout << "\n--- REINICIAR SISTEMA ---\n";
    cout << "Esta accion eliminara todos los datos cargados.\n";
    cout << "¿Desea continuar? (S/N): ";

    char respuesta;
    cin >> respuesta;

    if (respuesta == 'S' || respuesta == 's') {
        sistema.reiniciarEstructuras();
        cout << "Sistema reiniciado exitosamente.\n";
    } else {
        cout << "Operacion cancelada.\n";
    }
}

void mostrarTarjetonAlcaldiaMenu(SistemaElectoral& sistema) {
    cout << "\n--- TARJETON DE ALCALDIA ---\n";
    cout << "Ingrese el nombre de la ciudad: ";
    limpiarBuffer();
    string ciudad;
    getline(cin, ciudad);
    sistema.mostrarTarjetonAlcaldia(ciudad);
}

void mostrarTarjetonPresidencialMenu(SistemaElectoral& sistema) {
    cout << "\n--- TARJETON PRESIDENCIAL ---\n";
    sistema.mostrarTarjetonPresidencial();
}

void listarCandidatosPorPartidoMenu(SistemaElectoral& sistema) {
    cout << "\n--- CANDIDATOS MUNICIPALES POR PARTIDO ---\n";
    cout << "Nombre del partido: ";
    limpiarBuffer();
    string partido;
    getline(cin, partido);
    cout << "Region (dejar vacio para todas): ";
    string region;
    getline(cin, region);
    cout << "Ciudad (dejar vacio para todas): ";
    string ciudad;
    getline(cin, ciudad);
    sistema.listarCandidatosMunicipalesPorPartido(partido, region, ciudad);
}

void listarCandidatosPresidencialesMenu(SistemaElectoral& sistema) {
    cout << "\n--- CANDIDATOS PRESIDENCIALES ---\n";
    sistema.listarCandidatosPresidenciales();
}

void reporteGeneroPresidencialMenu(SistemaElectoral& sistema) {
    cout << "\n--- REPORTE GENERO VS PARTIDO ---\n";
    sistema.reporteGeneroPresidencialPorPartido();
}

void simularSegundaVueltaMenu(SistemaElectoral& sistema) {
    cout << "\n--- SEGUNDA VUELTA PRESIDENCIAL ---\n";
    sistema.simularSegundaVueltaPresidencial();
}

void mostrarEstructuraMenu(SistemaElectoral& sistema) {
    cout << "\n--- VISTA DE ESTRUCTURAS ---\n";
    sistema.mostrarEstructuraDatos();
}

void menuGestionDatos(SistemaElectoral& sistema, bool& datosCargados) {
    int opcion = -1;
    do {
        cout << "\n--- GESTION DE DATOS ---\n";
        cout << "1. Cargar datos desde archivos\n";
        cout << "2. Cargar datos de demostracion\n";
        cout << "3. Reiniciar sistema\n";
        cout << "0. Volver al menu principal\n";
        cout << "Opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cout << "\nIngrese una opcion valida.\n";
            limpiarBuffer();
            pausar();
            continue;
        }

        switch (opcion) {
            case 1:
                cargarDatosCompletos(sistema);
                datosCargados = true;
                pausar();
                break;
            case 2:
                sistema.cargarDatosDemostracion();
                datosCargados = true;
                pausar();
                break;
            case 3:
                reiniciarSistema(sistema);
                datosCargados = false;
                pausar();
                break;
            case 0:
                cout << "Regresando al menu principal...\n";
                break;
            default:
                cout << "Seleccione una opcion valida.\n";
                pausar();
                break;
        }
    } while (opcion != 0);
}

void menuConsultasBasicas(SistemaElectoral& sistema, bool datosCargados) {
    if (!datosDisponibles(datosCargados)) {
        pausar();
        return;
    }

    int opcion = -1;
    do {
        cout << "\n--- CONSULTAS BASICAS ---\n";
        cout << "1. Buscar ciudad\n";
        cout << "2. Buscar candidato por ID\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cout << "\nIngrese una opcion valida.\n";
            limpiarBuffer();
            pausar();
            continue;
        }

        switch (opcion) {
            case 1:
                buscarCiudadMenu(sistema);
                pausar();
                break;
            case 2:
                buscarCandidatoMenu(sistema);
                pausar();
                break;
            case 0:
                cout << "Regresando...\n";
                break;
            default:
                cout << "Seleccione una opcion valida.\n";
                pausar();
                break;
        }
    } while (opcion != 0);
}

void menuSimulacionResultados(SistemaElectoral& sistema, bool datosCargados) {
    if (!datosDisponibles(datosCargados)) {
        pausar();
        return;
    }

    int opcion = -1;
    do {
        cout << "\n--- SIMULACION Y RESULTADOS ---\n";
        cout << "1. Simular votacion completa\n";
        cout << "2. Ver ganadores municipales\n";
        cout << "3. Ver ganadores regionales\n";
        cout << "4. Ver ganador presidencial / Segunda vuelta\n";
        cout << "5. Simular segunda vuelta (si aplica)\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cout << "\nIngrese una opcion valida.\n";
            limpiarBuffer();
            pausar();
            continue;
        }

        switch (opcion) {
            case 1:
                simularElecciones(sistema);
                pausar();
                break;
            case 2:
                verGanadoresMunicipales(sistema);
                pausar();
                break;
            case 3:
                verGanadoresRegionales(sistema);
                pausar();
                break;
            case 4:
                verGanadorPresidencial(sistema);
                pausar();
                break;
            case 5:
                simularSegundaVueltaMenu(sistema);
                pausar();
                break;
            case 0:
                cout << "Regresando...\n";
                break;
            default:
                cout << "Seleccione una opcion valida.\n";
                pausar();
                break;
        }
    } while (opcion != 0);
}

void menuTarjetonesYCandidatos(SistemaElectoral& sistema, bool datosCargados) {
    if (!datosDisponibles(datosCargados)) {
        pausar();
        return;
    }

    int opcion = -1;
    do {
        cout << "\n--- TARJETONES Y LISTADOS ---\n";
        cout << "1. Mostrar tarjeton de alcaldia\n";
        cout << "2. Mostrar tarjeton presidencial\n";
        cout << "3. Listar candidatos municipales por partido/region/ciudad\n";
        cout << "4. Listar candidatos presidenciales\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cout << "\nIngrese una opcion valida.\n";
            limpiarBuffer();
            pausar();
            continue;
        }

        switch (opcion) {
            case 1:
                mostrarTarjetonAlcaldiaMenu(sistema);
                pausar();
                break;
            case 2:
                mostrarTarjetonPresidencialMenu(sistema);
                pausar();
                break;
            case 3:
                listarCandidatosPorPartidoMenu(sistema);
                pausar();
                break;
            case 4:
                listarCandidatosPresidencialesMenu(sistema);
                pausar();
                break;
            case 0:
                cout << "Regresando...\n";
                break;
            default:
                cout << "Seleccione una opcion valida.\n";
                pausar();
                break;
        }
    } while (opcion != 0);
}

void menuReportes(SistemaElectoral& sistema, bool datosCargados) {
    if (!datosDisponibles(datosCargados)) {
        pausar();
        return;
    }

    int opcion = -1;
    do {
        cout << "\n--- REPORTES Y ESTADISTICAS ---\n";
        cout << "1. Generar reporte de ciudades\n";
        cout << "2. Generar reporte de regiones\n";
        cout << "3. Generar reporte nacional\n";
        cout << "4. Reporte genero vs partido (presidencial)\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cout << "\nIngrese una opcion valida.\n";
            limpiarBuffer();
            pausar();
            continue;
        }

        switch (opcion) {
            case 1:
                generarReporteCiudades(sistema);
                pausar();
                break;
            case 2:
                generarReporteRegiones(sistema);
                pausar();
                break;
            case 3:
                generarReporteNacional(sistema);
                pausar();
                break;
            case 4:
                reporteGeneroPresidencialMenu(sistema);
                pausar();
                break;
            case 0:
                cout << "Regresando...\n";
                break;
            default:
                cout << "Seleccione una opcion valida.\n";
                pausar();
                break;
        }
    } while (opcion != 0);
}

void menuHerramientas(SistemaElectoral& sistema, bool datosCargados) {
    if (!datosDisponibles(datosCargados)) {
        pausar();
        return;
    }

    int opcion = -1;
    do {
        cout << "\n--- HERRAMIENTAS Y DEBUG ---\n";
        cout << "1. Mostrar estructuras en memoria\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cout << "\nIngrese una opcion valida.\n";
            limpiarBuffer();
            pausar();
            continue;
        }

        switch (opcion) {
            case 1:
                mostrarEstructuraMenu(sistema);
                pausar();
                break;
            case 0:
                cout << "Regresando...\n";
                break;
            default:
                cout << "Seleccione una opcion valida.\n";
                pausar();
                break;
        }
    } while (opcion != 0);
}

int main() {
    SistemaElectoral sistema;
    int opcion;
    bool datosCargados = false;

    cout << "========================================\n";
    cout << "  BIENVENIDO AL SISTEMA ELECTORAL\n";
    cout << "========================================\n";
    cout << "\nPor favor, cargue los datos antes de usar el sistema.\n";

    do {
        mostrarMenuPrincipal();
        cin >> opcion;

        // Validar entrada del buffer para no reiniciar todo el programa por un error de ingreso
        if (cin.fail()) {
            cout << "\nSelecciones una opcion valida (numero).\n";
            limpiarBuffer();
            pausar();
            continue;
        }

        switch(opcion) {
            case 1:
                menuGestionDatos(sistema, datosCargados);
                break;

            case 2:
                menuConsultasBasicas(sistema, datosCargados);
                break;

            case 3:
                menuSimulacionResultados(sistema, datosCargados);
                break;

            case 4:
                menuTarjetonesYCandidatos(sistema, datosCargados);
                break;

            case 5:
                menuReportes(sistema, datosCargados);
                break;

            case 6:
                menuHerramientas(sistema, datosCargados);
                break;

            case 0:
                cout << "\n |==== Gracias por usar el Sistema Electoral ====|\n";

                break;

            default:
                cout << "\nIntente nuevamente con una opcion valida.\n";
                pausar();
        }

    } while(opcion != 0);

    return 0;
}
