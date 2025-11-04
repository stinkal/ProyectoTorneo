#include "ventanagrupos.h"
#include "ui_ventanagrupos.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>

#include "estructuras/torneo.h"

VentanaGrupos::VentanaGrupos(Torneo *torneo, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::VentanaGrupos),
    torneo(torneo)
{
    ui->setupUi(this);
    refresh();
}

VentanaGrupos::~VentanaGrupos()
{
    delete ui;
}

void VentanaGrupos::refresh()
{
    ui->tabGrupos->clear();

    std::vector<std::string> resumenes = torneo->obtenerResumenGrupos();

    if (resumenes.empty()) {
        ui->lblInfo->setText(tr("No hay grupos disponibles.\nCree un torneo con fase de grupos o genere grupos automáticos."));
        return;
    }

    ui->lblInfo->setText(tr("Tablas de grupos:"));

    // each string in resumenes = one group's table text
    for (size_t i = 0; i < resumenes.size(); ++i) {
        QString titulo = tr("Grupo %1").arg(static_cast<int>(i) + 1);
        QWidget *tab = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(tab);
        QTextEdit *texto = new QTextEdit(tab);
        texto->setReadOnly(true);
        texto->setPlainText(QString::fromStdString(resumenes[i]));
        layout->addWidget(texto);
        ui->tabGrupos->addTab(tab, titulo);
    }
}

void VentanaGrupos::on_btnCerrar_clicked()
{
    accept();
}
