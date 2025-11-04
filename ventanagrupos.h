#ifndef VENTANAGRUPOS_H
#define VENTANAGRUPOS_H

#include <QDialog>
#include <vector>
#include <string>
#include "estructuras/torneo.h"

namespace Ui {
class VentanaGrupos;
}

class VentanaGrupos : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaGrupos(Torneo* torneo, QWidget *parent = nullptr);
    ~VentanaGrupos();

    // Devuelve los grupos creados como listas de nombres (para integrarlos al Torneo externamente)
    std::vector<std::vector<std::string>> getGrupos() const;

private slots:
    void alGenerarAuto();
    void alCrearManual();
    void alEliminar();
    void alLimpiar();
    void alActualizar();
    void alGuardar();
    void alCancelar();

private:
    Ui::VentanaGrupos *ui;
    Torneo* torneoActual;

    std::vector<std::vector<std::string>> grupos;       // grupos locales: vector de listas de nombres
    std::vector<std::string> equiposDisponibles;        // nombres disponibles para asignar

    void actualizarListas();
    void resetDisponiblesFromTorneo();
    void agregarGrupo(const std::vector<std::string>& g);
};

#endif // VENTANAGRUPOS_H
