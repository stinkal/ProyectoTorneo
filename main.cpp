#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "estructuras/torneo.h" 

int main() {
    using std::cout;
    using std::endl;

    // Seed RNG for reproducible-ish runs using time
    std::mt19937 rng((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> goalsGroup(0, 4);  // group: 0..4 (ties allowed)
    std::uniform_int_distribution<int> goalsKO(0, 4);     // knockout RNG, will re-roll ties

    cout << "=== BACKEND TEST: TORNEO SIMULATION ===\n\n";

    // 1) Create tournament and add teams
    Torneo torneo;
    torneo.configurarTorneo(true); // group + elimination (not strictly required here)

    std::vector<std::string> names = {
        "Dragones", "Tiburones", "Halcones", "Leones",
        "Águilas", "Toros", "Gatos", "Osos"
    };

    for (const auto &n : names) {
        Equipo* e = new Equipo(n);
        if (!torneo.agregarEquipo(e)) {
            cout << "Warning: could not add equipo " << n << endl;
            delete e; // avoid leak if agregarEquipo rejected it
        }
    }

    cout << "Added teams:\n";
    auto all = torneo.getNombresEquipos();
    for (auto &s : all) cout << "  - " << s << "\n";
    cout << "\n";

    // 2) Generate groups and calendar
    cout << "Generating groups (automatic) and calendar...\n";
    torneo.generarGruposAutomatico();
    torneo.generarCalendarioGrupos();

    cout << "Matches queued: " << torneo.colaTamano() << "\n\n";

    // 3) Play all group matches (dequeue & register random results; ties allowed)
    cout << "=== PLAYING GROUP STAGE (random results, ties allowed) ===\n";
    while (!torneo.colaVacia()) {
        Partido* p = torneo.jugarPartidoSiguiente();
        if (!p) break;

        int gL = goalsGroup(rng);
        int gV = goalsGroup(rng);

        // Register as group match (esEliminatoria = false)
        bool ok = torneo.registrarResultadoEnPartido(p, gL, gV, false);
        if (!ok) {
            cout << "Failed to register group result for: " << p->getResumen() << "\n";
        } else {
            cout << "Played (grupo): " << p->getResumen() << "\n";
        }
    }

    cout << "\n-- Group summaries --\n";
    auto resumen = torneo.obtenerResumenGrupos();
    for (auto &line : resumen) cout << line << "\n";
    cout << "\n";

    // 4) Generate elimination bracket from top-2 of each group
    cout << "Generating elimination bracket from group qualifiers...\n";
    bool okLlaves = torneo.generarLlavesDesdeGrupos();
    if (!okLlaves) {
        cout << "Could not generate llaves: number of qualifiers is not power of 2.\n";
        cout << "Please adjust groups/qualifiers and try again.\n";
        return 1;
    }

    cout << "Initial elimination matches enqueued: " << torneo.colaTamano() << "\n\n";

    // 5) Play elimination matches (dequeue & register RNG results; NO TIES allowed)
    cout << "=== PLAYING ELIMINATION STAGE (random results, no ties allowed) ===\n";
    while (!torneo.colaVacia()) {
        Partido* p = torneo.jugarPartidoSiguiente();
        if (!p) break;

        int gL, gV;
        do {
            gL = goalsKO(rng);
            gV = goalsKO(rng);
        } while (gL == gV); // ensure no tie for elimination

        bool ok = torneo.registrarResultadoEnPartido(p, gL, gV, true);
        if (!ok) {
            cout << "Failed to register elimination result for: " << p->getResumen() << "\n";
        } else {
            cout << "Played (eliminatoria): " << p->getResumen() << "\n";
        }
    }

    // 6) Print final bracket and champion
    cout << "\n=== FINAL BRACKET ===\n";
    auto bracket = torneo.obtenerBracket();
    for (auto &line : bracket) cout << line << "\n";

    cout << "\nCAMPEON: " << torneo.obtenerCampeon() << "\n";

    cout << "\n=== SIMULATION COMPLETE ===\n";
    return 0;
}
