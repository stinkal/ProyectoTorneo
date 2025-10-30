#ifndef GRUPO_H
#define GRUPO_H

#include <vector>
#include <string>
#include <iostream>
#include "equipo.h"
#include "partido.h"
using namespace std;

class Grupo {
public:
    Grupo(const string& nombre);

    bool agregarEquipo(Equipo* equipo);
    void listarEquipos() const;

    void generarPartidos();  // seran round-robin
    void listarPartidos() const;

    void registrarResultado(int indicePartido, int golesLocal, int golesVisitante);
    void calcularPuntos();
    void mostrarTablaPuntos() const;

    vector<Equipo*> getEquipos() const;
    vector<Partido*> getPartidosPtr();
    
private:
    string nombre;   //podrian ser numeros, pero creo que es mejor "grupo A, grupo B, etc."
    vector<Equipo*> equipos;
    vector<Partido> partidos;

    bool partidosGenerados;
};


#endif