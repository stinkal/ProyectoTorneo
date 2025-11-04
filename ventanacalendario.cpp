#include "ventanacalendario.h"
#include "ui_ventanacalendario.h"

#include <QMessageBox>
#include <QRandomGenerator>
#include <QFormLayout>
#include <QDialog>
#include <QSpinBox>
#include <QPushButton>

#include "estructuras/torneo.h"
#include "modelos/partido.h"
#include "modelos/equipo.h"

VentanaCalendario::VentanaCalendario(Torneo *torneo, QWidget *parent) :
    QDialog(parent), ui(new Ui::VentanaCalendario), torneo(torneo)
{
    ui->setupUi(this);
    refresh();
}

VentanaCalendario::~VentanaCalendario() { delete ui; }

void VentanaCalendario::refresh()
{
    ui->listPartidos->clear();

    if (torneo->colaVacia()) {
        ui->lblStatus->setText(tr("No hay partidos pendientes."));
        ui->btnPlayNext->setEnabled(false);
        ui->btnAutoGenerate->setEnabled(false);
        return;
    }

    ui->lblStatus->setText(tr("Partidos pendientes: %1").arg(torneo->colaTamano()));
    ui->btnPlayNext->setEnabled(true);
    ui->btnAutoGenerate->setEnabled(true);
}

void VentanaCalendario::on_btnPlayNext_clicked()
{
    Partido *p = torneo->jugarPartidoSiguiente();
    if (!p) {
        QMessageBox::information(this, tr("Sin partidos"), tr("No hay partidos en cola."));
        refresh();
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Registrar resultado"));
    QFormLayout form(&dlg);

    QLabel *lblL = new QLabel(QString::fromStdString(p->getLocal()->getNombre()));
    QLabel *lblV = new QLabel(QString::fromStdString(p->getVisitante()->getNombre()));
    QSpinBox *gl = new QSpinBox(); gl->setRange(0, 20);
    QSpinBox *gv = new QSpinBox(); gv->setRange(0, 20);
    form.addRow(lblL, gl);
    form.addRow(lblV, gv);

    QPushButton *ok = new QPushButton(tr("Registrar"));
    QPushButton *cancel = new QPushButton(tr("Cancelar"));
    QHBoxLayout *h = new QHBoxLayout();
    h->addWidget(ok); h->addWidget(cancel);
    form.addRow(h);
    connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        int golesL = gl->value();
        int golesV = gv->value();

        bool esElim = torneo->esPartidoDeEliminatoria(p);
        if (esElim && golesL == golesV) {
            QMessageBox::warning(this, tr("Error"), tr("No se permiten empates en eliminación."));
            return;
        }

        if (!torneo->registrarResultadoEnPartido(p, golesL, golesV, esElim))
            QMessageBox::warning(this, tr("Error"), tr("No se pudo registrar el resultado."));
        else
            QMessageBox::information(this, tr("OK"), tr("Resultado registrado."));
    }

    refresh();
}

void VentanaCalendario::on_btnAutoGenerate_clicked()
{
    Partido *p = torneo->jugarPartidoSiguiente();
    if (!p) { QMessageBox::information(this, tr("Fin"), tr("No hay más partidos.")); return; }

    int gl = QRandomGenerator::global()->bounded(5);
    int gv = QRandomGenerator::global()->bounded(5);

    bool esElim = torneo->esPartidoDeEliminatoria(p);
    if (esElim && gl == gv) gl++; // avoid tie

    torneo->registrarResultadoEnPartido(p, gl, gv, esElim);
    QMessageBox::information(this, tr("Autogenerado"),
                             tr("%1 %2 - %3 %4")
                                 .arg(QString::fromStdString(p->getLocal()->getNombre()))
                                 .arg(gl)
                                 .arg(QString::fromStdString(p->getVisitante()->getNombre()))
                                 .arg(gv));
    refresh();
}

void VentanaCalendario::on_btnCerrar_clicked() { accept(); }
