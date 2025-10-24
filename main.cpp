#include <iostream>
#include "modelos/equipo.h"
#include "modelos/partido.h"

int main() {
    Equipo e1("Dragones");   // placeholder
    Equipo e2("Tiburones");   //placeholder

    Partido p1(&e1, &e2);
    p1.registrarResultado(2, 1);

    cout << p1.getResumen() << endl;
    cout << e1.getNombre() << " tiene " << e1.getPuntos() << " puntos.\n";
    cout << e2.getNombre() << " tiene " << e2.getPuntos() << " puntos.\n";

    return 0;
}
