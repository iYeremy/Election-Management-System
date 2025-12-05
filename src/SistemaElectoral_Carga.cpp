#include "SistemaElectoral.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

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
#include "SistemaElectoral_Util.h"

using namespace SistemaElectoralInterno;

void SistemaElectoral::cargarRegiones(const std::string& ruta) {
    rutaRegiones = ruta;
    if (!regiones) {
        regiones = new MultilistaRegiones();
    } else if (regiones->getCabeza()) {
        std::cout << "Advertencia: ya existen regiones cargadas. Reinicia antes de volver a cargar.\n";
        return;
    }

    std::ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir " << ruta << std::endl;
        return;
    }

    std::string linea;
    int contador = 0;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') {
            continue;
        }

        if (buscarRegionPorNombre(regiones, linea)) {
            std::cout << "Advertencia: Region duplicada " << linea << ", se omite.\n";
            continue;
        }

        Region* region = new Region(linea, 0);
        regiones->insertarRegion(region);
        ++contador;
    }

    archivo.close();
    std::cout << "Regiones cargadas: " << contador << std::endl;
}

void SistemaElectoral::cargarCiudades(const std::string& ruta) {
    rutaCiudades = ruta;
    if (!regiones || !regiones->getCabeza()) {
        std::cout << "Error: Cargue las regiones antes de las ciudades.\n";
        return;
    }

    std::ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir " << ruta << std::endl;
        return;
    }

    std::string linea;
    int contador = 0;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string nombreCiudad, nombreRegion, censoStr;

        std::getline(ss, nombreCiudad, '|');
        std::getline(ss, nombreRegion, '|');
        std::getline(ss, censoStr, '|');

        if (nombreCiudad.empty() || nombreRegion.empty()) {
            continue;
        }

        if (arbolCiudades && arbolCiudades->buscar(nombreCiudad)) {
            std::cout << "Advertencia: Ciudad duplicada " << nombreCiudad << ", se omite.\n";
            continue;
        }

        Region* destino = buscarRegionPorNombre(regiones, nombreRegion);
        if (!destino) {
            std::cout << "Advertencia: Region " << nombreRegion << " no encontrada para " << nombreCiudad << "\n";
            continue;
        }

        int censo = std::atoi(censoStr.c_str());
        Ciudad* ciudad = new Ciudad(nombreCiudad, censo);

        destino->agregarCiudad(ciudad);
        arbolCiudades->insertar(ciudad);

        contador++;
    }

    archivo.close();
    std::cout << "Ciudades cargadas: " << contador << std::endl;
}

void SistemaElectoral::cargarPartidos(const std::string& ruta) {
    rutaPartidos = ruta;
    if (!partidos[0].getNombre().empty()) {
        std::cout << "Advertencia: los partidos ya fueron cargados.\n";
        return;
    }

    std::ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir " << ruta << std::endl;
        return;
    }

    std::string linea;
    int contador = 0;

    while (std::getline(archivo, linea) && contador < 5) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string nombre;
        std::string representante;

        std::getline(ss, nombre, '|');
        std::getline(ss, representante, '|');

        if (nombre.empty()) continue;

        partidos[contador].setNombre(nombre);
        partidos[contador].setRepresentante(representante);
        contador++;
    }

    archivo.close();
    std::cout << "Partidos cargados: " << contador << std::endl;
}

void SistemaElectoral::cargarCandidatosAlcaldia(const std::string& ruta) {
    rutaCandidatos = ruta;
    if (!arbolCiudades || !hayCiudadesRegistradas(regiones)) {
        std::cout << "Error: No hay ciudades cargadas para asignar candidatos.\n";
        return;
    }
    if (partidos[0].getNombre().empty()) {
        std::cout << "Error: Cargue los partidos antes de los candidatos municipales.\n";
        return;
    }

    std::ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir " << ruta << std::endl;
        return;
    }

    std::string linea;
    int contador = 0;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string nombre, apellido, idStr, sexoStr, estadoCivil;
        std::string diaStr, mesStr, anioStr;
        std::string ciudadNac, ciudadRes, nombrePartido, tipoStr, viceStr;

        std::getline(ss, nombre, '|');
        std::getline(ss, apellido, '|');
        std::getline(ss, idStr, '|');
        std::getline(ss, sexoStr, '|');
        std::getline(ss, estadoCivil, '|');
        std::getline(ss, diaStr, '|');
        std::getline(ss, mesStr, '|');
        std::getline(ss, anioStr, '|');
        std::getline(ss, ciudadNac, '|');
        std::getline(ss, ciudadRes, '|');
        std::getline(ss, nombrePartido, '|');
        std::getline(ss, tipoStr, '|');
        std::getline(ss, viceStr, '|');
        (void)viceStr;

        if (tipoStr != "A") continue;

        long id = std::atol(idStr.c_str());
        if (arbolCandidatos->buscar(id)) {
            std::cout << "Advertencia: ID de candidato repetido " << id << "\n";
            continue;
        }

        char sexo = sexoStr.empty() ? ' ' : sexoStr[0];
        int dia = std::atoi(diaStr.c_str());
        int mes = std::atoi(mesStr.c_str());
        int anio = std::atoi(anioStr.c_str());

        Fecha fecha(dia, mes, anio);

        Ciudad* ciudad = arbolCiudades->buscar(ciudadRes);
        if (!ciudad) {
            std::cout << "Advertencia: Ciudad " << ciudadRes << " no encontrada para candidato " << nombre << "\n";
            continue;
        }

        Partido* partido = buscarPartidoPorNombre(partidos, nombrePartido);
        if (!partido) {
            std::cout << "Advertencia: Partido " << nombrePartido << " no encontrado\n";
            continue;
        }

        NodoCandidato* nodo = new NodoCandidato();
        Candidato* candidato = new Candidato(
            nombre, apellido, id, sexo, estadoCivil, fecha,
            ciudadNac, ciudad, partido,
            Candidato::Tipo::ALCALDIA, nodo
        );

        arbolCandidatos->insertar(candidato);
        ciudad->agregarCandidatoAlcaldia(candidato);
        partido->agregarCandidatoAlcaldia(candidato);

        contador++;
    }

    archivo.close();
    std::cout << "Candidatos a alcaldia cargados: " << contador << std::endl;
}

