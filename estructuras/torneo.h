#ifndef TORNEO_H
#define TORNEO_H

#include <vector>
#include <string>
#include <random>
#include <unordered_set>
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
    static bool esPotenciaDe2(int n);
    std::unordered_set<Partido*> partidosEncolados;
    void encolarPartidosArbol();
    Partido* partidoTercerLugar;

public:
    Torneo();
    ~Torneo();   //  destructor

    bool agregarEquipo(Equipo* e);
    bool eliminarEquipo(const std::string& nombre);
    std::vector<std::string> getNombresEquipos() const;

    void configurarTorneo(bool modoGrupo);
    bool esModalidadGrupo() const;
    void generarGruposAutomatico();
    void generarCalendarioGrupos();

    bool colaVacia() const;
    int colaTamano() const;
    Partido* jugarPartidoSiguiente();
    bool registrarResultadoEnPartido(Partido* p, int golesLocal, int golesVisitante, bool esEliminatoria = false);

    std::vector<std::string> obtenerResumenGrupos() const;
    bool generarLlavesDesdeGrupos();
    bool generarLlavesDirectas(const std::vector<Equipo*>& equiposClasificados);
    Partido* jugarLlaveSiguiente();
    std::vector<std::string> obtenerBracket() const;
    std::string obtenerCampeon() const;
    Partido* getPartidoTercerLugar() const { return partidoTercerLugar; }
    void generarTercerLugar();

    bool esPartidoDeEliminatoria(Partido* p) const;

    bool contieneEquipo(const std::string& nombre) const;
    std::string generarReporteTexto() const;
    bool guardarEstado(const std::string& archivo) const;
    bool cargarEstado(const std::string& archivo);

};

#endif