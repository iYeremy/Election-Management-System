#ifndef TARJETON_PRESIDENCIAL_H
#define TARJETON_PRESIDENCIAL_H

// provee indices aleatorios para formulas presidenciales y opciones especiales
class TarjetonPresidencial {
public:
    TarjetonPresidencial();

    // Genera un voto aleatorio entre 0 y 7
    // 0..4 -> Formulas presidenciales (5 partidos)
    // 5    -> blanco
    // 6    -> nulo
    // 7    -> abstencion
    int votoAleatorio() const;

    int totalOpciones() const; // retorna 8
};

#endif
