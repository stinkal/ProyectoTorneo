#ifndef EQUIPO_H
#define EQUIPO_H
#include <string>
using namespace std;

class Equipo {
public:
    Equipo(const string& nombre);
    string getNombre() const;
    int getPuntos() const;
    void setNombre(const string& nombre);
    void setPuntos(int puntos);
    void sumarPuntos(int puntos);
private:
    string nombre;
    int puntos;
};

#endif