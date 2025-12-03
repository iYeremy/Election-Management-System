#ifndef REGION_H
#define REGION_H

#include <string>

class Ciudad;

// region que enlaza sus ciudades mediante una lista simple
class Region {
private:
    std::string nombre;
    // censo acumulado de todas las ciudades asociadas
    int censoTotal;
    // cabecera de la lista simple de ciudades
    Ciudad* listaCiudades;
    // enlace a la siguiente region dentro de la multilista
    Region* sigRegion;

    // remueve una ciudad de la lista local
    void removerCiudad(Ciudad* ciudad);

public:
    Region();
    Region(const std::string& nombre, int censoTotal);

    // inserta una ciudad dentro de la region
    void agregarCiudad(Ciudad* ciudad);

    void setNombre(const std::string& nombre);
    void setCensoTotal(int censoTotal);
    void setListaCiudades(Ciudad* lista);
    void setSigRegion(Region* region);

    const std::string& getNombre() const;
    int getCensoTotal() const;
    Ciudad* getListaCiudades() const;
    Region* getSigRegion() const;
};

#endif
