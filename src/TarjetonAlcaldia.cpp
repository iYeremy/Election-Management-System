#include "TarjetonAlcaldia.h"
#include <cstdlib>

TarjetonAlcaldia::TarjetonAlcaldia() {}

int TarjetonAlcaldia::totalOpciones() const {
    return 7; // 4 candidatos + blanco + nulo + abstencion
}

int TarjetonAlcaldia::votoAleatorio() const {
    return rand() % totalOpciones(); // 0..6
}
