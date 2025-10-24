#include "partido.h"
#include <sstream>

Partido::Partido(Equipo* local, Equipo* visitante)
    : local(local), visitante(visitante), golesLocal(0), golesVisitante(0), jugado(false) {}

Equipo* Partido::getLocal() const {return local;}
Equipo* Partido::getVisitante() const {return visitante;}
int Partido::getGolesLocal() const {return golesLocal;}
int Partido::getGolesVisitante() const {return golesVisitante;}
bool Partido::estaJugado() const {return jugado;}

void Partido::registrarResultado(int gLocal, int gVisitante) {
    golesLocal = gLocal;
    golesVisitante = gVisitante;
    jugado = true;
}

string Partido::getResumen() const {
    std::stringstream ss;
    ss << local->getNombre() << " (" << golesLocal << ") - "
       << visitante->getNombre() << " (" << golesVisitante << ")";
    return ss.str();
}
