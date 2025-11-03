#ifndef EQUIPOS_PER_H
#define EQUIPOS_PER_H

#include <string>
#include <vector>
#include "../modelos/equipo.h"
#include "torneo.h"

class EquiposPer {
public:
    EquiposPer(const std::string& filepath = "equipos.json");
    ~EquiposPer();

    Equipo* crearEquipo(const std::string& nombre);   
    bool editarEquipo(const std::string& oldName, const std::string& newName); 
    bool eliminarEquipo(const std::string& nombre);       

    const std::vector<Equipo*>& getAll() const;
    Equipo* findByName(const std::string& name) const;

    bool cargarDesdeArchivo();
    bool guardarAArchivo() const;

    void setFilepath(const std::string& path);
    std::string getFilepath() const;

    void setTorneoActivo(Torneo* t);

private:
    std::vector<Equipo*> equipos;
    std::string filepath;

    bool guardarInterno() const;

    void clearAll();
    Torneo* torneoActivo;
};

#endif
