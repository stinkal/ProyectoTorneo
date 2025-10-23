#ifndef PARTIDO_H
#define PARTIDO_H

#include <string>
#include "equipo.h"
using namespace std;

class Partido {
public:
    Partido(Equipo* local, Equipo* visitante);

    Equipo* getLocal() const;
    Equipo* getVisitante() const;
    int getGolesLocal() const;
    int getGolesVisitante() const;
    bool estaJugado() const;

    void registrarResultado(int golesLocal, int golesVisitante); // imprime resultados, e.g. "Saprissa (2) - Liga (1)"
    string getResumen() const;

private:
    Equipo* local;
    Equipo* visitante;
    int golesLocal;
    int golesVisitante;
    bool jugado;
};

#endif