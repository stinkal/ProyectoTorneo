#include "arbol.h"
#include <sstream>
#include <string>
#include <functional>
using namespace std;

Arbol::Arbol() : raiz(nullptr) {}
Arbol::~Arbol() {
    destruirRec(raiz);
    raiz = nullptr;
}
NodoArbol* Arbol::getRaiz() const { return raiz; }
void Arbol::destruirRec(NodoArbol* nodo) {   //  cuidado que no encole punteros en ningun otro lado pq aqui los destruyan
    if (!nodo) return;
    if (nodo->izquierda) destruirRec(nodo->izquierda);
    if (nodo->derecha) destruirRec(nodo->derecha);
    if (nodo->partido) {
        delete nodo->partido;
        nodo->partido = nullptr;
    }
    delete nodo;
}
void Arbol::clear() {
    destruirRec(raiz);
    raiz = nullptr;
}

static NodoArbol* buscarPrimerNoJugadoRec(NodoArbol* nodo) {
    if (!nodo) return nullptr;
    NodoArbol* left = buscarPrimerNoJugadoRec(nodo->izquierda);
    if (left) return left;
    NodoArbol* right = buscarPrimerNoJugadoRec(nodo->derecha);
    if (right) return right;
    if (nodo->partido && !nodo->partido->estaJugado()) return nodo;
    return nullptr;
}

NodoArbol* Arbol::buscarPrimerPartidoNoJugado() const {
    return buscarPrimerNoJugadoRec(raiz);
}

vector<Partido*> Arbol::obtenerPartidosPendientes() const {
    vector<Partido*> pendientes;
    function<void(NodoArbol*)> dfs = [&](NodoArbol* n) {
        if (!n) return;
        dfs(n->izquierda);
        dfs(n->derecha);
        if (n->partido && !n->partido->estaJugado()) pendientes.push_back(n->partido);
    };
    dfs(raiz);
    return pendientes;
}

NodoArbol* Arbol::construirRec(const vector<Equipo*>& equipos, int inicio, int fin) {
    if (inicio > fin) return nullptr;

    if (inicio + 1 == fin) {
        return new NodoArbol(new Partido(equipos[inicio], equipos[fin]));  //   caso base
    }

    int mid = (inicio + fin) / 2;
    NodoArbol* izquierda = construirRec(equipos, inicio, mid);
    NodoArbol* derecha = construirRec(equipos, mid + 1, fin);

    NodoArbol* nodo = new NodoArbol(nullptr);  //   creando el siguiente nodo para llenar luego
    nodo->izquierda = izquierda;
    nodo->derecha = derecha;

    return nodo;
}

NodoArbol* Arbol::construir(const vector<Equipo*>& equipos) {
    raiz = construirRec(equipos, 0, equipos.size() - 1);
    return raiz;
}

Equipo* Arbol::determinarGanador(Partido* partido) const {
    if (!partido->estaJugado()) return nullptr;
    if (partido->getGolesLocal() > partido->getGolesVisitante())
        return partido->getLocal();
    else if (partido->getGolesVisitante() > partido->getGolesLocal())
        return partido->getVisitante();
    else
        return nullptr; // empate
}

std::string Arbol::getTextoEstructura() const {
    std::ostringstream out;
    std::function<void(const NodoArbol*, int)> rec = [&](const NodoArbol* nodo, int nivel) {
        if (!nodo) return;
        rec(nodo->derecha, nivel + 1);
        out << std::string(nivel * 6, ' ');
        if (nodo->partido && nodo->partido->getLocal() && nodo->partido->getVisitante())
            out << nodo->partido->getLocal()->getNombre() << " vs "
                << nodo->partido->getVisitante()->getNombre();
        else
            out << "(pendiente)";
        out << "\n";
        rec(nodo->izquierda, nivel + 1);
    };
    rec(raiz, 0);
    return out.str();
}

void Arbol::jugar(NodoArbol* nodo) {
    if (!nodo) return;

    if (nodo->izquierda) jugar(nodo->izquierda);   //recursion
    if (nodo->derecha) jugar(nodo->derecha);

    if (!nodo->partido && nodo->izquierda && nodo->derecha) {
        Equipo* ganadorIzq = determinarGanador(nodo->izquierda->partido);
        Equipo* ganadorDer = determinarGanador(nodo->derecha->partido);
        if (ganadorIzq && ganadorDer)
            nodo->partido = new Partido(ganadorIzq, ganadorDer);
        else return; //  si hacen falta resultados
    }
}

Equipo* Arbol::obtenerCampeon() const {
    if (!raiz || !raiz->partido) return nullptr;
    return determinarGanador(raiz->partido);
}

Equipo* Arbol::obtenerSubcampeon() const {
    if (!raiz || !raiz->partido) return nullptr;
    Equipo* ganador = obtenerCampeon();
    Equipo* local = raiz->partido->getLocal();
    Equipo* visitante = raiz->partido->getVisitante();
    if (ganador == local) return visitante;
    if (ganador == visitante) return local;
    return nullptr;
}

void Arbol::mostrar(std::ostream& out, NodoArbol* nodo, int nivel) const {
    if (!nodo) return;
    mostrar(out, nodo->derecha, nivel + 1);
    out << std::string(nivel * 6, ' ');
    if (nodo->partido && nodo->partido->getLocal() && nodo->partido->getVisitante())
        out << nodo->partido->getLocal()->getNombre() << " vs "
            << nodo->partido->getVisitante()->getNombre();
    else
        out << "(pendiente)";
    out << "\n";
    mostrar(out, nodo->izquierda, nivel + 1);
}


Equipo* Arbol::obtenerPerdedor(Partido* p) const {
    if (!p || !p->estaJugado()) return nullptr;
    if (p->getGolesLocal() > p->getGolesVisitante()) return p->getVisitante();
    return p->getLocal();
}

static bool contieneRec(NodoArbol* nodo, Partido* p) {
    if (!nodo) return false;
    if (nodo->partido == p) return true;
    return contieneRec(nodo->izquierda, p) || contieneRec(nodo->derecha, p);
}

bool Arbol::contienePartido(Partido* p) const {
    return contieneRec(raiz, p);
}
