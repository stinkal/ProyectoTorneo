#include "ventanagrupos.h"
#include "ui_ventanagrupos.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <random>
#include <algorithm>

VentanaGrupos::VentanaGrupos(Torneo* torneo, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::VentanaGrupos),
    torneoActual(torneo)
{
    ui->setupUi(this);

    connect(ui->btnGenerarAuto, &QPushButton::clicked, this, &VentanaGrupos::alGenerarAuto);
    connect(ui->btnCrearManual, &QPushButton::clicked, this, &VentanaGrupos::alCrearManual);
    connect(ui->btnEliminar,    &QPushButton::clicked, this, &VentanaGrupos::alEliminar);
    connect(ui->btnLimpiar,     &QPushButton::clicked, this, &VentanaGrupos::alLimpiar);
    connect(ui->btnActualizar,  &QPushButton::clicked, this, &VentanaGrupos::alActualizar);
    connect(ui->btnGuardar,     &QPushButton::clicked, this, &VentanaGrupos::alGuardar);
    connect(ui->btnCancelar,    &QPushButton::clicked, this, &VentanaGrupos::alCancelar);

    resetDisponiblesFromTorneo();
    actualizarListas();
}

VentanaGrupos::~VentanaGrupos()
{
    delete ui;
}

void VentanaGrupos::resetDisponiblesFromTorneo()
{
    equiposDisponibles.clear();
    grupos.clear();
    if (!torneoActual) return;

    auto nombres = torneoActual->getNombresEquipos();
    equiposDisponibles = nombres; // copy
}

void VentanaGrupos::actualizarListas()
{
    ui->listaGrupos->clear();
    ui->listaEquipos->clear();

    // show groups
    if (grupos.empty()) {
        ui->listaGrupos->addItem("No hay grupos creados.");
    } else {
        for (size_t i = 0; i < grupos.size(); ++i) {
            const auto &g = grupos[i];
            std::string line = "Grupo " + std::to_string(i+1) + ": ";
            for (size_t k = 0; k < g.size(); ++k) {
                if (k) line += ", ";
                line += g[k];
            }
            ui->listaGrupos->addItem(QString::fromStdString(line));
        }
    }

    // show available teams
    if (equiposDisponibles.empty()) {
        ui->listaEquipos->addItem("No hay equipos disponibles.");
    } else {
        for (const auto &n : equiposDisponibles)
            ui->listaEquipos->addItem(QString::fromStdString(n));
    }
}

void VentanaGrupos::agregarGrupo(const std::vector<std::string>& g)
{
    if (g.empty()) return;
    grupos.push_back(g);
    // remove chosen names from equiposDisponibles
    for (const auto &name : g) {
        auto it = std::find(equiposDisponibles.begin(), equiposDisponibles.end(), name);
        if (it != equiposDisponibles.end()) equiposDisponibles.erase(it);
    }
}

// Generación aleatoria: mezcla nombres y crea grupos max 4 por grupo
void VentanaGrupos::alGenerarAuto()
{
    if (!torneoActual) {
        QMessageBox::warning(this, "Error", "No hay torneo asignado desde el cual generar grupos.");
        return;
    }

    auto nombres = torneoActual->getNombresEquipos();
    if (nombres.size() < 2) {
        QMessageBox::information(this, "Sin suficientes equipos", "Se requieren al menos 2 equipos para generar grupos.");
        return;
    }

    // shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(nombres.begin(), nombres.end(), g);

    grupos.clear();
    equiposDisponibles.clear();

    const size_t maxPorGrupo = 4;
    std::vector<std::string> current;
    for (size_t i = 0; i < nombres.size(); ++i) {
        current.push_back(nombres[i]);
        if (current.size() == maxPorGrupo || i + 1 == nombres.size()) {
            grupos.push_back(current);
            current.clear();
        }
    }

    actualizarListas();
    QMessageBox::information(this, "Grupos generados", "Se han generado grupos aleatorios.");
}

// Crear grupo manual: pide nombres del grupo uno por uno desde la lista disponible
void VentanaGrupos::alCrearManual()
{
    if (!torneoActual) {
        QMessageBox::warning(this, "Error", "No hay torneo asignado.");
        return;
    }

    if (equiposDisponibles.empty()) {
        QMessageBox::information(this, "Sin equipos", "No quedan equipos disponibles para asignar.");
        return;
    }

    // Ask user for how many teams in this group (1-4)
    bool okNum = false;
    int cuantos = QInputDialog::getInt(this, "Crear grupo manual",
                                       "¿Cuántos equipos tendrá el grupo? (1-4):",
                                       2, 1, 4, 1, &okNum);
    if (!okNum) return;

    std::vector<std::string> nuevo;
    for (int i = 0; i < cuantos; ++i) {
        // build QStringList from equiposDisponibles
        QStringList items;
        for (const auto &n : equiposDisponibles) items << QString::fromStdString(n);

        bool okItem = false;
        QString elegido = QInputDialog::getItem(this,
                                                "Seleccionar equipo",
                                                QString("Selecciona equipo %1 de %2:").arg(i+1).arg(cuantos),
                                                items, 0, false, &okItem);
        if (!okItem) {
            // user cancelled selection; if partial group chosen, return selected names to available and abort
            QMessageBox::information(this, "Cancelado", "Selección cancelada. Grupo no creado.");
            return;
        }

        std::string elegidoStd = elegido.toStdString();
        nuevo.push_back(elegidoStd);

        // remove chosen from available to prevent duplicates in same or future groups
        auto it = std::find(equiposDisponibles.begin(), equiposDisponibles.end(), elegidoStd);
        if (it != equiposDisponibles.end()) equiposDisponibles.erase(it);

        if (equiposDisponibles.empty() && i + 1 < cuantos) {
            QMessageBox::warning(this, "No hay más equipos", "No quedan equipos disponibles para completar el grupo.");
            // rollback: return previously removed back to available
            for (const auto &name : nuevo) {
                equiposDisponibles.push_back(name);
            }
            return;
        }
    }

    agregarGrupo(nuevo);
    actualizarListas();
}

// Elimina el grupo seleccionado, devolviendo sus equipos a disponibles
void VentanaGrupos::alEliminar()
{
    auto item = ui->listaGrupos->currentRow();
    if (item < 0 || item >= static_cast<int>(grupos.size())) {
        QMessageBox::information(this, "Selecciona un grupo", "Selecciona un grupo para eliminar.");
        return;
    }

    if (QMessageBox::question(this, "Confirmar eliminación", "¿Eliminar el grupo seleccionado?") != QMessageBox::Yes)
        return;

    auto removed = grupos[item];
    grupos.erase(grupos.begin() + item);

    // return teams to disponibles (append)
    for (const auto &name : removed) equiposDisponibles.push_back(name);

    actualizarListas();
}

// Limpia todos los grupos locales y restaura lista de equipos desde Torneo
void VentanaGrupos::alLimpiar()
{
    if (QMessageBox::question(this, "Confirmar", "¿Eliminar todos los grupos creados localmente?") != QMessageBox::Yes)
        return;

    resetDisponiblesFromTorneo();
    actualizarListas();
}

void VentanaGrupos::alActualizar()
{
    actualizarListas();
}

// Aceptar -> close with Accepted (caller can retrieve getGrupos())
void VentanaGrupos::alGuardar()
{
    accept();
}

// Cancelar -> close with Rejected and discard local groups (caller ignores)
void VentanaGrupos::alCancelar()
{
    reject();
}

std::vector<std::vector<std::string>> VentanaGrupos::getGrupos() const
{
    return grupos;
}
