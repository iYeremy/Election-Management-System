#include "Region.h"

#include "Ciudad.h"

Region::Region() : nombre(), censoTotal(0), listaCiudades(nullptr), sigRegion(nullptr) {}

Region::Region(const std::string& nombre, int censoTotal)
    : nombre(nombre), censoTotal(censoTotal), listaCiudades(nullptr), sigRegion(nullptr) {}

// inserta una ciudad al frente de la lista y actualiza el censo
void Region::agregarCiudad(Ciudad* ciudad) {
    if (!ciudad) {
        return;
    }

    if (ciudad->getRegion() == this) {
        return;
    }

    if (Region* anterior = ciudad->getRegion()) {
        anterior->removerCiudad(ciudad);
    }

    ciudad->setRegion(this);
    ciudad->setSigCiudad(listaCiudades);
    listaCiudades = ciudad;
    censoTotal += ciudad->getCenso();
}

// quita una ciudad de la lista local preservando enlaces
void Region::removerCiudad(Ciudad* ciudad) {
    if (!ciudad || ciudad->getRegion() != this) {
        return;
    }

    Ciudad* actual = listaCiudades;
    Ciudad* anterior = nullptr;

    while (actual && actual != ciudad) {
        anterior = actual;
        actual = actual->getSigCiudad();
    }

    if (!actual) {
        return;
    }

    if (anterior) {
        anterior->setSigCiudad(actual->getSigCiudad());
    } else {
        listaCiudades = actual->getSigCiudad();
    }

    ciudad->setSigCiudad(nullptr);
    ciudad->setRegion(nullptr);
    censoTotal -= ciudad->getCenso();
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
