#ifndef VENTANAARBOL_H
#define VENTANAARBOL_H

#include <QDialog>

class Torneo;
namespace Ui { class VentanaArbol; }

class VentanaArbol : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaArbol(Torneo *torneo, QWidget *parent = nullptr);
    ~VentanaArbol();

private slots:
    void on_btnCerrar_clicked();

private:
    Ui::VentanaArbol *ui;
    Torneo *torneo;
    void cargarArbol();   // helper to fill the text box
};

#endif // VENTANAARBOL_H
