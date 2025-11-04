#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Torneo;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnEquipos_clicked();
    void on_btnGrupos_clicked();
    void on_btnCalendario_clicked();
    void on_btnTorneo_clicked();
    void on_btnBracket_clicked();
    void on_btnReportes_clicked();

private:
    Ui::MainWindow *ui;
    Torneo *torneo;
};

#endif // MAINWINDOW_H
