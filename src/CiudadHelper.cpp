#include "CiudadHelper.h"

#include "Ciudad.h"
#include "NodoCandidato.h"
#include "Candidato.h"

void mapearNodosCiudad(Ciudad* ciudad, NodoCandidato* nodos[4]) {
    for (int i = 0; i < 4; ++i) {
        nodos[i] = nullptr;
    }
    if (!ciudad) {
        return;
    }

    NodoCandidato* actual = ciudad->getCandidatosAlcaldia();
    int idx = 0;
    while (actual && idx < 4) {
        nodos[idx++] = actual;
        actual = actual->getSigCiudad();
    }
}

Candidato* candidatoMunicipalPorIndice(Ciudad* ciudad, int idx) {
    if (!ciudad || idx < 0 || idx >= 4) {
        return nullptr;
    }
    NodoCandidato* actual = ciudad->getCandidatosAlcaldia();
    int pos = 0;
    while (actual && pos < 4) {
        if (pos == idx) {
            return actual->getCandidato();
        }
        actual = actual->getSigCiudad();
        ++pos;
    }
    return nullptr;
}
