#ifndef EQUIPO_H
#define EQUIPO_H
#include <string>

class Equipo {
public:
    Equipo(const std::string& nombre);
    std::string getNombre() const;
    int getPuntos() const;
    void setNombre(const std::string& nombre);
    void setPuntos(int puntos);
    void sumarPuntos(int puntos);
private:
    std::string nombre;
    int puntos;
};

#endif