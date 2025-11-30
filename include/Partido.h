#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>

class NodoCandidato;
class Duo;
class Candidato;

class Partido {
private:
    std::string nombre;
    std::string representante;
    NodoCandidato* listaCandidatosAlcaldia;
    Duo* formulaPresidencial;

public:
    Partido();
    Partido(const std::string& nombre, const std::string& representante);

    void agregarCandidatoAlcaldia(Candidato* candidato);
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
