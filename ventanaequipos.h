#ifndef VENTANAEQUIPOS_H
#define VENTANAEQUIPOS_H

#include <QDialog>

class Torneo;
namespace Ui { class VentanaEquipos; }

class VentanaEquipos : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaEquipos(Torneo *torneo, QWidget *parent = nullptr);
    ~VentanaEquipos();

private slots:
    void on_btnActualizar_clicked();
    void on_btnAgregar_clicked();
    void on_btnEditar_clicked();
    void on_btnEliminar_clicked();

private:
    void actualizarLista();   // internal helper to repopulate list

    Ui::VentanaEquipos *ui;
    Torneo *torneo;
};

#endif // VENTANAEQUIPOS_H
