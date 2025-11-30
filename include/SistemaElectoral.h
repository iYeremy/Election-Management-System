//SistemaElectoral.h

#ifndef SISTEMAELECTORAL_H
#define SISTEMAELECTORAL_H

class Region;

class SistemaElectoral {
private:
    Region* listaRegiones;

public:
    SistemaElectoral();

    void setListaRegiones(Region* regiones);
    Region* getListaRegiones() const;
};

#endif
