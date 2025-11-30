#ifndef DUO_H
#define DUO_H

class Candidato;
class Partido;

class Duo {
private:
    Candidato* presidente;
    Candidato* vicepresidente;
    Partido* partido;
    int votos;
    int numeroPartido;

public:
    Duo();
    Duo(Candidato* presidente, Candidato* vicepresidente, int numeroPartido);

    void setPresidente(Candidato* presidente);
    void setVicepresidente(Candidato* vicepresidente);
    void setPartido(Partido* partido);
    void setVotos(int votos);
    void setNumeroPartido(int numeroPartido);

    Candidato* getPresidente() const;
    Candidato* getVicepresidente() const;
    Partido* getPartido() const;
    int getVotos() const;
    int getNumeroPartido() const;
};

#endif
