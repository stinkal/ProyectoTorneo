#ifndef TORNEO_H
#define TORNEO_H

#include <vector>
#include <string>
#include <random>
#include "../modelos/equipo.h"
#include "../modelos/grupo.h"
#include "../modelos/partido.h"
#include "arbol.h"
#include "colapartidos.h"

class Torneo {
private:
    std::vector<Equipo*> equipos;
    std::vector<Grupo*> grupos;
    ColaPartidos calendario;
    Arbol arbolLlaves;
    bool modalidadGrupo;
    mutable std::mt19937 rng;  //  aleatorio
    std::vector<Equipo*> getClasificados() const;
    void limpiarGrupos();

public:
    Torneo();
    ~Torneo();   //  destructor

    bool agregarEquipos(Equipo* e);
    bool eliminarEquipo(const std::string& nombre);
    std::vector<std::string> getNombresEquipos() const;

    void configurarTorneo(bool modoGrupo);


};

#endif