#ifndef AVLCIUDADES_H
#define AVLCIUDADES_H

#include <string>

class NodoCiudad;
class Ciudad;

// arbol AVL que organiza ciudades alfabeticamente por nombre
class AVLCiudades {
private:
    // nodo raiz del arbol
    NodoCiudad* raiz;

    // utilidades internas del AVL
    int obtenerAltura(NodoCiudad* nodo);
    int obtenerBalance(NodoCiudad* nodo);
    void actualizarAltura(NodoCiudad* nodo);

    NodoCiudad* rotarDerecha(NodoCiudad* nodo);
    NodoCiudad* rotarIzquierda(NodoCiudad* nodo);
    NodoCiudad* balancear(NodoCiudad* nodo);

public:
    AVLCiudades();
    ~AVLCiudades();

    // inserta una ciudad respetando el orden alfabetico
    void insertar(Ciudad* ciudad);
    // Busca una ciudad por nombre exacto
    Ciudad* buscar(const std::string& nombre);
    // imprime las ciudades en orden alfabetico
    void imprimir();
};

#endif
