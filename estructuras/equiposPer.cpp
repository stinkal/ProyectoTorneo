#include "equiposper.h"
// #include "../persistencia/persistencia.h" // uses guardarEquiposJSON / cargarEquiposJSON
#include <algorithm>

EquiposPer::EquiposPer(const std::string& filepath_)
    : filepath(filepath_) {
    // Try to load existing teams
    cargarDesdeArchivo();
}

EquiposPer::~EquiposPer() {
    clearAll();
}

void EquiposPer::clearAll() {
    for (auto e : equipos) delete e;
    equipos.clear();
}

const std::vector<Equipo*>& EquiposPer::getAll() const {
    return equipos;
}

Equipo* EquiposPer::findByName(const std::string& name) const {
    for (auto e : equipos) {
        if (e->getNombre() == name) return e;
    }
    return nullptr;
}

bool EquiposPer::guardarInterno() const {
    return persistencia::guardarEquiposJSON(equipos, filepath);
}

bool EquiposPer::guardarAArchivo() const {
    return guardarInterno();
}

bool EquiposPer::cargarDesdeArchivo() {
    // free current memory first
    clearAll();
    std::vector<Equipo*> loaded;
    if (!persistencia::cargarEquiposJSON(loaded, filepath)) {
        // no file or parse fail => treat as empty (do not fail constructor)
        // Make sure loaded memory is owned by manager if loaded succeeded; if failed, nothing to do.
        for (auto e : loaded) delete e;
        return false;
    }
    equipos = std::move(loaded);
    return true;
}

Equipo* EquiposPer::crearEquipo(const std::string& nombre) {
    if (nombre.empty()) return nullptr;
    if (findByName(nombre) != nullptr) return nullptr; // duplicate

    Equipo* e = new Equipo(nombre);
    equipos.push_back(e);
    // autosave best-effort
    guardarInterno();
    return e;
}

bool EquiposPer::editarEquipo(const std::string& oldName, const std::string& newName) {
    if (newName.empty()) return false;
    if (oldName == newName) return true; // no change
    if (findByName(newName) != nullptr) return false; // collision

    Equipo* e = findByName(oldName);
    if (!e) return false;
    e->setNombre(newName);
    guardarInterno();
    return true;
}

bool EquiposPer::eliminarEquipo(const std::string& nombre) {
    if (torneoActivo && torneoActivo->contieneEquipo(nombre)) {
        std::cout << "No se puede eliminar el equipo " << nombre
                  << " porque está participando en un torneo activo.\n";
        return false;
    }

    for (auto it = equipos.begin(); it != equipos.end(); ++it) {
        if ((*it)->getNombre() == nombre) {
            delete *it;
            equipos.erase(it);
            guardarInterno();
            return true;
        }
    }
    return false;
}

void EquiposPer::setFilepath(const std::string& path) {
    filepath = path;
}

std::string EquiposPer::getFilepath() const {
    return filepath;
}
