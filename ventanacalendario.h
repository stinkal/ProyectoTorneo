#ifndef VENTANACALENDARIO_H
#define VENTANACALENDARIO_H

#include <QDialog>
#include <QStringList>
#include <random>
#include "modelos/partido.h"
#include "estructuras/torneo.h"

namespace Ui {
class VentanaCalendario;
}

class VentanaCalendario : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaCalendario(Torneo* torneo, QWidget* parent = nullptr);
    ~VentanaCalendario();

private slots:
    void alActualizar();
    void alJugarSiguiente();
    void alAutogenerar();
    void alCerrar();

private:
    Ui::VentanaCalendario *ui;
    Torneo* torneoActual;
    std::mt19937 rng;

    void mostrarCalendario();
};

#endif // VENTANACALENDARIO_H
