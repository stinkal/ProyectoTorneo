#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "estructuras/torneo.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Torneo torneo;

private slots:
    void onEquiposClicked();
    void onCalendarioClicked();
    void onGruposClicked();
    void onBracketClicked();
    void onTorneoClicked();
    void onReportesClicked();
};

#endif // MAINWINDOW_H
