#include "MultilistaRegiones.h"

#include "Ciudad.h"
#include "Region.h"

// constructor: inicia la multilista vacia
MultilistaRegiones::MultilistaRegiones() : cabeza(nullptr) {}

// inserta una region al inicio de la lista
void MultilistaRegiones::insertarRegion(Region* region) {
    if (!region) {
        return;
    }

    region->setSigRegion(cabeza);
    cabeza = region;
}

// busca la region solicitada y delega la insercion de la ciudad
void MultilistaRegiones::insertarCiudadEnRegion(const std::string& nombreRegion, Ciudad* ciudad) {
    if (!ciudad) {
        return;
    }

    Region* actual = cabeza;
    while (actual) {
        if (actual->getNombre() == nombreRegion) {
            actual->agregarCiudad(ciudad);
            return;
        }
        actual = actual->getSigRegion();
    }
}

// recorre primero regiones y luego ciudades hasta hallar el nombre
Ciudad* MultilistaRegiones::buscarCiudad(const std::string& nombre) const {
    Region* actualRegion = cabeza;
    while (actualRegion) {
        Ciudad* actualCiudad = actualRegion->getListaCiudades();
        while (actualCiudad) {
            if (actualCiudad->getNombre() == nombre) {
                return actualCiudad;
            }
            actualCiudad = actualCiudad->getSigCiudad();
        }
        actualRegion = actualRegion->getSigRegion();
    }
    return nullptr;
}

Region* MultilistaRegiones::getCabeza() const {
    return cabeza;
}
