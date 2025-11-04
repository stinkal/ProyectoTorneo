#include "ventanaarbol.h"
#include "ui_ventanaarbol.h"

#include "estructuras/torneo.h"
#include <QMessageBox>

VentanaArbol::VentanaArbol(Torneo *torneo, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::VentanaArbol),
    torneo(torneo)
{
    ui->setupUi(this);
    cargarArbol();
}

VentanaArbol::~VentanaArbol()
{
    delete ui;
}

void VentanaArbol::cargarArbol()
{
    ui->txtArbol->clear();

    if (!torneo) {
        ui->txtArbol->setPlainText(tr("Error: no hay instancia de torneo."));
        return;
    }

    std::vector<std::string> bracket = torneo->obtenerBracket();

    if (bracket.empty()) {
        ui->txtArbol->setPlainText(tr("No hay un torneo activo o la fase de eliminación no ha comenzado."));
        return;
    }

    QString contenido;
    for (const std::string &linea : bracket) {
        contenido += QString::fromStdString(linea) + "\n";
    }
    ui->txtArbol->setPlainText(contenido);
}

void VentanaArbol::on_btnCerrar_clicked()
{
    accept();
}
