#ifndef REGION_H
#define REGION_H

#include <string>

class Ciudad;

class Region {
private:
    std::string nombre;
    int censoTotal;
    Ciudad* listaCiudades;
    Region* sigRegion;

public:
    Region();
    Region(const std::string& nombre, int censoTotal);

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
