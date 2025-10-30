#include "estructuras/colapartidos.h"

// constructor del nodo
NodoCola::NodoCola(Partido* p) : partido(p), siguiente(nullptr) {}

// constructor de la cola
ColaPartidos::ColaPartidos() : frente(nullptr), final(nullptr) {}

// destructor
ColaPartidos::~ColaPartidos() {
    while (!vacia()) {
        desencolar();
    }
}

bool ColaPartidos::vacia() const {
    return frente == nullptr;
}

void ColaPartidos::encolar(Partido* p) {
    NodoCola* nuevo = new NodoCola(p);
    if (vacia()) {
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
}

Partido* ColaPartidos::desencolar() {
    if (vacia()) {
        throw runtime_error("No hay más partidos en la cola.");
    }

    NodoCola* temp = frente;
    Partido* p = temp->partido;
    frente = frente->siguiente;
    if (!frente) final = nullptr;

    delete temp; // solo borra el nodo, no el Partido
    return p;
}

vector<string> ColaPartidos::mostrarCola() const {
    vector<string> resumenes;
    if (vacia()) {
        resumenes.push_back("No hay partidos en la cola.");
        return resumenes;
    }

    NodoCola* aux = frente;
    while (aux) {
        resumenes.push_back(aux->partido->getResumen());
        aux = aux->siguiente;
    }
    return resumenes;
}

int ColaPartidos::tamano() const {
    int count = 0;
    NodoCola* aux = frente;
    while (aux) {
        count++;
        aux = aux->siguiente;
    }
    return count;
}
