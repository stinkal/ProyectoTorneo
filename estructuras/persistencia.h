#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
#include <vector>
#include "../modelos/equipo.h"

namespace persistencia {

    bool guardarEquiposJSON(const std::vector<Equipo*>& equipos, const std::string& filepath);

    bool cargarEquiposJSON(std::vector<Equipo*>& outEquipos, const std::string& filepath);

}

#endif
