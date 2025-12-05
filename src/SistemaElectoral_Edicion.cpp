#include "SistemaElectoral.h"

#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <fstream>

#include "AVLCandidatos.h"
#include "Ciudad.h"
#include "Partido.h"
#include "Fecha.h"
#include "Candidato.h"
#include "SistemaElectoral_Util.h"

using namespace SistemaElectoralInterno;

bool SistemaElectoral::puedeEditar() const {
    return !rutaPartidos.empty() && !rutaRegiones.empty() && !rutaCiudades.empty() &&
           !rutaCandidatos.empty() && !rutaFormulas.empty();
}

bool SistemaElectoral::simulacionEjecutada() const {
    return simulacionEjecutadaFlag;
}

bool SistemaElectoral::recargarDesdeArchivosInterno() {
    if (!puedeEditar()) {
        std::cout << "Los datos actuales no provienen de archivos editables.\n";
        return false;
    }

    std::string rutaP = rutaPartidos;
    std::string rutaR = rutaRegiones;
    std::string rutaCiu = rutaCiudades;
    std::string rutaCand = rutaCandidatos;
    std::string rutaFor = rutaFormulas;

    reiniciarEstructuras();

    rutaPartidos = rutaP;
    rutaRegiones = rutaR;
    rutaCiudades = rutaCiu;
    rutaCandidatos = rutaCand;
    rutaFormulas = rutaFor;

    cargarPartidos(rutaPartidos);
    cargarRegiones(rutaRegiones);
    cargarCiudades(rutaCiudades);
    cargarCandidatosAlcaldia(rutaCandidatos);
    cargarCandidatosPresidencia(rutaCandidatos);
    cargarFormulasPresidenciales(rutaFormulas);
    return true;
}

bool SistemaElectoral::actualizarArchivosDesdeMemoria() {
    if (!puedeEditar() || !arbolCandidatos) {
        return false;
    }

    std::vector<Candidato*> candidatos;
    arbolCandidatos->recolectarInorden(candidatos);
    std::ofstream archivo(rutaCandidatos.c_str());
    if (!archivo.is_open()) {
        return false;
    }

    for (Candidato* candidato : candidatos) {
        if (!candidato) {
            continue;
        }
        const Fecha& fecha = candidato->getFechaNacimiento();
        Ciudad* residencia = candidato->getCiudadResidencia();
        Partido* partido = candidato->getPartido();
        char tipo = candidato->getTipo() == Candidato::Tipo::ALCALDIA ? 'A' : 'P';

        archivo << candidato->getNombre() << '|'
                << candidato->getApellido() << '|'
                << candidato->getId() << '|'
                << candidato->getSexo() << '|'
                << candidato->getEstadoCivil() << '|'
                << fecha.getDia() << '|'
                << fecha.getMes() << '|'
                << fecha.getYear() << '|'
                << candidato->getCiudadNacimiento() << '|'
                << (residencia ? residencia->getNombre() : "") << '|'
                << (partido ? partido->getNombre() : "") << '|'
                << tipo << '|'
                << (candidato->esVicepresidente() ? 1 : 0) << '\n';
    }

    archivo.close();
    return true;
}

