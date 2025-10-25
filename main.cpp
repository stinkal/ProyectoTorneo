#include <iostream>
#include <vector>
#include "estructuras/arbol.h"
#include "modelos/equipo.h"

using namespace std;

int main() {
    cout << "=== PRUEBA DE TORNEO ===" << endl;

    // Crear algunos equipos
    vector<Equipo*> equipos;
    equipos.push_back(new Equipo("Saprissa"));
    equipos.push_back(new Equipo("Alajuelense"));
    equipos.push_back(new Equipo("Heredia"));
    equipos.push_back(new Equipo("Cartago"));

    // Crear árbol del torneo
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

    // Limpieza de memoria
    for (Equipo* e : equipos) delete e;

    return 0;
}
