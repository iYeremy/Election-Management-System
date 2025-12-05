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

void mostrarMenuPrincipal() {
    cout << "\n========================================\n";
    cout << "     SISTEMA ELECTORAL NACIONAL\n";
    cout << "========================================\n";
    cout << "1.  Cargar datos desde archivos\n";
    cout << "2.  Cargar datos de demostracion\n";
    cout << "3.  Buscar ciudad\n";
    cout << "4.  Buscar candidato por ID\n";
    cout << "5.  Simular votacion\n";
    cout << "6.  Ver ganadores municipales\n";
    cout << "7.  Ver ganadores regionales\n";
    cout << "8.  Ver ganador presidencial\n";
    cout << "9.  Generar reporte de ciudades\n";
    cout << "10. Generar reporte de regiones\n";
    cout << "11. Generar reporte nacional\n";
    cout << "12. Reiniciar sistema\n";
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
                if (!datosCargados) {
                    cout << "\n[ADVERTENCIA] Debe cargar los datos primero (opcion 1 o 2).\n";
                } else {
                    buscarCiudadMenu(sistema);
                }
                pausar();
                break;

            case 4:
                if (!datosCargados) {
                    cout << "\n[ADVERTENCIA] Debe cargar los datos primero (opcion 1 o 2).\n";
                } else {
                    buscarCandidatoMenu(sistema);
                }
                pausar();
                break;

            case 5:
                if (!datosCargados) {
                    cout << "\n[ADVERTENCIA] Debe cargar los datos primero (opcion 1 o 2).\n";
                } else {
                    simularElecciones(sistema);
                }
                pausar();
                break;

            case 6:
                if (!datosCargados) {
                    cout << "\n[ADVERTENCIA] Debe cargar los datos primero (opcion 1 o 2).\n";
                } else {
                    verGanadoresMunicipales(sistema);
                }
                pausar();
                break;

            case 7:
                if (!datosCargados) {
                    cout << "\n[ADVERTENCIA] Debe cargar los datos primero (opcion 1 o 2).\n";
                } else {
                    verGanadoresRegionales(sistema);
                }
                pausar();
                break;

            case 8:
                if (!datosCargados) {
                    cout << "\n[ADVERTENCIA] Debe cargar los datos primero (opcion 1 o 2).\n";
                } else {
                    verGanadorPresidencial(sistema);
                }
                pausar();
                break;

            case 9:
                if (!datosCargados) {
                    cout << "\n[ADVERTENCIA] Debe cargar los datos primero (opcion 1 o 2).\n";
                } else {
                    generarReporteCiudades(sistema);
                }
                pausar();
                break;

            case 10:
                if (!datosCargados) {
                    cout << "\n[ADVERTENCIA] Debe cargar los datos primero (opcion 1 o 2).\n";
                } else {
                    generarReporteRegiones(sistema);
                }
                pausar();
                break;

            case 11:
                if (!datosCargados) {
                    cout << "\n[ADVERTENCIA] Debe cargar los datos primero (opcion 1 o 2).\n";
                } else {
                    generarReporteNacional(sistema);
                }
                pausar();
                break;

            case 12:
                reiniciarSistema(sistema);
                datosCargados = false;
                pausar();
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