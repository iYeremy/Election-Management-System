#ifndef AVLCIUDADES_H
#define AVLCIUDADES_H

#include <string>

class NodoCiudad;
class Ciudad;

class AVLCiudades {
private:
    NodoCiudad* raiz;

    int obtenerAltura(NodoCiudad* nodo);
    int obtenerBalance(NodoCiudad* nodo);
    void actualizarAltura(NodoCiudad* nodo);

    NodoCiudad* rotarDerecha(NodoCiudad* nodo);
    NodoCiudad* rotarIzquierda(NodoCiudad* nodo);
    NodoCiudad* balancear(NodoCiudad* nodo);

public:
    AVLCiudades();
    ~AVLCiudades();

    void insertar(Ciudad* ciudad);
    Ciudad* buscar(const std::string& nombre);
    void imprimir();
};

#endif