#include "SistemaElectoral.h"

SistemaElectoral::SistemaElectoral() : listaRegiones(nullptr) {}

void SistemaElectoral::setListaRegiones(Region* regiones) {
    listaRegiones = regiones;
}

Region* SistemaElectoral::getListaRegiones() const {
    return listaRegiones;
}
