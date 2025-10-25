#ifndef COLAPARTIDOS_H
#define COLAPARTIDOS_H

#include "../modelos/partido.h"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

class NodoCola {
public:
    Partido* partido;
    NodoCola* siguiente;

    NodoCola(Partido* p);
};

class ColaPartidos {   //partidos como lista enlazada
private:
    NodoCola* frente;
    NodoCola* final;

public:
    ColaPartidos();
    ~ColaPartidos();

    bool vacia() const;
    void encolar(Partido* p);
    Partido* desencolar();
    vector<string> mostrarCola() const;
    int tamano() const;
};

#endif
