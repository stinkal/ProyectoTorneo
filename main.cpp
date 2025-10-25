#include <iostream>
#include <vector>
#include "estructuras/arbol.h"
#include "modelos/equipo.h"

using namespace std;

//commit 2

int main() {
    cout << "=== PRUEBA DE TORNEO ===" << endl;

    // crear algunos equipos
    vector<Equipo*> equipos;
    equipos.push_back(new Equipo("Saprissa"));
    equipos.push_back(new Equipo("Alajuelense"));
    equipos.push_back(new Equipo("Heredia"));
    equipos.push_back(new Equipo("Cartago"));

    // crear arbol del torneo
    Arbol torneo;
    torneo.construir(equipos);

    cout << "\n=== LLAVES DEL TORNEO ===" << endl;
    torneo.mostrar(torneo.raiz);

    cout << "\n=== COMIENZA EL TORNEO ===" << endl;
    torneo.jugar(torneo.raiz);

    cout << "\n=== RESULTADOS ===" << endl;
    torneo.mostrar(torneo.raiz);

    Equipo* campeon = torneo.obtenerCampeon();
    if (campeon)
        cout << "\nCAMPEÓN: " << campeon->getNombre() << endl;
    else
        cout << "\nNo se pudo determinar un campeón." << endl;

    // limpieza de memoria
    for (Equipo* e : equipos) delete e;

    return 0;
}