void SistemaElectoral::agregarCandidatoAlcaldia() {
    if (simulacionEnCurso || simulacionEjecutadaFlag) {
        std::cout << "No puede agregar candidatos una vez iniciada la simulacion. Reinicie las estructuras para continuar.\n";
        return;
    }
    if (!puedeEditar()) {
        std::cout << "Debe cargar datos desde archivos para poder editarlos.\n";
        return;
    }

    std::vector<RegistroPlanoCandidato> registros;
    if (!leerRegistrosArchivo(rutaCandidatos, registros)) {
        std::cout << "No se pudo leer " << rutaCandidatos << " para editar.\n";
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    auto leerLinea = [](const std::string& prompt) {
        std::string valor;
        std::cout << prompt;
        std::getline(std::cin, valor);
        return valor;
    };

    RegistroPlanoCandidato nuevo;
    nuevo.tipo = 'A';
    nuevo.esVice = false;

    nuevo.nombre = leerLinea("Nombre: ");
    nuevo.apellido = leerLinea("Apellido: ");
    std::string idStr = leerLinea("Numero de identificacion: ");
    nuevo.id = std::atol(idStr.c_str());
    if (nuevo.id <= 0) {
        std::cout << "ID invalido.\n";
        return;
    }
    for (const auto& reg : registros) {
        if (reg.id == nuevo.id) {
            std::cout << "Ya existe un candidato con ese ID.\n";
            return;
        }
    }
    std::string sexoStr = leerLinea("Sexo (M/F): ");
    nuevo.sexo = sexoStr.empty() ? ' ' : sexoStr[0];
    nuevo.estadoCivil = leerLinea("Estado civil: ");
    std::string fechaStr = leerLinea("Fecha de nacimiento (DD/MM/AAAA): ");
    int d = 0, m = 0, a = 0;
    if (sscanf(fechaStr.c_str(), "%d/%d/%d", &d, &m, &a) != 3) {
        std::cout << "Formato de fecha invalido.\n";
        return;
    }
    nuevo.dia = d;
    nuevo.mes = m;
    nuevo.anio = a;
    nuevo.ciudadNacimiento = leerLinea("Ciudad de nacimiento: ");
    nuevo.ciudadResidencia = leerLinea("Ciudad de residencia: ");
    Ciudad* ciudad = buscarCiudad(nuevo.ciudadResidencia);
    if (!ciudad) {
        std::cout << "La ciudad indicada no existe en el sistema.\n";
        return;
    }
    nuevo.partido = leerLinea("Partido (dejar vacio si es independiente): ");
    if (!nuevo.partido.empty() && !buscarPartidoPorNombre(partidos, nuevo.partido)) {
        std::cout << "Partido no encontrado.\n";
        return;
    }

    registros.push_back(nuevo);
    if (!escribirRegistrosArchivo(rutaCandidatos, registros)) {
        std::cout << "No se pudo actualizar " << rutaCandidatos << ".\n";
        return;
    }

    if (recargarDesdeArchivosInterno()) {
        std::cout << "Candidato agregado correctamente.\n";
    }
}

void SistemaElectoral::eliminarCandidato() {
    if (simulacionEnCurso || simulacionEjecutadaFlag) {
        std::cout << "No puede eliminar candidatos despues de iniciar la simulacion. Reinicie las estructuras para continuar.\n";
        return;
    }
    if (!puedeEditar()) {
        std::cout << "Debe cargar datos desde archivos para poder editarlos.\n";
        return;
    }

    std::vector<RegistroPlanoCandidato> registros;
    if (!leerRegistrosArchivo(rutaCandidatos, registros)) {
        std::cout << "No se pudo leer " << rutaCandidatos << " para editar.\n";
        return;
    }

    long id;
    std::cout << "Ingrese el ID del candidato a eliminar: ";
    std::cin >> id;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ID invalido.\n";
        return;
    }

    auto it = std::find_if(registros.begin(), registros.end(),
                           [id](const RegistroPlanoCandidato& reg) { return reg.id == id; });
    if (it == registros.end()) {
        std::cout << "No se encontro un candidato con ese ID.\n";
        return;
    }
    if (it->tipo != 'A') {
        std::cout << "Por ahora solo se pueden editar candidatos a alcaldia.\n";
        return;
    }

    registros.erase(it);
    if (!escribirRegistrosArchivo(rutaCandidatos, registros)) {
        std::cout << "No se pudo actualizar " << rutaCandidatos << ".\n";
        return;
    }

    if (recargarDesdeArchivosInterno()) {
        std::cout << "Candidato eliminado correctamente.\n";
    }
}

