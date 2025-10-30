#include "arbol.h"
#include <iostream>
using namespace std;

Arbol::Arbol() : raiz(nullptr) {}
Arbol::~Arbol() {
    destruirRec(raiz);
    raiz = nullptr;
}

void Arbol::destruirRec(NodoArbol* nodo) {
    if (!nodo) return;
    if (nodo->izquierda) destruirRec(nodo->izquierda);
    if (nodo->derecha) destruirRec(nodo->derecha);
    if (nodo->partido) {
        delete nodo->partido;
        nodo->partido = nullptr;
    }
    delete nodo;
}

NodoArbol* Arbol::construirRec(const vector<Equipo*>& equipos, int inicio, int fin) {
    if (inicio > fin) return nullptr;

    if (inicio + 1 == fin) {
        return new NodoArbol(new Partido(equipos[inicio], equipos[fin]));  //   caso base
    }

    int mid = (inicio + fin) / 2;
    NodoArbol* izquierda = construirRec(equipos, inicio, mid);
    NodoArbol* derecha = construirRec(equipos, mid + 1, fin);

    NodoArbol* nodo = new NodoArbol(nullptr);  //   creando el siguiente nodo para llenar luego
    nodo->izquierda = izquierda;
    nodo->derecha = derecha;

    return nodo;
}

NodoArbol* Arbol::construir(const vector<Equipo*>& equipos) {
    raiz = construirRec(equipos, 0, equipos.size() - 1);
    return raiz;
}

Equipo* Arbol::determinarGanador(Partido* partido) const {
    if (!partido->estaJugado()) return nullptr;
    if (partido->getGolesLocal() > partido->getGolesVisitante())
        return partido->getLocal();
    else if (partido->getGolesVisitante() > partido->getGolesLocal())
        return partido->getVisitante();
    else
        return nullptr; // empate
}

void Arbol::jugar(NodoArbol* nodo) {
    if (!nodo) return;

    if (nodo->izquierda) jugar(nodo->izquierda);   //recursion
    if (nodo->derecha) jugar(nodo->derecha);

    if (!nodo->partido && nodo->izquierda && nodo->derecha) {
        Equipo* ganadorIzq = determinarGanador(nodo->izquierda->partido);
        Equipo* ganadorDer = determinarGanador(nodo->derecha->partido);
        if (ganadorIzq && ganadorDer)
            nodo->partido = new Partido(ganadorIzq, ganadorDer);
        else
            return; //  si hacen falta resultados
    }

    if (nodo->partido && !nodo->partido->estaJugado()) {
        cout << "Ingrese resultado de "
             << nodo->partido->getLocal()->getNombre() << " vs "
             << nodo->partido->getVisitante()->getNombre() << " (ejemplo: 2 1): ";
        int gL, gV;
        cin >> gL >> gV;
        nodo->partido->registrarResultado(gL, gV);
    }
}

Equipo* Arbol::obtenerCampeon() const {
    if (!raiz || !raiz->partido) return nullptr;
    return determinarGanador(raiz->partido);
}

void Arbol::mostrar(NodoArbol* nodo, int nivel) const {
    if (!nodo) return;

    mostrar(nodo->derecha, nivel + 1);

    cout << string(nivel * 6, ' ');
    if (nodo->partido) {
        cout << nodo->partido->getResumen();
    } else {
        cout << "(pendiente)";
    }
    cout << endl;

    mostrar(nodo->izquierda, nivel + 1);
}
