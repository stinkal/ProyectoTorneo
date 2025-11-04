#ifndef VENTANATORNEO_H
#define VENTANATORNEO_H

#include <QDialog>

class Torneo;
namespace Ui { class VentanaTorneo; }

class VentanaTorneo : public QDialog
{
    Q_OBJECT
public:
    explicit VentanaTorneo(Torneo *torneo, QWidget *parent = nullptr);
    ~VentanaTorneo();

private slots:
    void on_btnCrear_clicked();
    void on_btnCancelar_clicked();
    void on_btnCargar_clicked();

private:
    Ui::VentanaTorneo *ui;
    Torneo *torneo;

    bool isPowerOfTwo(int n) const;
    void loadAllEquiposToList();
};

#endif // VENTANATORNEO_H
