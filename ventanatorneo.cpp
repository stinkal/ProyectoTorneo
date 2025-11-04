#include "ventanatorneo.h"
#include "ui_ventanatorneo.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "estructuras/torneo.h"
#include "modelos/equipo.h"

VentanaTorneo::VentanaTorneo(Torneo *torneo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VentanaTorneo),
    torneo(torneo)
{
    ui->setupUi(this);
    ui->rbCompleto->setChecked(true);
    loadAllEquiposToList();
}

VentanaTorneo::~VentanaTorneo()
{
    delete ui;
}

void VentanaTorneo::loadAllEquiposToList()
{
    ui->listEquipos->clear();
    for (auto &n : torneo->getNombresEquipos())
        ui->listEquipos->addItem(QString::fromStdString(n));
}

bool VentanaTorneo::isPowerOfTwo(int n) const {
    if (n < 1) return false;
    return (n & (n - 1)) == 0;
}

void VentanaTorneo::on_btnCrear_clicked()
{
    if (torneo->estaActivo()) {
        QMessageBox::warning(this, tr("Error"), tr("Ya existe un torneo activo."));
        return;
    }

    QList<QListWidgetItem*> sel = ui->listEquipos->selectedItems();
    if (sel.empty()) {
        QMessageBox::information(this, tr("Error"), tr("Seleccione equipos."));
        return;
    }

    int count = sel.size();
    if (!isPowerOfTwo(count)) {
        QMessageBox::warning(this, tr("Error"),
                             tr("El número de equipos debe ser potencia de 2."));
        return;
    }

    bool modoGrupo = ui->rbCompleto->isChecked();
    torneo->configurarTorneo(modoGrupo);

    if (modoGrupo) {
        if (ui->cbAutoGrupos->isChecked()) torneo->generarGruposAutomatico();
        torneo->generarCalendarioGrupos();
    } else {
        // knockout-only tournament
        // create Equipo* list
        std::vector<Equipo*> eq;
        for (auto i : sel)
            eq.push_back(new Equipo(i->text().toStdString())); // duplicates acceptable if new
        torneo->generarLlavesDirectas(eq);
    }

    QMessageBox::information(this, tr("Éxito"), tr("Torneo configurado correctamente."));
    accept();
}

void VentanaTorneo::on_btnCancelar_clicked() { reject(); }

void VentanaTorneo::on_btnCargar_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Cargar torneo"), "", tr("JSON (*.json)"));
    if (file.isEmpty()) return;

    if (!torneo->cargarEstado(file.toStdString()))
        QMessageBox::warning(this, tr("Error"), tr("No se pudo cargar el torneo."));
    else {
        QMessageBox::information(this, tr("Cargado"), tr("Torneo cargado correctamente."));
        accept();
    }
}
