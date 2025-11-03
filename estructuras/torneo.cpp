#include "torneo.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <functional>
#include <random>
#include <iomanip>
#include <json.hpp>
using json = nlohmann::json;

bool Torneo::guardarEstado(const std::string& archivo) const {
    json j;

    j["modalidadGrupo"] = modalidadGrupo;

    // Equipos
    for (auto e : equipos) {
        j["equipos"].push_back({
            {"nombre", e->getNombre()},
            {"puntos", e->getPuntos()}
        });
    }

    // Grupos
    for (auto g : grupos) {
        json grupoJson;
        grupoJson["nombre"] = g->getNombre();
        for (auto e : g->getEquipos()) {
            grupoJson["equipos"].push_back(e->getNombre());
        }

        // Partidos
        for (auto& p : g->getPartidos()) {
            grupoJson["partidos"].push_back({
                {"local", p.getLocal()->getNombre()},
                {"visitante", p.getVisitante()->getNombre()},
                {"golesLocal", p.getGolesLocal()},
                {"golesVisitante", p.getGolesVisitante()},
                {"jugado", p.estaJugado()}
            });
        }

        j["grupos"].push_back(grupoJson);
    }

    // Elimination tree
    std::function<json(const NodoArbol*)> guardarNodo = [&](const NodoArbol* nodo) -> json {
        if (!nodo) return nullptr;
        json n;
        if (nodo->partido) {
            n["local"] = nodo->partido->getLocal() ? nodo->partido->getLocal()->getNombre() : "";
            n["visitante"] = nodo->partido->getVisitante() ? nodo->partido->getVisitante()->getNombre() : "";
            n["golesLocal"] = nodo->partido->getGolesLocal();
            n["golesVisitante"] = nodo->partido->getGolesVisitante();
            n["jugado"] = nodo->partido->estaJugado();
        }
        n["izquierda"] = guardarNodo(nodo->izquierda);
        n["derecha"] = guardarNodo(nodo->derecha);
        return n;
    };

    j["eliminacion"] = guardarNodo(arbolLlaves.raiz);

    std::ofstream out(archivo);
    if (!out) return false;
    out << j.dump(4);  // pretty print with indent = 4
    return true;
}

bool Torneo::cargarEstado(const std::string& archivo) {
    std::ifstream in(archivo);
    if (!in) return false;

    json j;
    in >> j;

    limpiarGrupos();
    equipos.clear();

    modalidadGrupo = j["modalidadGrupo"];

    // Equipos
    for (auto& eJson : j["equipos"]) {
        Equipo* e = new Equipo(eJson["nombre"]);
        e->setPuntos(eJson["puntos"]);
        equipos.push_back(e);
    }

    // Grupos
    for (auto& gJson : j["grupos"]) {
        Grupo* g = new Grupo(gJson["nombre"]);
        for (auto& en : gJson["equipos"]) {
            auto it = std::find_if(equipos.begin(), equipos.end(),
                [&](Equipo* e){ return e->getNombre() == en; });
            if (it != equipos.end()) g->agregarEquipo(*it);
        }
        grupos.push_back(g);
    }

    // Elimination Tree (recursively)
    std::function<NodoArbol*(const json&)> cargarNodo = [&](const json& n) -> NodoArbol* {
        if (n.is_null()) return nullptr;
        NodoArbol* nodo = new NodoArbol();

        if (n.contains("local") && n["local"] != "" && n.contains("visitante") && n["visitante"] != "") {
            Equipo* local = nullptr;
            Equipo* visitante = nullptr;

            for (auto e : equipos) {
                if (e->getNombre() == n["local"]) local = e;
                if (e->getNombre() == n["visitante"]) visitante = e;
            }

            nodo->partido = new Partido(local, visitante);
            nodo->partido->registrarResultado(n["golesLocal"], n["golesVisitante"]);
        }

        nodo->izquierda = cargarNodo(n["izquierda"]);
        nodo->derecha = cargarNodo(n["derecha"]);
        return nodo;
    };

    arbolLlaves.raiz = cargarNodo(j["eliminacion"]);

    return true;
}

