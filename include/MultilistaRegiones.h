#ifndef MULTILISTAREGIONES_H
#define MULTILISTAREGIONES_H

#include <string>

class Region;
class Ciudad;

class MultilistaRegiones {
private:
    Region* cabeza;

public:
    MultilistaRegiones();

    void insertarRegion(Region* region);
    void insertarCiudadEnRegion(const std::string& nombreRegion, Ciudad* ciudad);
    Ciudad* buscarCiudad(const std::string& nombre) const;
    Region* getCabeza() const;
};

#endif
