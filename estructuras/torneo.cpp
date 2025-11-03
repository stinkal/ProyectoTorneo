#include "torneo.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <functional>
#include <random>
#include <iomanip>
#include <json.hpp>  //  tener cuidado incluir esto correctamente en el frontend
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

        for (auto p : g->getPartidos()) {
            if (!p) continue;
            grupoJson["partidos"].push_back({
                {"local", p->getLocal() ? p->getLocal()->getNombre() : ""},
                {"visitante", p->getVisitante() ? p->getVisitante()->getNombre() : ""},
                {"golesLocal", p->getGolesLocal()},
                {"golesVisitante", p->getGolesVisitante()},
                {"jugado", p->estaJugado()}
            });
        }

        j["grupos"].push_back(grupoJson);
    }

    std::function<json(const NodoArbol*)> guardarNodo = [&](const NodoArbol* nodo) -> json {
        if (!nodo) return json(); // null
        json n;
        if (nodo->partido) {
            n["local"] = nodo->partido->getLocal() ? nodo->partido->getLocal()->getNombre() : "";
            n["visitante"] = nodo->partido->getVisitante() ? nodo->partido->getVisitante()->getNombre() : "";
            n["golesLocal"] = nodo->partido->getGolesLocal();
            n["golesVisitante"] = nodo->partido->getGolesVisitante();
            n["jugado"] = nodo->partido->estaJugado();
        } else {
            n = nullptr;
        }
        n["izquierda"] = guardarNodo(nodo->izquierda);
        n["derecha"] = guardarNodo(nodo->derecha);
        return n;
    };

    j["eliminacion"] = guardarNodo(arbolLlaves.raiz);

    if (partidoTercerLugar) {
        j["tercer_lugar"] = {
            {"local", partidoTercerLugar->getLocal() ? partidoTercerLugar->getLocal()->getNombre() : ""},
            {"visitante", partidoTercerLugar->getVisitante() ? partidoTercerLugar->getVisitante()->getNombre() : ""},
            {"golesLocal", partidoTercerLugar->getGolesLocal()},
            {"golesVisitante", partidoTercerLugar->getGolesVisitante()},
            {"jugado", partidoTercerLugar->estaJugado()}
        };
    } else {
        j["tercer_lugar"] = nullptr;
    }

    std::ofstream out(archivo);
    if (!out) return false;
    out << j.dump(4);  // indentacion 4
    return true;
}

