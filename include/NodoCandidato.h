#ifndef NODOCANDIDATO_H
#define NODOCANDIDATO_H

class Candidato;
class Ciudad;
class Partido;

// nodo que conecta un candidato con listas por ciudad y por partido
class NodoCandidato {
private:
    // referencia al candidato dueño del nodo
    Candidato* cand;
    // enlaces para la lista doble por ciudad
    NodoCandidato* sigCiudad;
    NodoCandidato* antCiudad;
    // enlaces para la lista doble por partido
    NodoCandidato* sigPartido;
    NodoCandidato* antPartido;
    // ciudad en la cual esta enlazado actualmente
    Ciudad* ciudadLista;
    // partido con el que esta enlazado actualmente
    Partido* partidoLista;

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

    void setCiudadLista(Ciudad* ciudad);
    Ciudad* getCiudadLista() const;

    void setPartidoLista(Partido* partido);
    Partido* getPartidoLista() const;
};

#endif
