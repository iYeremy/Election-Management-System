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

// -----------------------------
// DATOS DE DEMOSTRACION
// -----------------------------
void SistemaElectoral::cargarDatosDemostracion() {
    rutaPartidos.clear();
    rutaRegiones.clear();
    rutaCiudades.clear();
    rutaCandidatos.clear();
    rutaFormulas.clear();
    std::cout << "\nCargando datos de demostracion...\n";
    reiniciarEstructuras();

    const int totalRegiones = 2;
    const char* nombresRegiones[totalRegiones] = { "Region Andina", "Region Caribe" };
    Region* regionesDemo[totalRegiones];

    for (int i = totalRegiones - 1; i >= 0; --i) {
        regionesDemo[i] = new Region(nombresRegiones[i], 0);
        regiones->insertarRegion(regionesDemo[i]);
    }

    struct CiudadDemo {
        const char* nombre;
        int censo;
        int regionIdx;
    };

    const CiudadDemo ciudadesInfo[] = {
        {"Bogota", 120, 0},
        {"Medellin", 90, 0},
        {"Barranquilla", 80, 1},
        {"Cartagena", 70, 1}
    };
    const int totalCiudades = static_cast<int>(sizeof(ciudadesInfo) / sizeof(ciudadesInfo[0]));
    Ciudad* ciudadesDemo[totalCiudades];

    for (int i = 0; i < totalCiudades; ++i) {
        const CiudadDemo& info = ciudadesInfo[i];
        Ciudad* ciudad = new Ciudad(info.nombre, info.censo);
        regionesDemo[info.regionIdx]->agregarCiudad(ciudad);
        arbolCiudades->insertar(ciudad);
        ciudadesDemo[i] = ciudad;
    }

    const char* nombresPartidos[] = {
        "Movimiento Aurora",
        "Camino Verde",
        "Frente Ciudadano"
    };
    const char* representantes[] = {
        "Carolina Ruiz",
        "Luis Pardo",
        "Angela Torres"
    };
    const int totalPartidos = 3;
    for (int i = 0; i < totalPartidos; ++i) {
        partidos[i].setNombre(nombresPartidos[i]);
        partidos[i].setRepresentante(representantes[i]);
    }

    struct CandidatoMunicipalDemo {
        const char* nombre;
        const char* apellido;
        long id;
        char sexo;
        const char* estadoCivil;
        int dia;
        int mes;
        int anio;
        const char* ciudadNacimiento;
        int ciudadResidenciaIdx;
        int partidoIdx;
    };

    const CandidatoMunicipalDemo candidatosMunicipales[] = {
        {"Laura", "Diaz", 2001, 'F', "Casada", 15, 3, 1985, "Bogota", 0, 0},
        {"Mateo", "Rivera", 2005, 'M', "Soltero", 22, 7, 1982, "Bogota", 0, 1},
        {"Carlos", "Lopez", 2002, 'M', "Union libre", 1, 5, 1979, "Medellin", 1, 2},
        {"Juliana", "Morales", 2006, 'F', "Divorciada", 9, 11, 1988, "Medellin", 1, 0},
        {"Andres", "Soto", 2003, 'M', "Casado", 5, 1, 1980, "Barranquilla", 2, 0},
        {"Paula", "Herrera", 2007, 'F', "Soltera", 18, 9, 1986, "Barranquilla", 2, 2},
        {"Ricardo", "Franco", 2004, 'M', "Casado", 30, 4, 1975, "Cartagena", 3, 1},
        {"Elena", "Vega", 2008, 'F', "Union libre", 14, 6, 1983, "Cartagena", 3, 0}
    };
    const int totalCandidatosMunicipales =
        static_cast<int>(sizeof(candidatosMunicipales) / sizeof(candidatosMunicipales[0]));

    for (int i = 0; i < totalCandidatosMunicipales; ++i) {
        const CandidatoMunicipalDemo& info = candidatosMunicipales[i];
        Ciudad* ciudad = ciudadesDemo[info.ciudadResidenciaIdx];
        Partido* partido = (info.partidoIdx >= 0 && info.partidoIdx < totalPartidos)
                               ? &partidos[info.partidoIdx]
                               : nullptr;

        NodoCandidato* nodo = new NodoCandidato();
        Fecha fecha(info.dia, info.mes, info.anio);
        Candidato* candidato = new Candidato(
            info.nombre,
            info.apellido,
            info.id,
            info.sexo,
            info.estadoCivil,
            fecha,
            info.ciudadNacimiento,
            ciudad,
            partido,
            Candidato::Tipo::ALCALDIA,
            nodo
        );

        arbolCandidatos->insertar(candidato);
        ciudad->agregarCandidatoAlcaldia(candidato);
        if (partido) {
            partido->agregarCandidatoAlcaldia(candidato);
        }
    }

    struct CandidatoPresidencialDemo {
        const char* nombre;
        const char* apellido;
        long id;
        char sexo;
        const char* estadoCivil;
        int dia;
        int mes;
        int anio;
        const char* ciudadNacimiento;
        int ciudadResidenciaIdx;
        int partidoIdx;
        bool esVice;
        int formulaIdx;
    };

    const CandidatoPresidencialDemo candidatosPresidenciales[] = {
        {"Sebastian", "Naranjo", 5001, 'M', "Casado", 10, 2, 1970, "Cali", 0, 0, false, 0},
        {"Marcela", "Quintero", 5002, 'F', "Casada", 3, 8, 1974, "Pereira", 1, 0, true, 0},
        {"Diego", "Calderon", 5003, 'M', "Soltero", 21, 9, 1968, "Santa Marta", 2, 1, false, 1},
        {"Isabela", "Suarez", 5004, 'F', "Union libre", 7, 12, 1976, "Barranquilla", 3, 1, true, 1}
    };
    const int totalCandidatosPresidenciales =
        static_cast<int>(sizeof(candidatosPresidenciales) / sizeof(candidatosPresidenciales[0]));

    const int totalFormulasDemo = 2;
    Candidato* presidentes[totalFormulasDemo];
    Candidato* vicepresidentes[totalFormulasDemo];
    for (int i = 0; i < totalFormulasDemo; ++i) {
        presidentes[i] = nullptr;
        vicepresidentes[i] = nullptr;
    }

    for (int i = 0; i < totalCandidatosPresidenciales; ++i) {
        const CandidatoPresidencialDemo& info = candidatosPresidenciales[i];
        Partido* partido = (info.partidoIdx >= 0 && info.partidoIdx < totalPartidos)
                               ? &partidos[info.partidoIdx]
                               : nullptr;
        Ciudad* residencia = ciudadesDemo[info.ciudadResidenciaIdx];

        NodoCandidato* nodo = new NodoCandidato();
        Fecha fecha(info.dia, info.mes, info.anio);
        Candidato* candidato = new Candidato(
            info.nombre,
            info.apellido,
            info.id,
            info.sexo,
            info.estadoCivil,
            fecha,
            info.ciudadNacimiento,
            residencia,
            partido,
            Candidato::Tipo::PRESIDENCIA,
            nodo,
            info.esVice
        );

        arbolCandidatos->insertar(candidato);
        if (info.formulaIdx >= 0 && info.formulaIdx < totalFormulasDemo) {
            if (info.esVice) {
                vicepresidentes[info.formulaIdx] = candidato;
            } else {
                presidentes[info.formulaIdx] = candidato;
            }
        }
    }

    for (int i = 0; i < totalFormulasDemo; ++i) {
        if (presidentes[i] && vicepresidentes[i]) {
            formulas[i].setPresidente(presidentes[i]);
            formulas[i].setVicepresidente(vicepresidentes[i]);
            formulas[i].setNumeroPartido(i);
            formulas[i].setPartido(presidentes[i]->getPartido());
            formulas[i].setVotos(0);

            Partido* partido = presidentes[i]->getPartido();
            if (partido) {
                partido->asignarFormulaPresidencial(&formulas[i]);
            }
        }
    }

    std::cout << "Datos de demostracion cargados correctamente.\n";
}
