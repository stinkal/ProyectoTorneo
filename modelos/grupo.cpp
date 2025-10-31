#include "grupo.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

Grupo::Grupo(const std::string& nombre)
    : nombre(nombre), partidosGenerados(false) {}

bool Grupo::agregarEquipo(Equipo* equipo) {
    if (!equipo) return false;
    if (equipos.size() >= 4) {
        return false;
    }

    for (auto e : equipos) {
        if (e->getNombre() == equipo->getNombre()) {
            return false;
        }
    }

    equipos.push_back(equipo);
    return true;
}

std::vector<std::string> Grupo::listarEquipos() const {
    std::vector<std::string> out;
    out.reserve(equipos.size());
    for (auto e : equipos) out.push_back(e->getNombre());
    return out;
}

std::vector<Equipo*> Grupo::obtenerEquipos() const {
    return equipos;
}

void Grupo::generarPartidos() {
    partidos.clear();
    if (equipos.size() < 2) {
        partidosGenerados = false;
        return;
    }

    for (size_t i = 0; i < equipos.size(); ++i) {
        for (size_t j = i + 1; j < equipos.size(); ++j) {
            partidos.emplace_back(equipos[i], equipos[j]);
        }
    }
    partidosGenerados = true;
}

bool Grupo::partidosGeneradosFlag() const {
    return partidosGenerados;
}

std::vector<std::string> Grupo::listarPartidos() const {
    std::vector<std::string> out;
    if (!partidosGenerados) return out;
    out.reserve(partidos.size());
    for (size_t i = 0; i < partidos.size(); ++i) {
        std::ostringstream ss;
        ss << (i + 1) << ") " << partidos[i].getResumen();
        out.push_back(ss.str());
    }
    return out;
}

std::vector<Partido*> Grupo::obtenerPartidosPtr() {
    std::vector<Partido*> ptrs;
    ptrs.reserve(partidos.size());
    for (auto &p : partidos) ptrs.push_back(&p);
    return ptrs;
}

bool Grupo::registrarResultado(int indicePartido, int golesLocal, int golesVisitante) {
    if (indicePartido < 1 || indicePartido > static_cast<int>(partidos.size())) {
        return false;
    }
    Partido& p = partidos[indicePartido - 1];
    p.registrarResultado(golesLocal, golesVisitante);
    return true;
}

void Grupo::calcularPuntos() {
    for (auto e : equipos) {
        e->setPuntos(0);
    }


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

std::vector< std::pair<std::string,int> > Grupo::obtenerTablaPuntos() const {
    const_cast<Grupo*>(this)->calcularPuntos();

    std::vector<std::pair<std::string,int>> table;
    table.reserve(equipos.size());
    for (auto e : equipos) table.emplace_back(e->getNombre(), e->getPuntos());

    std::sort(table.begin(), table.end(),
              [](const std::pair<std::string,int>& a, const std::pair<std::string,int>& b) {
                  if (a.second != b.second) return a.second > b.second;
                  return a.first < b.first; 
              });
    return table;
}

std::vector<Equipo*> Grupo::obtenerEquiposOrdenadosPorPuntos() const {
    const_cast<Grupo*>(this)->calcularPuntos();
    std::vector<Equipo*> ordenados = equipos;
    std::sort(ordenados.begin(), ordenados.end(),
              [](Equipo* a, Equipo* b) {
                  if (a->getPuntos() != b->getPuntos()) return a->getPuntos() > b->getPuntos();
                  return a->getNombre() < b->getNombre();
              });
    return ordenados;
}

std::string Grupo::getNombre() const {
    return nombre;
}