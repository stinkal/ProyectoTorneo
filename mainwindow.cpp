#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ventanaequipos.h"
#include "ventanacalendario.h"
#include "estructuras/torneo.h"
#include "ventanagrupos.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect buttons to slots
    connect(ui->btnEquipos, &QPushButton::clicked, this, &MainWindow::onEquiposClicked);
    connect(ui->btnCalendario, &QPushButton::clicked, this, &MainWindow::onCalendarioClicked);
    connect(ui->btnGrupos, &QPushButton::clicked, this, &MainWindow::onGruposClicked);
    connect(ui->btnBracket, &QPushButton::clicked, this, &MainWindow::onBracketClicked);
    connect(ui->btnTorneo, &QPushButton::clicked, this, &MainWindow::onTorneoClicked);
    connect(ui->btnReportes, &QPushButton::clicked, this, &MainWindow::onReportesClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Placeholder slots
void MainWindow::onEquiposClicked() {
    // pass the persistent MainWindow::torneo member into the dialog
    VentanaEquipos ventana(&torneo, this);
    ventana.exec();
}
void MainWindow::onCalendarioClicked()
{
    // MainWindow tiene un miembro Torneo torneo; pasamos su dirección
    VentanaCalendario ventana(&torneo, this);
    ventana.exec();
}
void MainWindow::onGruposClicked()
{
    // Crea el diálogo de gestión de grupos, pasando el torneo actual
    VentanaGrupos dlg(&torneo, this);

    // Ejecuta la ventana de forma modal (bloquea hasta que el usuario cierre)
    if (dlg.exec() == QDialog::Accepted) {
        // El usuario presionó “Aceptar”, así que obtenemos los grupos creados
        auto grupos = dlg.getGrupos();  // vector<vector<string>>

        // Por ahora, solo mostramos un resumen como prueba
        QString resumen = "Grupos creados:\n";
        int i = 1;
        for (const auto& g : grupos) {
            resumen += QString("Grupo %1: ").arg(i++);
            for (size_t j = 0; j < g.size(); ++j) {
                resumen += QString::fromStdString(g[j]);
                if (j + 1 < g.size()) resumen += ", ";
            }
            resumen += "\n";
        }

        QMessageBox::information(this, "Resumen de grupos", resumen);
    }
    else {
        // El usuario canceló, no hacemos nada
        QMessageBox::information(this, "Cancelado", "No se aplicaron cambios en los grupos.");
    }
}
void MainWindow::onBracketClicked()     { QMessageBox::information(this, "Bracket", "Bracket window coming soon!"); }
void MainWindow::onTorneoClicked()      { QMessageBox::information(this, "Torneo", "Torneo creation coming soon!"); }
void MainWindow::onReportesClicked()    { QMessageBox::information(this, "Reportes", "Report window coming soon!"); }
