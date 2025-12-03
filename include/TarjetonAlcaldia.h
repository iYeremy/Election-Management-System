#ifndef TARJETON_ALCALDIA_H
#define TARJETON_ALCALDIA_H

// genera indices aleatorios para simular un voto municipal
class TarjetonAlcaldia {
public:
    TarjetonAlcaldia();

    // Genera un voto aleatorio entre 0 y 6
    // 0..3 -> candidatos
    // 4    -> blanco
    // 5    -> nulo
    // 6    -> abstención
    int votoAleatorio() const;

    int totalOpciones() const; // retorna 7
};

#endif
