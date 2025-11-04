#include "ventanaequipos.h"
#include "ui_ventanaequipos.h"

#include <QInputDialog>
#include <QMessageBox>

#include "estructuras/torneo.h"
#include "modelos/equipo.h"

VentanaEquipos::VentanaEquipos(Torneo *torneo, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::VentanaEquipos),
    torneo(torneo)
{
    ui->setupUi(this);

    actualizarLista();
}

VentanaEquipos::~VentanaEquipos()
{
    delete ui;
}

void VentanaEquipos::actualizarLista()
{
    ui->listaEquipos->clear();
    if (!torneo) return;

    bool activo = torneo->estaActivo();

    ui->btnAgregar->setEnabled(!activo);
    ui->btnEditar->setEnabled(!activo);
    ui->btnEliminar->setEnabled(!activo);

    for (const std::string &nombre : torneo->getNombresEquipos()) {
        ui->listaEquipos->addItem(QString::fromStdString(nombre));
    }
}

void VentanaEquipos::on_btnActualizar_clicked()
{
    actualizarLista();
}

void VentanaEquipos::on_btnAgregar_clicked()
{
    bool ok;
    QString nombre = QInputDialog::getText(this, tr("Agregar equipo"),
                                           tr("Nombre del equipo:"), QLineEdit::Normal,
                                           "", &ok);
    if (!ok || nombre.trimmed().isEmpty()) return;

    Equipo *e = new Equipo(nombre.toStdString());
    if (!torneo->agregarEquipo(e)) {
        QMessageBox::warning(this, tr("Error"), tr("El equipo ya existe o el torneo está activo."));
        delete e;
        return;
    }

    actualizarLista();
}

void VentanaEquipos::on_btnEditar_clicked()
{
    QListWidgetItem *item = ui->listaEquipos->currentItem();
    if (!item) return;

    QString actual = item->text();
    bool ok;
    QString nuevo = QInputDialog::getText(this, tr("Editar equipo"),
                                          tr("Nuevo nombre:"), QLineEdit::Normal,
                                          actual, &ok);
    if (!ok || nuevo.trimmed().isEmpty() || nuevo == actual)
        return;

    torneo->eliminarEquipo(actual.toStdString());
    Equipo *e = new Equipo(nuevo.toStdString());
    torneo->agregarEquipo(e);

    actualizarLista();
}

void VentanaEquipos::on_btnEliminar_clicked()
{
    QListWidgetItem *item = ui->listaEquipos->currentItem();
    if (!item) return;

    QString nombre = item->text();
    if (QMessageBox::question(this, tr("Eliminar"),
                              tr("¿Eliminar el equipo \"%1\"?").arg(nombre))
        == QMessageBox::Yes)
    {
        torneo->eliminarEquipo(nombre.toStdString());
        actualizarLista();
    }
}
