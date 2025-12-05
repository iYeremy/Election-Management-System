#ifndef CIUDAD_HELPER_H
#define CIUDAD_HELPER_H

class Ciudad;
class NodoCandidato;
class Candidato;
class Partido;

// Mapea hasta cuatro candidatos municipales para acceso rapido.
void mapearNodosCiudad(Ciudad* ciudad, NodoCandidato* nodos[4]);

// Devuelve el candidato segun el indice en la lista municipal.
Candidato* candidatoMunicipalPorIndice(Ciudad* ciudad, int idx);
int indicePartido(const Partido partidos[], const Partido* partido);

#endif
