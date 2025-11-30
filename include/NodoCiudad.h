#ifndef NODOCIUDAD_H
#define NODOCIUDAD_H

class Ciudad;

class NodoCiudad {
private:
    Ciudad* ciudad;
    NodoCiudad* izq;
    NodoCiudad* der;
    int altura;

public:
    NodoCiudad();

    void setCiudad(Ciudad* ciudad);
    Ciudad* getCiudad() const;

    void setIzq(NodoCiudad* nodo);
    NodoCiudad* getIzq() const;

    void setDer(NodoCiudad* nodo);
    NodoCiudad* getDer() const;

    void setAltura(int nuevaAltura);
    int getAltura() const;
};

#endif
