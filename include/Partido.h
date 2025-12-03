#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>

class NodoCandidato;
class Duo;
class Candidato;

// representa un partido con sus candidatos municipales y (presidente + vicepresidente)
class Partido {
private:
    // nombre oficial del partido
    std::string nombre;
    // representante legal
    std::string representante;
    // cabecera de la lista doble de candidatos a alcaldia
    NodoCandidato* listaCandidatosAlcaldia;
    // (presidente + vicepresidente) asociado en el arreglo de Duos
    Duo* formulaPresidencial;
    // Extrae un nodo de la lista local
    void removerCandidatoDeLista(NodoCandidato* nodo);

public:
    Partido();
    Partido(const std::string& nombre, const std::string& representante);

    // inserta un candidato municipal en la lista del partido
    void agregarCandidatoAlcaldia(Candidato* candidato);
    // enlaza el partido con su formula presidencial
    void asignarFormulaPresidencial(Duo* duo);

    void setNombre(const std::string& nombre);
    void setRepresentante(const std::string& representante);
    void setListaCandidatosAlcaldia(NodoCandidato* lista);
    void setFormulaPresidencial(Duo* duo);

    const std::string& getNombre() const;
    const std::string& getRepresentante() const;
    NodoCandidato* getListaCandidatosAlcaldia() const;
    Duo* getFormulaPresidencial() const;
};

#endif
