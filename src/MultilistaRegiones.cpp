#include "MultilistaRegiones.h"

#include "Ciudad.h"
#include "Region.h"

MultilistaRegiones::MultilistaRegiones() : cabeza(nullptr) {}

void MultilistaRegiones::insertarRegion(Region* region) {
    if (!region) {
        return;
    }

    region->setSigRegion(cabeza);
    cabeza = region;
}

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