void SistemaElectoral::cargarCandidatosPresidencia(const std::string& ruta) {
    rutaCandidatos = ruta;
    if (!arbolCiudades || !hayCiudadesRegistradas(regiones)) {
        std::cout << "Error: No hay ciudades registradas para los candidatos presidenciales.\n";
        return;
    }
    if (partidos[0].getNombre().empty()) {
        std::cout << "Error: Cargue los partidos antes de los candidatos presidenciales.\n";
        return;
    }

    std::ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir " << ruta << std::endl;
        return;
    }

    std::string linea;
    int contador = 0;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string nombre, apellido, idStr, sexoStr, estadoCivil;
        std::string diaStr, mesStr, anioStr;
        std::string ciudadNac, ciudadRes, nombrePartido, tipoStr, viceStr;

        std::getline(ss, nombre, '|');
        std::getline(ss, apellido, '|');
        std::getline(ss, idStr, '|');
        std::getline(ss, sexoStr, '|');
        std::getline(ss, estadoCivil, '|');
        std::getline(ss, diaStr, '|');
        std::getline(ss, mesStr, '|');
        std::getline(ss, anioStr, '|');
        std::getline(ss, ciudadNac, '|');
        std::getline(ss, ciudadRes, '|');
        std::getline(ss, nombrePartido, '|');
        std::getline(ss, tipoStr, '|');
        std::getline(ss, viceStr, '|');

        if (tipoStr != "P") continue;

        long id = std::atol(idStr.c_str());
        if (arbolCandidatos->buscar(id)) {
            std::cout << "Advertencia: ID de candidato repetido " << id << "\n";
            continue;
        }

        char sexo = sexoStr.empty() ? ' ' : sexoStr[0];
        int dia = std::atoi(diaStr.c_str());
        int mes = std::atoi(mesStr.c_str());
        int anio = std::atoi(anioStr.c_str());
        bool esVice = (viceStr == "1" || viceStr == "V" || viceStr == "v");

        Fecha fecha(dia, mes, anio);

        Ciudad* ciudad = arbolCiudades ? arbolCiudades->buscar(ciudadRes) : nullptr;
        if (!ciudad) {
            std::cout << "Advertencia: Ciudad " << ciudadRes << " no encontrada para candidato " << nombre << "\n";
            continue;
        }

        Partido* partido = buscarPartidoPorNombre(partidos, nombrePartido);
        if (!partido) {
            std::cout << "Advertencia: Partido " << nombrePartido << " no encontrado\n";
            continue;
        }

        NodoCandidato* nodo = new NodoCandidato();
        Candidato* candidato = new Candidato(
            nombre, apellido, id, sexo, estadoCivil, fecha,
            ciudadNac, ciudad, partido,
            Candidato::Tipo::PRESIDENCIA, nodo, esVice
        );

        arbolCandidatos->insertar(candidato);
        contador++;
    }

    archivo.close();
    std::cout << "Candidatos presidenciales cargados: " << contador << std::endl;
}

void SistemaElectoral::cargarFormulasPresidenciales(const std::string& ruta) {
    rutaFormulas = ruta;
    if (!arbolCandidatos) {
        std::cout << "Error: no hay arbol de candidatos inicializado.\n";
        return;
    }

    std::ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir " << ruta << std::endl;
        return;
    }

    std::string linea;
    int contador = 0;

    while (std::getline(archivo, linea) && contador < 5) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string idPresStr, idViceStr, numeroPartidoStr;
        std::getline(ss, idPresStr, '|');
        std::getline(ss, idViceStr, '|');
        std::getline(ss, numeroPartidoStr, '|');

        long idPres = std::atol(idPresStr.c_str());
        long idVice = std::atol(idViceStr.c_str());
        int numPartido = std::atoi(numeroPartidoStr.c_str());

        Candidato* presidente = arbolCandidatos->buscar(idPres);
        Candidato* vicepresidente = arbolCandidatos->buscar(idVice);
        if (!presidente || !vicepresidente) {
            std::cout << "Advertencia: formula con IDs " << idPres << " y " << idVice << " no valida.\n";
            continue;
        }
        if (presidente->esVicepresidente()) {
            std::cout << "Advertencia: el ID " << idPres << " esta marcado como vicepresidente.\n";
            continue;
        }
        if (!vicepresidente->esVicepresidente()) {
            std::cout << "Advertencia: el ID " << idVice << " no esta marcado como vicepresidente.\n";
            continue;
        }

        formulas[contador].setPresidente(presidente);
        formulas[contador].setVicepresidente(vicepresidente);
        formulas[contador].setPartido(presidente->getPartido());
        formulas[contador].setNumeroPartido(numPartido);
        formulas[contador].setVotos(0);

        if (numPartido >= 0 && numPartido < 5) {
            partidos[numPartido].asignarFormulaPresidencial(&formulas[contador]);
        }

        contador++;
    }

    archivo.close();
    std::cout << "Formulas presidenciales cargadas: " << contador << std::endl;
}