bool Torneo::cargarEstado(const std::string& archivo) {
    std::ifstream in(archivo);
    if (!in) return false;

    json j;
    in >> j;

    limpiarGrupos();
    while (!calendario.vacia()) {
        try { calendario.desencolar(); } catch(...) { break; }
    }
    partidosEncolados.clear();
    for (auto e : equipos) delete e;
    equipos.clear();
    if (partidoTercerLugar) { delete partidoTercerLugar; partidoTercerLugar = nullptr; }
    // limpiar arbol
    arbolLlaves = Arbol(); 

    modalidadGrupo = j.value("modalidadGrupo", true);

    // equipos
    if (j.contains("equipos") && j["equipos"].is_array()) {
        for (auto& eJson : j["equipos"]) {
            std::string name = eJson.value("nombre", std::string());
            Equipo* e = new Equipo(name);
            e->setPuntos(eJson.value("puntos", 0));
            equipos.push_back(e);
        }
    }

    if (j.contains("grupos") && j["grupos"].is_array()) {
        for (auto& gJson : j["grupos"]) {
            Grupo* g = new Grupo(gJson.value("nombre", std::string("")));
            if (gJson.contains("equipos") && gJson["equipos"].is_array()) {
                for (auto& en : gJson["equipos"]) {
                    std::string ename = en.get<std::string>();
                    auto it = std::find_if(equipos.begin(), equipos.end(),
                        [&](Equipo* e){ return e->getNombre() == ename; });
                    if (it != equipos.end()) g->agregarEquipo(*it);
                }
            }
            if (gJson.contains("partidos") && gJson["partidos"].is_array()) {
                for (auto& pJson : gJson["partidos"]) {
                    std::string localName = pJson.value("local", std::string());
                    std::string visitName = pJson.value("visitante", std::string());
                    Equipo* local = nullptr;
                    Equipo* visit = nullptr;
                    for (auto e : equipos) {
                        if (e->getNombre() == localName) local = e;
                        if (e->getNombre() == visitName) visit = e;
                    }
                    if (local && visit) {
                        Partido* p = new Partido(local, visit);
                        if (pJson.value("jugado", false)) {
                            p->registrarResultado(pJson.value("golesLocal", 0),
                                                  pJson.value("golesVisitante", 0));
                        }
                        g->agregarPartido(p);
                    }
                }
            }
            grupos.push_back(g);
        }
    }

    if (j.contains("eliminacion")) {
        std::function<NodoArbol*(const json&)> cargarNodo = [&](const json& n) -> NodoArbol* {
            if (n.is_null()) return nullptr;
            NodoArbol* nodo = new NodoArbol();
            if (n.contains("local") && n.contains("visitante") && n["local"] != "" && n["visitante"] != "") {
                Equipo* local = nullptr;
                Equipo* visitante = nullptr;
                for (auto e : equipos) {
                    if (e->getNombre() == n["local"].get<std::string>()) local = e;
                    if (e->getNombre() == n["visitante"].get<std::string>()) visitante = e;
                }
                nodo->partido = new Partido(local, visitante);
                if (n.value("jugado", false)) {
                    nodo->partido->registrarResultado(n.value("golesLocal", 0), n.value("golesVisitante", 0));
                }
            }
            nodo->izquierda = cargarNodo(n["izquierda"]);
            nodo->derecha = cargarNodo(n["derecha"]);
            return nodo;
        };
        arbolLlaves.raiz = cargarNodo(j["eliminacion"]);
    } else {
        arbolLlaves.raiz = nullptr;
    }

    if (j.contains("tercer_lugar") && !j["tercer_lugar"].is_null()) {
        auto t = j["tercer_lugar"];
        std::string localName = t.value("local", std::string());
        std::string visitName = t.value("visitante", std::string());
        Equipo* local = nullptr;
        Equipo* visit = nullptr;
        for (auto e : equipos) {
            if (e->getNombre() == localName) local = e;
            if (e->getNombre() == visitName) visit = e;
        }
        if (local && visit) {
            partidoTercerLugar = new Partido(local, visit);
            if (t.value("jugado", false)) {
                partidoTercerLugar->registrarResultado(t.value("golesLocal", 0),
                                                      t.value("golesVisitante", 0));
            }
        } else {
            partidoTercerLugar = nullptr;
        }
    } else {
        partidoTercerLugar = nullptr;
    }
// limpia calendario y lo genera de nuevo
    while (!calendario.vacia()) {
        try { calendario.desencolar(); } catch(...) { break; }
    }
    partidosEncolados.clear();

    for (auto g : grupos) {  //  encolar grupos
        for (auto p : g->getPartidos()) {
            if (p && partidosEncolados.insert(p).second) calendario.encolar(p);
        }
    }

    arbolLlaves.jugar(arbolLlaves.getRaiz());
    encolarPartidosArbol();

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
    try {
        while (!calendario.vacia()) {
            calendario.desencolar(); // cuidado no borrar punteros aqui porque pertenecen a Arbol
        }
    } catch (...) {
    }

    for (auto g : grupos) delete g;  //  grupos contienen partidos por puntero, no por valor
    grupos.clear();

    for (auto e : equipos) delete e;
    equipos.clear();

    partidosEncolados.clear();
}

