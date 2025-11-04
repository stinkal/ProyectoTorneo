#include "ventanareportes.h"
#include "ui_ventanareportes.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "estructuras/torneo.h"

VentanaReportes::VentanaReportes(Torneo *torneo, QWidget *parent) :
    QDialog(parent), ui(new Ui::VentanaReportes), torneo(torneo)
{
    ui->setupUi(this);
    refresh();
}

VentanaReportes::~VentanaReportes() { delete ui; }

void VentanaReportes::refresh()
{
    QString rep = QString::fromStdString(torneo->generarReporteTexto());
    if (rep.trimmed().isEmpty())
        rep = tr("No hay datos de torneo para mostrar.");
    ui->txtResumen->setPlainText(rep);

    ui->tabReportes->clear();
    QString bracket;
    for (auto &l : torneo->obtenerBracket()) // returns vector<string>
        bracket += QString::fromStdString(l) + "\n";

    if (!bracket.trimmed().isEmpty()) {
        QWidget *tab = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(tab);
        QTextEdit *txt = new QTextEdit(tab);
        txt->setReadOnly(true);
        txt->setPlainText(bracket);
        layout->addWidget(txt);
        ui->tabReportes->addTab(tab, tr("Bracket"));
    }
}

void VentanaReportes::on_btnExport_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Guardar reporte"), "", tr("Text (*.txt)"));
    if (file.isEmpty()) return;
    QFile f(file);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo guardar."));
        return;
    }
    QTextStream out(&f);
    out << ui->txtResumen->toPlainText();
    f.close();
    QMessageBox::information(this, tr("Guardado"), tr("Reporte exportado."));
}

void VentanaReportes::on_btnCerrar_clicked() { accept(); }
