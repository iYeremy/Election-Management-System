#ifndef NODOCANDIDATOID_H
#define NODOCANDIDATOID_H

class Candidato;

class NodoCandidatoID {
private:
    Candidato* candidato;
    NodoCandidatoID* izq;
    NodoCandidatoID* der;
    int altura;

public:
    NodoCandidatoID();

    void setCandidato(Candidato* candidato);
    Candidato* getCandidato() const;

    void setIzq(NodoCandidatoID* nodo);
    NodoCandidatoID* getIzq() const;

    void setDer(NodoCandidatoID* nodo);
    NodoCandidatoID* getDer() const;

    void setAltura(int nuevaAltura);
    int getAltura() const;
};

#endif
