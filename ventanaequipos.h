#ifndef VENTANAEQUIPOS_H
#define VENTANAEQUIPOS_H

#include <QDialog>
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include "modelos/equipo.h"
#include "estructuras/torneo.h"

namespace Ui {
class VentanaEquipos;
}

class VentanaEquipos : public QDialog
{
    Q_OBJECT

public:
    // Ahora recibe un puntero al Torneo que manejará (o nullptr si aún no hay torneo)
    explicit VentanaEquipos(Torneo* torneo, QWidget *parent = nullptr);
    ~VentanaEquipos();

private slots:
    void alAgregar();
    void alEditar();
    void alEliminar();
    void alActualizar();

private:
    Ui::VentanaEquipos *ui;
    Torneo* torneoActual;

    void actualizarLista();
    bool equipoBloqueado(const std::string&);
};

#endif // VENTANAEQUIPOS_H
