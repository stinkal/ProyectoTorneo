#include "ventanacalendario.h"
#include "ui_ventanacalendario.h"
#include <QMessageBox>
#include <QInputDialog>
#include <chrono>
#include <sstream>

VentanaCalendario::VentanaCalendario(Torneo* torneo, QWidget* parent)
    : QDialog(parent),
    ui(new Ui::VentanaCalendario),
    torneoActual(torneo),
    rng(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()))
{
    ui->setupUi(this);

    connect(ui->btnActualizar, &QPushButton::clicked, this, &VentanaCalendario::alActualizar);
    connect(ui->btnJugarSiguiente, &QPushButton::clicked, this, &VentanaCalendario::alJugarSiguiente);
    connect(ui->btnAutogenerar, &QPushButton::clicked, this, &VentanaCalendario::alAutogenerar);
    connect(ui->btnCerrar, &QPushButton::clicked, this, &VentanaCalendario::alCerrar);

    mostrarCalendario();
}

VentanaCalendario::~VentanaCalendario()
{
    delete ui;
}

void VentanaCalendario::mostrarCalendario()
{
    ui->listaCalendario->clear();

    if (!torneoActual) {
        ui->listaCalendario->addItem("No hay torneo cargado.");
        ui->btnJugarSiguiente->setEnabled(false);
        ui->btnAutogenerar->setEnabled(false);
        return;
    }

    if (torneoActual->colaVacia()) {
        ui->listaCalendario->addItem("No hay partidos pendientes.");
        ui->btnJugarSiguiente->setEnabled(false);
        ui->btnAutogenerar->setEnabled(false);
        return;
    }

    ui->btnJugarSiguiente->setEnabled(true);
    ui->btnAutogenerar->setEnabled(true);

    // Por ahora solo mostramos información general de la cola
    ui->listaCalendario->addItem("Partidos pendientes en cola: " + QString::number(torneoActual->colaTamano()));
    ui->listaCalendario->addItem("Presiona 'Jugar siguiente' para continuar.");
}

void VentanaCalendario::alActualizar()
{
    mostrarCalendario();
}

void VentanaCalendario::alJugarSiguiente()
{
    if (!torneoActual) return;

    Partido* partido = torneoActual->jugarPartidoSiguiente();
    if (!partido) {
        QMessageBox::information(this, "Calendario", "No hay partidos pendientes.");
        mostrarCalendario();
        return;
    }

    bool ok1 = false, ok2 = false;
    int golesLocal = QInputDialog::getInt(
        this,
        QString::fromStdString(partido->getLocal()->getNombre()),
        "Goles:",
        0, 0, 20, 1, &ok1
        );
    if (!ok1) return;

    int golesVisitante = QInputDialog::getInt(
        this,
        QString::fromStdString(partido->getVisitante()->getNombre()),
        "Goles:",
        0, 0, 20, 1, &ok2
        );
    if (!ok2) return;

    bool esEliminatoria = torneoActual->esPartidoDeEliminatoria(partido);
    if (esEliminatoria && golesLocal == golesVisitante) {
        QMessageBox::warning(this, "Error", "En eliminación directa no se permiten empates.");
        return;
    }

    if (!torneoActual->registrarResultadoEnPartido(partido, golesLocal, golesVisitante, esEliminatoria)) {
        QMessageBox::warning(this, "Error", "No se pudo registrar el resultado.");
        return;
    }

    QString msg = QString::fromStdString(
        partido->getLocal()->getNombre() + " (" + std::to_string(golesLocal) + ") - " +
        partido->getVisitante()->getNombre() + " (" + std::to_string(golesVisitante) + ")"
        );
    QMessageBox::information(this, "Resultado registrado", msg);

    mostrarCalendario();
}

void VentanaCalendario::alAutogenerar()
{
    if (!torneoActual) return;

    Partido* partido = torneoActual->jugarPartidoSiguiente();
    if (!partido) {
        QMessageBox::information(this, "Calendario", "No hay partidos pendientes.");
        mostrarCalendario();
        return;
    }

    std::uniform_int_distribution<int> dist(0, 5);
    int gL = dist(rng);
    int gV = dist(rng);

    bool esEliminatoria = torneoActual->esPartidoDeEliminatoria(partido);
    if (esEliminatoria && gL == gV) {
        if (gL < 5) gL++; else gV++;
    }

    if (!torneoActual->registrarResultadoEnPartido(partido, gL, gV, esEliminatoria)) {
        QMessageBox::warning(this, "Error", "No se pudo registrar el resultado.");
        return;
    }

    QString msg = QString::fromStdString(
        partido->getLocal()->getNombre() + " (" + std::to_string(gL) + ") - " +
        partido->getVisitante()->getNombre() + " (" + std::to_string(gV) + ")"
        );
    QMessageBox::information(this, "Autogenerado", msg);

    mostrarCalendario();
}

void VentanaCalendario::alCerrar()
{
    close();
}