Torneo::Torneo() : modalidadGrupo(true) {
    std::random_device rd;
    rng.seed(rd());
}
bool Torneo::esPartidoDeEliminatoria(Partido* p) const {
    return arbolLlaves.contienePartido(p);
}
Torneo::~Torneo() {
    // Clear the queue so it doesn't hold pointers into group's partido vectors or arbol-owned partidos.
    try {
        while (!calendario.vacia()) {
            calendario.desencolar(); // do not delete Partido* here (owners will delete)
        }
    } catch (...) {
        // ignore in destructor
    }

    // Delete groups (they contain partidos by value)
    for (auto g : grupos) delete g;
    grupos.clear();

    // Delete equipos
    for (auto e : equipos) delete e;
    equipos.clear();

    // Clear enqueued set
    partidosEncolados.clear();
}

// -------------------- Equipos --------------------
bool Torneo::agregarEquipo(Equipo* e) {
    if (!e) return false;
    if (e->getNombre().empty()) return false;
    for (auto ex : equipos) {
        if (ex->getNombre() == e->getNombre()) return false; // duplicate
    }
    equipos.push_back(e);
    return true;
}

bool Torneo::eliminarEquipo(const std::string& nombre) {
    for (auto it = equipos.begin(); it != equipos.end(); ++it) {
        if ((*it)->getNombre() == nombre) {
            delete *it;
            equipos.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<std::string> Torneo::getNombresEquipos() const {
    std::vector<std::string> out;
    out.reserve(equipos.size());
    for (auto e : equipos) out.push_back(e->getNombre());
    return out;
}

bool Torneo::contieneEquipo(const std::string& nombre) const {  //  no permite borrar equipo que esta en torneo actual
    for (auto e : equipos) {
        if (e->getNombre() == nombre) return true;
    }
    return false;
}

// -------------------- Configuración --------------------
void Torneo::configurarTorneo(bool modoGrupo) {
    modalidadGrupo = modoGrupo;
}

bool Torneo::esModalidadGrupo() const {
    return modalidadGrupo;
}

// -------------------- Grupos (auto-generation) --------------------
void Torneo::limpiarGrupos() {
    for (auto g : grupos) delete g;
    grupos.clear();
}

void Torneo::generarGruposAutomatico() {
    // Delete any existing groups (but do not delete equipos)
    limpiarGrupos();

    if (equipos.empty()) return;

    const int perGroup = 4;
    int total = static_cast<int>(equipos.size());
    int idx = 0;
    int gi = 0;

    while (idx < total) {
        ++gi;
        std::stringstream ss;
        ss << "Grupo " << char('A' + (gi - 1));
        Grupo* g = new Grupo(ss.str());
        for (int j = 0; j < perGroup && idx < total; ++j, ++idx) {
            g->agregarEquipo(equipos[idx]);
        }
        grupos.push_back(g);
    }
}

void Torneo::generarCalendarioGrupos() {
    // For each group: generate matches and enqueue pointers to those matches
    for (auto g : grupos) {
        g->generarPartidos();
        auto partidosPtr = g->obtenerPartidosPtr(); // Grupo provides pointers to its partidos
        for (auto p : partidosPtr) {
            // Only enqueue if not already enqueued (defensive)
            if (partidosEncolados.insert(p).second) {
                calendario.encolar(p);
            }
        }
    }
}

// -------------------- Cola --------------------
bool Torneo::colaVacia() const { return calendario.vacia(); }
int Torneo::colaTamano() const { return calendario.tamano(); }

Partido* Torneo::jugarPartidoSiguiente() {
    if (calendario.vacia()) return nullptr;
    // Dequeue and return the partido; caller (GUI) should call registrarResultado on it.
    Partido* siguiente = calendario.desencolar();
    // note: partido remains considered "enqueued" in our set even after dequeuing - we keep it to avoid re-enqueue later
    return siguiente;
}
void Torneo::generarTercerLugar() {
    // Ensure the knockout tree exists and has semifinals
    if (!arbolLlaves.raiz || !arbolLlaves.raiz->izquierda || !arbolLlaves.raiz->derecha)
        return;

    NodoArbol* semi1 = arbolLlaves.raiz->izquierda;
    NodoArbol* semi2 = arbolLlaves.raiz->derecha;

    if (!semi1->partido || !semi2->partido)
        return;

    Equipo* perdedor1 = arbolLlaves.obtenerPerdedor(semi1->partido);
    Equipo* perdedor2 = arbolLlaves.obtenerPerdedor(semi2->partido);

    if (perdedor1 && perdedor2)
        partidoTercerLugar = new Partido(perdedor1, perdedor2);
    else
        partidoTercerLugar = nullptr;
}

// -------------------- Centralized result registration --------------------
bool Torneo::registrarResultadoEnPartido(Partido* p, int gL, int gV, bool esEliminatoria) {
    if (!p) return false;    
    if (!esEliminatoria) esEliminatoria = esPartidoDeEliminatoria(p);
    if (esEliminatoria && gL == gV) return false;

    // Register result
    p->registrarResultado(gL, gV);

    // If this partido belongs to a group, recalculate that group's points
    for (auto g : grupos) {
        auto ptrs = g->obtenerPartidosPtr();
        if (std::find(ptrs.begin(), ptrs.end(), p) != ptrs.end()) {
            g->calcularPuntos();
            break;
        }
    }

    // If elimination match, propagate winners and enqueue newly-ready matches
    if (esEliminatoria) {
        arbolLlaves.jugar(arbolLlaves.getRaiz());          // propagate winners, create parent Partido* where possible
        encolarPartidosArbol();                     // enqueue any new elimination matches
    }

    return true;
}

// -------------------- Reportes / resumen de grupos --------------------     DOUBLE CHECK WITH DEEP THINKING
std::vector<std::string> Torneo::obtenerResumenGrupos() const {
    std::vector<std::string> resumen;
    for (auto g : grupos) {
        // Ensure points are computed
        const_cast<Grupo*>(g)->calcularPuntos();

        std::ostringstream ss;
        ss << g->getNombre() << ":";
        auto tabla = g->obtenerTablaPuntos(); // vector<pair<string,int>>
        bool first = true;
        for (auto &row : tabla) {
            if (!first) ss << " | ";
            ss << row.first << "(" << row.second << ")";
            first = false;
        }
        resumen.push_back(ss.str());
    }
    return resumen;
}
std::string Torneo::generarReporteTexto() const {
    std::ostringstream out;

    out << "===== REPORTE DEL TORNEO =====\n\n";
    out << "Modalidad: " << (modalidadGrupo ? "Grupos + Eliminación" : "Eliminación directa") << "\n";
    out << "Equipos participantes: " << equipos.size() << "\n\n";

    // Campeón, subcampeón, tercer lugar
    Equipo* campeon = arbolLlaves.obtenerCampeon();
    Equipo* subcampeon = arbolLlaves.obtenerSubcampeon();   // TODO
    Equipo* tercero = nullptr;

    if (partidoTercerLugar && partidoTercerLugar->estaJugado()) {
        tercero = (partidoTercerLugar->getGolesLocal() > partidoTercerLugar->getGolesVisitante())
                    ? partidoTercerLugar->getLocal()
                    : partidoTercerLugar->getVisitante();
    }

    if (campeon) out << "Campeón: " << campeon->getNombre() << "\n";
    if (subcampeon) out << "Subcampeón: " << subcampeon->getNombre() << "\n";
    if (tercero) out << "Tercer lugar: " << tercero->getNombre() << "\n";
    out << "\n";

    // Group tables
    if (modalidadGrupo && !grupos.empty()) {
        out << "===== TABLAS DE GRUPOS =====\n";
        for (auto g : grupos) {
            out << "Grupo " << g->getNombre() << "\n";
            auto equiposGrupo = g->obtenerEquiposOrdenadosPorPuntos();
            for (auto e : equiposGrupo) {
                out << std::setw(20) << e->getNombre() << e->getPuntos() << " pts\n";
            }
            out << "\n";
        }
    }

    // Bracket
    out << "===== FASE DE ELIMINACIÓN =====\n";
    out << arbolLlaves.getTextoEstructura(); // Return a string representing the tree (see below)
    out << "\n===== FIN DEL REPORTE =====\n";

    return out.str();
}

// -------------------- Clasificados y llaves --------------------
std::vector<Equipo*> Torneo::getClasificados() const {
    std::vector<Equipo*> clasificados;
    for (auto g : grupos) {
        const_cast<Grupo*>(g)->calcularPuntos();
        auto orden = g->obtenerEquiposOrdenadosPorPuntos();
        if (orden.size() >= 1) clasificados.push_back(orden[0]);
        if (orden.size() >= 2) clasificados.push_back(orden[1]);
    }
    return clasificados;
}

bool Torneo::esPotenciaDe2(int n) {
    if (n < 1) return false;
    return (n & (n - 1)) == 0;
}

bool Torneo::generarLlavesDesdeGrupos() {
    auto clasificados = getClasificados();
    if (clasificados.empty()) return false;

    // Shuffle qualifiers for random placement in bracket
    std::shuffle(clasificados.begin(), clasificados.end(), rng);

    if (!esPotenciaDe2(static_cast<int>(clasificados.size()))) {
        // Not a power of two: GUI must resolve (byes or manual selection)
        return false;
    }

    return generarLlavesDirectas(clasificados);
}

bool Torneo::generarLlavesDirectas(const std::vector<Equipo*>& equiposClasificados) {
    if (equiposClasificados.empty()) return false;
    if (!esPotenciaDe2(static_cast<int>(equiposClasificados.size()))) return false;

    // Build arbol; Arbol will create and own Partido* for elimination matches.
    arbolLlaves.construir(equiposClasificados);

    // Propagate winners (this will create parent Partido* where both child winners exist)
    arbolLlaves.jugar(arbolLlaves.getRaiz());

    // Enqueue first-round matches (leaves) into the calendario
    encolarPartidosArbol();
    return true;
}

// -------------------- Enqueue helper for arbol matches --------------------
void Torneo::encolarPartidosArbol() {
    // post-order traversal so leaves (first round) are seen before parents
    std::function<void(NodoArbol*)> post = [&](NodoArbol* n) {
        if (!n) return;
        post(n->izquierda);
        post(n->derecha);
        if (n->partido && !n->partido->estaJugado()) {
            // enqueue only if not already enqueued
            if (partidosEncolados.insert(n->partido).second) {
                calendario.encolar(n->partido);
            }
        }
    };

    post(arbolLlaves.getRaiz());
}

// Returns next unplayed elimination match (Partido*) or nullptr if none
Partido* Torneo::jugarLlaveSiguiente() {
    NodoArbol* nodo = arbolLlaves.buscarPrimerPartidoNoJugado();
    if (!nodo) return nullptr;
    return nodo->partido;
}

// Simple traversal to collect all matches with summaries (in-order-like)
std::vector<std::string> Torneo::obtenerBracket() const {
    std::vector<std::string> lines;
    std::function<void(NodoArbol*)> dfs = [&](NodoArbol* n) {
        if (!n) return;
        dfs(n->izquierda);
        if (n->partido) lines.push_back(n->partido->getResumen());
        dfs(n->derecha);
    };
    dfs(arbolLlaves.getRaiz());
    return lines;
}

std::string Torneo::obtenerCampeon() const {
    Equipo* c = arbolLlaves.obtenerCampeon();
    return c ? c->getNombre() : std::string("");
}
