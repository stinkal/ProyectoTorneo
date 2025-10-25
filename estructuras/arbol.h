#ifndef ARBOL_H
#define ARBOL_H

#include "../modelos/partido.h"
#include "../modelos/equipo.h"
#include <vector>

//commit 2

struct NodoArbol {
    Partido* partido;
    NodoArbol* izquierda;
    NodoArbol* derecha;

    NodoArbol(Partido* p = nullptr) : partido(p), izquierda(nullptr), derecha(nullptr) {}
};

class Arbol {
public:
    NodoArbol* raiz;

    Arbol();
    NodoArbol* construir(const std::vector<Equipo*>& equipos);
    void jugar(NodoArbol* nodo);
    Equipo* obtenerCampeon() const;
    void mostrar(NodoArbol* nodo, int nivel = 0) const;

private:
    static NodoArbol* construirRec(const std::vector<Equipo*>& equipos, int inicio, int fin);
    Equipo* determinarGanador(Partido* partido) const;
};

#endif
