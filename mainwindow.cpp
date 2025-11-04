#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>

#include "estructuras/torneo.h"
#include "ventanaequipos.h"
#include "ventanagrupos.h"
#include "ventanacalendario.h"
#include "ventanatorneo.h"
#include "ventanareportes.h"
#include "ventanaarbol.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    torneo(new Torneo)
{
    ui->setupUi(this);

    // Connect the six visible buttons
   /* connect(ui->btnEquipos,    &QPushButton::clicked, this, &MainWindow::on_btnEquipos_clicked);
    connect(ui->btnGrupos,     &QPushButton::clicked, this, &MainWindow::on_btnGrupos_clicked);
    connect(ui->btnCalendario, &QPushButton::clicked, this, &MainWindow::on_btnCalendario_clicked);
    connect(ui->btnTorneo,     &QPushButton::clicked, this, &MainWindow::on_btnTorneo_clicked);
    connect(ui->btnBracket,    &QPushButton::clicked, this, &MainWindow::on_btnBracket_clicked);
    connect(ui->btnReportes,   &QPushButton::clicked, this, &MainWindow::on_btnReportes_clicked); */
}

MainWindow::~MainWindow()
{
    if (torneo) {
        torneo->guardarEstado("autosave.json");
        delete torneo;
    }
    delete ui;
}

// === Button slot implementations ===

void MainWindow::on_btnEquipos_clicked()
{
    VentanaEquipos dlg(torneo, this);
    dlg.exec();
}

void MainWindow::on_btnGrupos_clicked()
{
    VentanaGrupos dlg(torneo, this);
    dlg.exec();
}

void MainWindow::on_btnCalendario_clicked()
{
    VentanaCalendario dlg(torneo, this);
    dlg.exec();
}

void MainWindow::on_btnTorneo_clicked()
{
    VentanaTorneo dlg(torneo, this);
    dlg.exec();
}

void MainWindow::on_btnBracket_clicked()
{
    VentanaArbol dlg(torneo, this);
    dlg.exec();
}

void MainWindow::on_btnReportes_clicked()
{
    VentanaReportes dlg(torneo, this);
    dlg.exec();
}
