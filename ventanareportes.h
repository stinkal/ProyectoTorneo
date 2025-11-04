#ifndef VENTANAREPORTES_H
#define VENTANAREPORTES_H

#include <QDialog>

class Torneo;
namespace Ui { class VentanaReportes; }

class VentanaReportes : public QDialog
{
    Q_OBJECT
public:
    explicit VentanaReportes(Torneo *torneo, QWidget *parent = nullptr);
    ~VentanaReportes();

    void refresh();

private slots:
    void on_btnExport_clicked();
    void on_btnCerrar_clicked();

private:
    Ui::VentanaReportes *ui;
    Torneo *torneo;
};

#endif // VENTANAREPORTES_H
