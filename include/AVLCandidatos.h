#ifndef AVLCANDIDATOS_H
#define AVLCANDIDATOS_H

class NodoCandidatoID;
class Candidato;

// arbol AVL que organiza candidatos por su numero de identificacion
class AVLCandidatos {
private:
    // nodo raiz que sostiene la estructura completa
    NodoCandidatoID* raiz;

    // herramientas internas para mantener el equilibrio del AVL
    int obtenerAltura(NodoCandidatoID* nodo);
    int obtenerBalance(NodoCandidatoID* nodo);
    void actualizarAltura(NodoCandidatoID* nodo);

    NodoCandidatoID* rotarDerecha(NodoCandidatoID* nodo);
    NodoCandidatoID* rotarIzquierda(NodoCandidatoID* nodo);
    NodoCandidatoID* balancear(NodoCandidatoID* nodo);

public:
    AVLCandidatos();
    ~AVLCandidatos();

    // inserta un candidato usando el ID como clave
    void insertar(Candidato* candidato);
    // busca un candidato segun su ID
    Candidato* buscar(long id);
    // recorre en orden para imprimir los candidatos
    void imprimir();
};

#endif
