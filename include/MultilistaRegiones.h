#ifndef MULTILISTAREGIONES_H
#define MULTILISTAREGIONES_H

#include <string>

class Region;
class Ciudad;

// multilista que enlaza regiones y permite ubicar ciudades dentro de cada una
class MultilistaRegiones {
private:
    // region inicial de la cadena
    Region* cabeza;

public:
    MultilistaRegiones();

    // inserta una region al inicio de la multilista
    void insertarRegion(Region* region);
    // navega hasta la region indicada y agrega la ciudad suministrada
    void insertarCiudadEnRegion(const std::string& nombreRegion, Ciudad* ciudad);
    // busca una ciudad recorriendo todas las regiones
    Ciudad* buscarCiudad(const std::string& nombre) const;
    Region* getCabeza() const;
};

#endif