void SistemaElectoral::modificarCandidato() {
    if (simulacionEnCurso || simulacionEjecutadaFlag) {
        std::cout << "No puede modificar candidatos despues de iniciar la simulacion. Reinicie las estructuras para continuar.\n";
        return;
    }
    if (!puedeEditar()) {
        std::cout << "Debe cargar datos desde archivos para poder editarlos.\n";
        return;
    }

    std::vector<RegistroPlanoCandidato> registros;
    if (!leerRegistrosArchivo(rutaCandidatos, registros)) {
        std::cout << "No se pudo leer " << rutaCandidatos << " para editar.\n";
        return;
    }

    long id;
    std::cout << "Ingrese el ID del candidato a modificar: ";
    std::cin >> id;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ID invalido.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto it = std::find_if(registros.begin(), registros.end(),
                           [id](const RegistroPlanoCandidato& reg) { return reg.id == id; });
    if (it == registros.end()) {
        std::cout << "No se encontro un candidato con ese ID.\n";
        return;
    }
    if (it->tipo != 'A') {
        std::cout << "Por ahora solo se pueden editar candidatos a alcaldia.\n";
        return;
    }

    auto leerLinea = [](const std::string& prompt) {
        std::string valor;
        std::cout << prompt;
        std::getline(std::cin, valor);
        return valor;
    };

    std::string entrada = leerLinea("Nuevo nombre (" + it->nombre + "): ");
    if (!entrada.empty()) it->nombre = entrada;
    entrada = leerLinea("Nuevo apellido (" + it->apellido + "): ");
    if (!entrada.empty()) it->apellido = entrada;
    entrada = leerLinea("Nuevo sexo (" + std::string(1, it->sexo) + "): ");
    if (!entrada.empty()) it->sexo = entrada[0];
    entrada = leerLinea("Nuevo estado civil (" + it->estadoCivil + "): ");
    if (!entrada.empty()) it->estadoCivil = entrada;
    entrada = leerLinea("Nueva fecha (DD/MM/AAAA) (actual " + std::to_string(it->dia) + "/" +
                        std::to_string(it->mes) + "/" + std::to_string(it->anio) + "): ");
    if (!entrada.empty()) {
        int d = 0, m = 0, a = 0;
        if (sscanf(entrada.c_str(), "%d/%d/%d", &d, &m, &a) == 3) {
            it->dia = d;
            it->mes = m;
            it->anio = a;
        } else {
            std::cout << "Formato de fecha invalido. Se mantiene la anterior.\n";
        }
    }
    entrada = leerLinea("Nueva ciudad de nacimiento (" + it->ciudadNacimiento + "): ");
    if (!entrada.empty()) it->ciudadNacimiento = entrada;
    entrada = leerLinea("Nueva ciudad de residencia (" + it->ciudadResidencia + "): ");
    if (!entrada.empty()) {
        if (!buscarCiudad(entrada)) {
            std::cout << "La ciudad indicada no existe. Se mantiene la anterior.\n";
        } else {
            it->ciudadResidencia = entrada;
        }
    }
    entrada = leerLinea("Nuevo partido (" + (it->partido.empty() ? "Independiente" : it->partido) + "): ");
    if (!entrada.empty()) {
        if (!buscarPartidoPorNombre(partidos, entrada)) {
            std::cout << "Partido no encontrado. Se mantiene el anterior.\n";
        } else {
            it->partido = entrada;
        }
    }

    if (!escribirRegistrosArchivo(rutaCandidatos, registros)) {
        std::cout << "No se pudo actualizar " << rutaCandidatos << ".\n";
        return;
    }

    if (recargarDesdeArchivosInterno()) {
        std::cout << "Candidato modificado correctamente.\n";
    }
}

void SistemaElectoral::guardarCambios() {
    if (!puedeEditar()) {
        std::cout << "Los datos actuales no provienen de archivos editables.\n";
        return;
    }
    std::cout << "Los cambios se guardan automaticamente en " << rutaCandidatos << ".\n";
}
