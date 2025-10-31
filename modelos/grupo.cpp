#include "grupo.h"
#include <iomanip>
#include <algorithm>
using namespace std;

Grupo::Grupo(const string& nombre)
    : nombre(nombre), partidosGenerados(false) {}

bool Grupo::agregarEquipo(Equipo* equipo) {
    if (equipos.size() >= 4) {
        cout << "El grupo ya tiene 4 equipos.\n";
        return false;
    }

    for (auto e : equipos) {   // validacion de duplicados
        if (e->getNombre() == equipo->getNombre()) {
            cout << "El equipo " << equipo->getNombre() << " ya está en el grupo.\n";
            return false;
        }
    }

    equipos.push_back(equipo);   //append al final
    return true;
}

void Grupo::listarEquipos() const {
    cout << "Equipos en el grupo " << nombre << ":\n";   //acordad nombrar los grupos solo 'A', 'B', etc. no 'Grupo A'
    for (auto e : equipos) {
        cout << "- " << e->getNombre() << endl;
    }
}

void Grupo::generarPartidos() {
    if (equipos.size() < 2) {
        cout << "Se necesitan al menos 2 equipos para generar partidos.\n";
        return;
    }

    partidos.clear();
    for (size_t i = 0; i < equipos.size(); ++i) {
        for (size_t j = i + 1; j < equipos.size(); ++j) {
            partidos.push_back(Partido(equipos[i], equipos[j]));   //CAMBIE ESTO SI TIENE QUE SER DOBLE ROUND-ROBIN
        }
    }
    partidosGenerados = true;
    cout << "Partidos generados exitosamente para el grupo " << nombre << ".\n";
}

void Grupo::listarPartidos() const {
    if (!partidosGenerados) {
        cout << "No hay partidos generados todavía.\n";
        return;
    }

    cout << "Partidos del grupo " << nombre << ":\n";
    for (size_t i = 0; i < partidos.size(); ++i) {
        cout << i + 1 << ") " << partidos[i].getResumen() << endl;
    }
}

void Grupo::registrarResultado(int indicePartido, int golesLocal, int golesVisitante) {
    if (indicePartido < 1 || indicePartido > (int)partidos.size()) {
        cout << "Índice de partido inválido.\n";
        return;
    }

    Partido& p = partidos[indicePartido - 1];
    p.registrarResultado(golesLocal, golesVisitante);
    cout << "Resultado registrado: " << p.getResumen() << endl;
}

void Grupo::calcularPuntos() {
    //reinicia los puntos
    for (auto e : equipos) {
        e->setPuntos(0);
    }

    //calcula puntos por cada partido jugado
    for (auto& p : partidos) {
        if (!p.estaJugado()) continue;

        int gL = p.getGolesLocal();
        int gV = p.getGolesVisitante();
        Equipo* local = p.getLocal();
        Equipo* visitante = p.getVisitante();

        if (gL > gV) {
            local->sumarPuntos(3);
        } else if (gL < gV) {
            visitante->sumarPuntos(3);
        } else {
            local->sumarPuntos(1);
            visitante->sumarPuntos(1);
        }
    }
}

void Grupo::mostrarTablaPuntos() const {
    cout << "\nTabla de puntos del grupo " << nombre << ":\n";
    cout << left << setw(20) << "Equipo" << "Puntos\n";
    cout << "-------------------------\n";

    //copia para ordenar sin modificar los originales
    vector<Equipo*> ordenados = equipos;
    sort(ordenados.begin(), ordenados.end(),
         [](Equipo* a, Equipo* b) { return a->getPuntos() > b->getPuntos(); });

    for (auto e : ordenados) {
        cout << left << setw(20) << e->getNombre() << e->getPuntos() << endl;
    }
}

vector<Equipo*> Grupo::getEquipos() const{
    return equipos;
}
vector<Partido*> Grupo::getPartidosPtr() {
    vector<Partido*> ptrs;
    ptrs.reserve(partidos.size());   //  manejo de memoria
    for (auto &p : partidos) ptrs.push_back(&p);
    return ptrs;
}