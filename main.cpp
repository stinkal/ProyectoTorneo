#include <iostream>
#include "modelos/equipo.h"

int main() {
    Equipo e1("Dragones");
    Equipo e2("Tiburones");

    e1.sumarPuntos(3);
    e2.sumarPuntos(1);

    cout << e1.getNombre() << " tiene " << e1.getPuntos() << " puntos.\n";
    cout << e2.getNombre() << " tiene " << e2.getPuntos() << " puntos.\n";

    return 0;
}
