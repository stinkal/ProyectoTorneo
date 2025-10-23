#include "partido.h"
#include <sstream>  //stringstream

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

    // actualiza puntos para grupos
    if (gLocal > gVisitante) {
        local->sumarPuntos(3);   // gana local
    } else if (gLocal < gVisitante) {
        visitante->sumarPuntos(3); // gana visitante
    } else {
        local->sumarPuntos(1);     // empate
        visitante->sumarPuntos(1);
    }
}

string Partido::getResumen() const {
    stringstream ss;
    ss << local->getNombre() << " (" << golesLocal << ") - "
       << visitante->getNombre() << " (" << golesVisitante << ")";
    return ss.str();
}
