#ifndef DUO_H
#define DUO_H

class Candidato;
class Partido;

// representa (presidente + vicepresidente)
class Duo {
private:
    // candidato que aspira a presidente
    Candidato* presidente;
    // candidato que aspira a vicepresidente
    Candidato* vicepresidente;

    Partido* partido;
    // conteo de votos obtenido durante la simulacion
    int votos;
    // numero que identifica (presidente + vicepresidente) en el tarjeton
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
