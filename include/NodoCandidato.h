#ifndef NODOCANDIDATO_H
#define NODOCANDIDATO_H

class Candidato;

class NodoCandidato {
private:
    Candidato* cand;
    NodoCandidato* sigCiudad;
    NodoCandidato* antCiudad;
    NodoCandidato* sigPartido;
    NodoCandidato* antPartido;

public:
    NodoCandidato();

    void setCandidato(Candidato* candidato);
    Candidato* getCandidato() const;

    void setSigCiudad(NodoCandidato* nodo);
    NodoCandidato* getSigCiudad() const;

    void setAntCiudad(NodoCandidato* nodo);
    NodoCandidato* getAntCiudad() const;

    void setSigPartido(NodoCandidato* nodo);
    NodoCandidato* getSigPartido() const;

    void setAntPartido(NodoCandidato* nodo);
    NodoCandidato* getAntPartido() const;
};

#endif
