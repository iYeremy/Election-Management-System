#include "Region.h"

#include "Ciudad.h"

Region::Region() : nombre(), censoTotal(0), listaCiudades(nullptr), sigRegion(nullptr) {}

Region::Region(const std::string& nombre, int censoTotal)
    : nombre(nombre), censoTotal(censoTotal), listaCiudades(nullptr), sigRegion(nullptr) {}

void Region::agregarCiudad(Ciudad* ciudad) {
    if (!ciudad) {
        return;
    }
    
    //Limpieza 
    ciudad->setSigCiudad(nullptr); //elimina los enlaces previos
    ciudad->setRegion(this); //asegura que pertenece a esa region

    //insercion al inicio
    ciudad->setSigCiudad(listaCiudades);
    listaCiudades = ciudad;
}

void Region::setNombre(const std::string& nuevoNombre) {
    nombre = nuevoNombre;
}

void Region::setCensoTotal(int nuevoCenso) {
    censoTotal = nuevoCenso;
}

void Region::setListaCiudades(Ciudad* lista) {
    listaCiudades = lista;
}

void Region::setSigRegion(Region* regionSiguiente) {
    sigRegion = regionSiguiente;
}

const std::string& Region::getNombre() const {
    return nombre;
}

int Region::getCensoTotal() const {
    return censoTotal;
}

Ciudad* Region::getListaCiudades() const {
    return listaCiudades;
}

Region* Region::getSigRegion() const {
    return sigRegion;
}
