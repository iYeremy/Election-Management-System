#include "SistemaElectoral.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdio>

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
#include "CiudadHelper.h"
#include "Reportes.h"

namespace {
 

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

int indicePartido(const Partido partidos[], const Partido* partido) {
    if (!partido) {
        return -1;
    }
    for (int i = 0; i < 5; ++i) {
        if (&partidos[i] == partido) {
            return i;
        }
    }
    return -1;
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

struct RegistroPlanoCandidato {
    std::string nombre;
    std::string apellido;
    long id = 0;
    char sexo = ' ';
    std::string estadoCivil;
    int dia = 1;
    int mes = 1;
    int anio = 1970;
    std::string ciudadNacimiento;
    std::string ciudadResidencia;
    std::string partido;
    char tipo = 'A';
    bool esVice = false;
};

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

}  // namespace

// constructor: prepara las estructuras base y arreglos fijos
SistemaElectoral::SistemaElectoral()
    : regiones(new MultilistaRegiones()),
      arbolCiudades(new AVLCiudades()),
      arbolCandidatos(new AVLCandidatos()),
      segundaVueltaPendiente(false),
      segundaVueltaRealizada(false),
      blancosSegundaVuelta(0),
      nulosSegundaVuelta(0),
      abstencionSegundaVuelta(0),
      totalVotosPresidenciales(0),
      blancosPresidenciales(0),
      nulosPresidenciales(0),
      abstencionPresidencial(0),
      rutaPartidos(),
      rutaRegiones(),
      rutaCiudades(),
      rutaCandidatos(),
      rutaFormulas()
{
    // inicializar los arreglos fijos
    for (int i = 0; i < 5; i++) {
        partidos[i] = Partido();
        formulas[i] = Duo();
    }
    indicesSegundaVuelta[0] = indicesSegundaVuelta[1] = -1;
    votosSegundaVuelta[0] = votosSegundaVuelta[1] = 0;
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
        std::string idPresStr, idViceStr, numPartidoStr;

        std::getline(ss, idPresStr, '|');
        std::getline(ss, idViceStr, '|');
        std::getline(ss, numPartidoStr, '|');

        long idPres = std::atol(idPresStr.c_str());
        long idVice = std::atol(idViceStr.c_str());
        int numPartido = std::atoi(numPartidoStr.c_str());

        Candidato* presidente = arbolCandidatos->buscar(idPres);
        Candidato* vicepresidente = arbolCandidatos->buscar(idVice);

        if (!presidente || !vicepresidente) {
            std::cout << "Advertencia: Candidatos no encontrados para formula\n";
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
        formulas[contador].setNumeroPartido(numPartido);
        formulas[contador].setPartido(presidente->getPartido());
        formulas[contador].setVotos(0);

        if (numPartido >= 0 && numPartido < 5) {
            partidos[numPartido].asignarFormulaPresidencial(&formulas[contador]);
        }

        contador++;
    }

    archivo.close();
    std::cout << "Formulas presidenciales cargadas: " << contador << std::endl;
}

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
// REPORTES
// -----------------------------
void SistemaElectoral::generarReporteCiudades(const std::string& rutaSalida) {
    Reportes::generarReporteCiudades(regiones, rutaSalida);
}

void SistemaElectoral::generarReporteRegiones(const std::string& rutaSalida) {
    Reportes::generarReporteRegiones(regiones, rutaSalida);
}

void SistemaElectoral::generarReporteNacional(const std::string& rutaSalida) {
    Reportes::generarReporteNacional(regiones, formulas, rutaSalida);
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

    segundaVueltaPendiente = false;
    segundaVueltaRealizada = false;
    indicesSegundaVuelta[0] = indicesSegundaVuelta[1] = -1;
    votosSegundaVuelta[0] = votosSegundaVuelta[1] = 0;
    blancosSegundaVuelta = nulosSegundaVuelta = abstencionSegundaVuelta = 0;
    totalVotosPresidenciales = 0;
    blancosPresidenciales = 0;
    nulosPresidenciales = 0;
    abstencionPresidencial = 0;
}

// -----------------------------
// CONSULTAS COMPLEMENTARIAS
// -----------------------------
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

    if (!hayFormulas) {
        std::cout << "No hay formulas presidenciales registradas.\n";
    }
    std::cout << posicion << ". Voto nulo\n";
    std::cout << (posicion + 1) << ". Abstencion\n";
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
        if (!formula.getPresidente() || !formula.getVicepresidente()) {
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
        std::cout << "No hay formulas completas registradas.\n";
    }
}

void SistemaElectoral::mostrarCandidatosCiudadPorPartido(const std::string& nombreCiudad) {
    Ciudad* ciudad = buscarCiudad(nombreCiudad);
    if (!ciudad) {
        std::cout << "Ciudad " << nombreCiudad << " no encontrada.\n";
        return;
    }

    std::cout << "\nCandidatos por partido en " << ciudad->getNombre() << ":\n";
    for (int i = 0; i < 5; ++i) {
        if (partidos[i].getNombre().empty()) {
            continue;
        }
        Candidato* candidatoEncontrado = nullptr;
        NodoCandidato* nodo = ciudad->getCandidatosAlcaldia();
        while (nodo) {
            Candidato* candidato = nodo->getCandidato();
            if (candidato && candidato->getPartido() == &partidos[i]) {
                candidatoEncontrado = candidato;
                break;
            }
            nodo = nodo->getSigCiudad();
        }
        std::cout << " - " << partidos[i].getNombre() << ": ";
        if (candidatoEncontrado) {
            std::cout << candidatoEncontrado->getNombre() << " " << candidatoEncontrado->getApellido();
        } else {
            std::cout << "Sin candidato registrado";
        }
        std::cout << "\n";
    }

    bool hayIndependientes = false;
    NodoCandidato* nodo = ciudad->getCandidatosAlcaldia();
    while (nodo) {
        Candidato* candidato = nodo->getCandidato();
        if (candidato && !candidato->getPartido()) {
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

bool SistemaElectoral::puedeEditar() const {
    return !rutaPartidos.empty() && !rutaRegiones.empty() && !rutaCiudades.empty() &&
           !rutaCandidatos.empty() && !rutaFormulas.empty();
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

void SistemaElectoral::agregarCandidatoAlcaldia() {
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

void SistemaElectoral::reporteGeneroPresidencialPorPartido() const {
    Reportes::reporteGeneroPresidencialPorPartido(partidos, formulas);
}

void SistemaElectoral::mostrarEstructuraDatos() const {
    Reportes::mostrarEstructuraDatos(regiones, arbolCiudades, arbolCandidatos);
}
