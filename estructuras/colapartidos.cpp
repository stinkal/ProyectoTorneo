#include "estructuras/colapartidos.h"

// constructor del nodo
NodoCola::NodoCola(Partido* p) : partido(p), siguiente(nullptr) {}

// constructor de la cola
ColaPartidos::ColaPartidos() : frente(nullptr), final(nullptr) {}

// destructor para liberar los nodos
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

void ColaPartidos::mostrarCola() const {
    if (vacia()) {
        cout << "No hay partidos en la cola." << endl;
        return;
    }

    NodoCola* aux = frente;
    cout << "\n=== CALENDARIO DE PARTIDOS ===" << endl;
    while (aux) {
        cout << aux->partido->getResumen() << endl;
        aux = aux->siguiente;
    }
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
