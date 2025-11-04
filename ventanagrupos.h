#ifndef VENTANAGRUPOS_H
#define VENTANAGRUPOS_H

#include <QDialog>

class Torneo;
namespace Ui { class VentanaGrupos; }

class VentanaGrupos : public QDialog
{
    Q_OBJECT
public:
    explicit VentanaGrupos(Torneo *torneo, QWidget *parent = nullptr);
    ~VentanaGrupos();

    void refresh();

private slots:
    void on_btnCerrar_clicked();

private:
    Ui::VentanaGrupos *ui;
    Torneo *torneo;
};

#endif // VENTANAGRUPOS_H
