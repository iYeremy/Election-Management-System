#include "TarjetonPresidencial.h"
#include <cstdlib>

TarjetonPresidencial::TarjetonPresidencial() {}

int TarjetonPresidencial::totalOpciones() const {
    return 8; // 5 formulas + blanco + nulo + abstencion
}

int TarjetonPresidencial::votoAleatorio() const {
    return rand() % totalOpciones(); // 0..7
}
