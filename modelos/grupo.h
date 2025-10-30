#ifndef GRUPO_H
#define GRUPO_H

#include <vector>
#include <string>
#include <iostream>
#include "equipo.h"
#include "partido.h"

class Grupo {
public:
    Grupo(const std::string& nombre);

    bool agregarEquipo(Equipo* equipo);
    void listarEquipos() const;

    void generarPartidos();  // seran round-robin
    void listarPartidos() const;

    void registrarResultado(int indicePartido, int golesLocal, int golesVisitante);
    void calcularPuntos();
    void mostrarTablaPuntos() const;

    std::vector<Equipo*> getEquipos() const;
    std::vector<Partido*> getPartidosPtr();

private:
    std::string nombre;   //podrian ser numeros, pero creo que es mejor "grupo A, grupo B, etc."
    std::vector<Equipo*> equipos;
    std::vector<Partido> partidos;

    bool partidosGenerados;
};


#endif