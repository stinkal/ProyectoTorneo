#ifndef GRUPO_H
#define GRUPO_H

#include <vector>
#include <string>
#include "equipo.h"
#include "partido.h"

class Grupo {
public:
    Grupo(const std::string& nombre);

    bool agregarEquipo(Equipo* equipo);
    std::vector<std::string> listarEquipos() const;
    std::vector<Equipo*> obtenerEquipos() const;

    void generarPartidos();  // seran round-robin
    bool partidosGeneradosFlag() const;
    std::vector<std::string> listarPartidos() const;
    std::vector<Partido*> obtenerPartidosPtr();

    bool registrarResultado(int indicePartido, int golesLocal, int golesVisitante);
    void calcularPuntos();
    
    std::vector< std::pair<std::string,int> > obtenerTablaPuntos() const;
    std::vector<Equipo*> obtenerEquiposOrdenadosPorPuntos() const;

    std::string getNombre() const;

private:
    std::string nombre;   //podrian ser numeros, pero creo que es mejor "grupo A, grupo B, etc."
    std::vector<Equipo*> equipos;
    std::vector<Partido> partidos;

    bool partidosGenerados;
};


#endif