// -------------------- Equipos --------------------
bool Torneo::agregarEquipo(Equipo* e) {
    if (!e) return false;
    if (e->getNombre().empty()) return false;
    for (auto ex : equipos) {
        if (ex->getNombre() == e->getNombre()) return false; // duplicar
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

// -------------------- Grupos auto--------------------
void Torneo::limpiarGrupos() {
    for (auto g : grupos) delete g;
    grupos.clear();
}

void Torneo::generarGruposAutomatico() {
    limpiarGrupos();  //  borrar grupos existentes 

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

void Torneo::generarCalendarioGrupos() {  //  NO PERMITIR el GUI hacer esto si ya lo ha hecho
    while (!calendario.vacia()) {
        try { calendario.desencolar(); } catch(...) { break; }
    }
    partidosEncolados.clear();

    for (auto g : grupos) {  //  encola punteros a partidos
        if (!g->partidosGeneradosFlag()) g->generarPartidos();

        auto partidosPtr = g->obtenerPartidosPtr();
        for (auto p : partidosPtr) {
            if (p && partidosEncolados.insert(p).second) {
                calendario.encolar(p);
            }
        }
    }
}

// -------------------- Cola --------------------
bool Torneo::colaVacia() const { return calendario.vacia(); }
int Torneo::colaTamano() const { return calendario.tamano(); }

Partido* Torneo::jugarPartidoSiguiente() {
    if (calendario.vacia()) return nullptr;  //  GUI necesita llamar registrarResultado !!!
    Partido* siguiente = calendario.desencolar();
    return siguiente;
}
void Torneo::generarTercerLugar() { 
    if (!arbolLlaves.raiz || !arbolLlaves.raiz->izquierda || !arbolLlaves.raiz->derecha)  //  asegura que arbol existente tiene semifinales
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

// -------------------- registro de resultados --------------------
bool Torneo::registrarResultadoEnPartido(Partido* p, int gL, int gV, bool esEliminatoria) {
    if (!p) return false;    
    if (!esEliminatoria) esEliminatoria = esPartidoDeEliminatoria(p);
    if (esEliminatoria && gL == gV) return false;

    p->registrarResultado(gL, gV);

    for (auto g : grupos) {  //  si el partido pertenece a un grupo, calcula puntos de ese grupo
        auto ptrs = g->obtenerPartidosPtr();
        if (std::find(ptrs.begin(), ptrs.end(), p) != ptrs.end()) {
            g->calcularPuntos();
            break;
        }
    }

    if (esEliminatoria) {  //  encola partidos siguientes cuando hay ganador en arbol eliminatorio
        arbolLlaves.jugar(arbolLlaves.getRaiz());       
        encolarPartidosArbol();                     
    }

    return true;
}

// -------------------- Reportes / resumen de grupos --------------------    
std::vector<std::string> Torneo::obtenerResumenGrupos() const {
    std::vector<std::string> resumen;
    for (auto g : grupos) {
        const_cast<Grupo*>(g)->calcularPuntos();  //  revisa que puntos estan calculados

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

    // campeón, subcampeón, tercer lugar
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

    if (modalidadGrupo && !grupos.empty()) {  //  grupos
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

    out << "===== FASE DE ELIMINACIÓN =====\n";   //  fase eliminatoria
    out << arbolLlaves.getTextoEstructura(); 
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

    std::shuffle(clasificados.begin(), clasificados.end(), rng);  //  calificados colocados aleatoriamente

    if (!esPotenciaDe2(static_cast<int>(clasificados.size()))) {  //  NECESITA RESOLVER EN EL GUI
        return false;
    }

    return generarLlavesDirectas(clasificados);
}

bool Torneo::generarLlavesDirectas(const std::vector<Equipo*>& equiposClasificados) {
    if (equiposClasificados.empty()) return false;
    if (!esPotenciaDe2(static_cast<int>(equiposClasificados.size()))) return false;

    arbolLlaves.construir(equiposClasificados);  //  construye arbol

    arbolLlaves.jugar(arbolLlaves.getRaiz());

    encolarPartidosArbol();  //  encolar primer ronda de partidos
    return true;
}

void Torneo::encolarPartidosArbol() {
    std::function<void(NodoArbol*)> post = [&](NodoArbol* n) {  //  para encolar hojas primero, y luego padres cuando hay ganador
        if (!n) return;
        post(n->izquierda);
        post(n->derecha);
        if (n->partido && !n->partido->estaJugado()) {
            if (partidosEncolados.insert(n->partido).second) {
                calendario.encolar(n->partido);
            }
        }
    };

    post(arbolLlaves.getRaiz());
}

Partido* Torneo::jugarLlaveSiguiente() {  //  tira el siguiente partido pendienet
    NodoArbol* nodo = arbolLlaves.buscarPrimerPartidoNoJugado();
    if (!nodo) return nullptr;
    return nodo->partido;
}

std::vector<std::string> Torneo::obtenerBracket() const {  //  resumenes in-order
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
