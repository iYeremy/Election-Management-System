#ifndef MULTILISTA_H
#define MULTILISTA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct datos {
    int pos;
    datos *sigNom;
    datos *sigCar;
    datos *sigActiv;
    datos *sigEdad;
};

struct cabeza {
    string caracteristica;
    datos *cab;
};

struct estudiante { //fue de la actividad
    string nombre;
    string carrera;
    string actividad;
    int edad;
    bool activo;
};

class Multilista {
private:
    cabeza cabeceras[4];
    vector<estudiante> registros;
    string archivo = "estudiantes.txt";

    datos* crearNodo(int pos);

    // Inserciones internas
    void insertarNombre(datos* nuevo);
    void insertarCarrera(datos* nuevo);
    void insertarActividad(datos* nuevo);
    void insertarPorEdad(datos* nuevo);

    // Borrado interno
    void eliminarDeLista(datos*& cab, int pos, string tipo);

public:
    Multilista();
    ~Multilista();

    void cargarArchivo();
    void guardarArchivo();
    bool vacia();

    void insertar(const estudiante& r);
    void eliminar(string nombre);
    void buscar(string nombre);

    // Listados
    void imprimirPorNombre();
    void imprimirPorCarrera(string carrera);
    void imprimirPorActividad(string actividad);
    void imprimirPorEdad();
};

#endif
