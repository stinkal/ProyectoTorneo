#include "equipo.h"

Equipo::Equipo(const std::string& nombre) : nombre(nombre), puntos(0) {}

std::string Equipo::getNombre() const {
    return nombre;
}

int Equipo::getPuntos() const {
    return puntos;
}

void Equipo::setNombre(const std::string& n) {
    nombre = n;
}

void Equipo::setPuntos(int p) {
    puntos = p;
}

void Equipo::sumarPuntos(int p) {
    puntos += p;
}
