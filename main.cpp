#include <iostream>
#include "modelos/equipo.h"
#include "modelos/partido.h"
#include "modelos/grupo.h"

// MUCHOS IMPLEMENTACIONES SON TEMPORALES PARA DEMOSTRAR FUNCIONALIDAD EN LA CONSOLA

int main() {
    Equipo e1("Equipo 1");   //eventualmente seran widgets qmake
    Equipo e2("Equipo 2");  
    Equipo e3("Equipo 3");
    Equipo e4("Equipo 4");

    Grupo grupoA("A");
    grupoA.agregarEquipo(&e1);
    grupoA.agregarEquipo(&e2);
    grupoA.agregarEquipo(&e3);
    grupoA.agregarEquipo(&e4);

    grupoA.listarEquipos();
    grupoA.generarPartidos();
    grupoA.listarPartidos();

    grupoA.registrarResultado(1, 2, 1);
    grupoA.registrarResultado(2, 0, 0);
    grupoA.registrarResultado(3, 3, 1);
    grupoA.registrarResultado(4, 2, 1);
    grupoA.registrarResultado(5, 0, 0);
    grupoA.registrarResultado(6, 3, 1);

    grupoA.calcularPuntos();
    grupoA.mostrarTablaPuntos();

    return 0;
}
