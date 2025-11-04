#ifndef VENTANACALENDARIO_H
#define VENTANACALENDARIO_H

#include <QDialog>

class Torneo;
class Partido;
namespace Ui { class VentanaCalendario; }

class VentanaCalendario : public QDialog
{
    Q_OBJECT
public:
    explicit VentanaCalendario(Torneo *torneo, QWidget *parent = nullptr);
    ~VentanaCalendario();
    void refresh();

private slots:
    void on_btnPlayNext_clicked();
    void on_btnAutoGenerate_clicked();
    void on_btnCerrar_clicked();

private:
    Ui::VentanaCalendario *ui;
    Torneo *torneo;

    void playMatchInteractively(Partido *p);
    void showMatchList();
};

#endif // VENTANACALENDARIO_H
