#include "SistemaElectoral_Util.h"

#include <ctime>
#include <fstream>
#include <sstream>

#include "MultilistaRegiones.h"
#include "Region.h"
#include "Ciudad.h"
#include "Partido.h"
#include "Fecha.h"

namespace SistemaElectoralInterno {

Region* buscarRegionPorNombre(MultilistaRegiones* lista, const std::string& nombre) {
    if (!lista) {
        return nullptr;
    }
    Region* actual = lista->getCabeza();
    while (actual) {
        if (actual->getNombre() == nombre) {
            return actual;
        }
        actual = actual->getSigRegion();
    }
    return nullptr;
}

bool hayRegionesCargadas(MultilistaRegiones* lista) {
    return lista && lista->getCabeza();
}

Partido* buscarPartidoPorNombre(Partido partidos[], const std::string& nombre) {
    if (nombre.empty()) {
        return nullptr;
    }
    for (int i = 0; i < 5; ++i) {
        if (partidos[i].getNombre() == nombre) {
            return &partidos[i];
        }
    }
    return nullptr;
}

bool hayCiudadesRegistradas(MultilistaRegiones* lista) {
    if (!lista) {
        return false;
    }
    Region* region = lista->getCabeza();
    while (region) {
        if (region->getListaCiudades()) {
            return true;
        }
        region = region->getSigRegion();
    }
    return false;
}

int calcularEdad(const Fecha& fecha) {
    std::time_t t = std::time(nullptr);
    std::tm actual = *std::localtime(&t);
    int edad = (actual.tm_year + 1900) - fecha.getYear();
    int mesActual = actual.tm_mon + 1;
    int diaActual = actual.tm_mday;
    if (mesActual < fecha.getMes() ||
        (mesActual == fecha.getMes() && diaActual < fecha.getDia())) {
        --edad;
    }
    return edad;
}

bool leerRegistrosArchivo(const std::string& ruta, std::vector<RegistroPlanoCandidato>& registros) {
    registros.clear();
    std::ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        return false;
    }
    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') {
            continue;
        }
        std::stringstream ss(linea);
        RegistroPlanoCandidato reg;
        std::string idStr, diaStr, mesStr, anioStr, tipoStr, viceStr;
        std::string sexoStr;
        std::getline(ss, reg.nombre, '|');
        std::getline(ss, reg.apellido, '|');
        std::getline(ss, idStr, '|');
        std::getline(ss, sexoStr, '|');
        std::getline(ss, reg.estadoCivil, '|');
        std::getline(ss, diaStr, '|');
        std::getline(ss, mesStr, '|');
        std::getline(ss, anioStr, '|');
        std::getline(ss, reg.ciudadNacimiento, '|');
        std::getline(ss, reg.ciudadResidencia, '|');
        std::getline(ss, reg.partido, '|');
        std::getline(ss, tipoStr, '|');
        std::getline(ss, viceStr, '|');
        reg.id = std::atol(idStr.c_str());
        reg.sexo = sexoStr.empty() ? ' ' : sexoStr[0];
        reg.dia = std::atoi(diaStr.c_str());
        reg.mes = std::atoi(mesStr.c_str());
        reg.anio = std::atoi(anioStr.c_str());
        reg.tipo = tipoStr.empty() ? 'A' : tipoStr[0];
        reg.esVice = !viceStr.empty() && (viceStr == "1" || viceStr == "V" || viceStr == "v");
        registros.push_back(reg);
    }
    return true;
}

bool escribirRegistrosArchivo(const std::string& ruta, const std::vector<RegistroPlanoCandidato>& registros) {
    std::ofstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        return false;
    }
    for (const auto& reg : registros) {
        archivo << reg.nombre << '|'
                << reg.apellido << '|'
                << reg.id << '|'
                << reg.sexo << '|'
                << reg.estadoCivil << '|'
                << reg.dia << '|'
                << reg.mes << '|'
                << reg.anio << '|'
                << reg.ciudadNacimiento << '|'
                << reg.ciudadResidencia << '|'
                << reg.partido << '|'
                << reg.tipo << '|'
                << (reg.esVice ? 1 : 0) << '\n';
    }
    return true;
}

}  // namespace SistemaElectoralInterno
