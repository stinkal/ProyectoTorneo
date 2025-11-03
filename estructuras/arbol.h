#ifndef ARBOL_H
#define ARBOL_H

#include "../modelos/partido.h"
#include "../modelos/equipo.h"
#include <vector>
#include <string>

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
    ~Arbol();
    NodoArbol* construir(const std::vector<Equipo*>& equipos);
    NodoArbol* buscarPrimerPartidoNoJugado() const;
    void jugar(NodoArbol* nodo);
    Equipo* obtenerCampeon() const;
    void mostrar(std::ostream& out, NodoArbol* nodo, int nivel = 0) const;
    std::vector<Partido*> obtenerPartidosPendientes() const;
    NodoArbol* getRaiz() const;
    bool contienePartido(Partido* p) const;
    Equipo* obtenerPerdedor(Partido* p) const;
    std::string getTextoEstructura() const;
    Equipo* obtenerSubcampeon() const;


private:
    static NodoArbol* construirRec(const std::vector<Equipo*>& equipos, int inicio, int fin);
    Equipo* determinarGanador(Partido* partido) const;
    void destruirRec(NodoArbol* nodo);
};

#endif
