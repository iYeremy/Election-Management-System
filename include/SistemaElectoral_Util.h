#ifndef SISTEMA_ELECTORAL_UTIL_H
#define SISTEMA_ELECTORAL_UTIL_H

#include <string>
#include <vector>

class MultilistaRegiones;
class Region;
class Partido;
class Ciudad;
class Fecha;

namespace SistemaElectoralInterno {

Region* buscarRegionPorNombre(MultilistaRegiones* lista, const std::string& nombre);
bool hayRegionesCargadas(MultilistaRegiones* lista);
Partido* buscarPartidoPorNombre(Partido partidos[], const std::string& nombre);
bool hayCiudadesRegistradas(MultilistaRegiones* lista);
int calcularEdad(const Fecha& fecha);

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

bool leerRegistrosArchivo(const std::string& ruta, std::vector<RegistroPlanoCandidato>& registros);
bool escribirRegistrosArchivo(const std::string& ruta, const std::vector<RegistroPlanoCandidato>& registros);

}  // namespace SistemaElectoralInterno

#endif
