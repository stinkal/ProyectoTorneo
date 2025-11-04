#include "ventanaequipos.h"
#include "ui_ventanaequipos.h"
#include <QMessageBox>
#include <QInputDialog>
#include <algorithm> // for std::find

VentanaEquipos::VentanaEquipos(Torneo* torneo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VentanaEquipos)
    , torneoActual(torneo)
{
    ui->setupUi(this);

    // Conectar botones a slots
    connect(ui->btnAgregar, &QPushButton::clicked, this, &VentanaEquipos::alAgregar);
    connect(ui->btnEditar, &QPushButton::clicked, this, &VentanaEquipos::alEditar);
    connect(ui->btnEliminar, &QPushButton::clicked, this, &VentanaEquipos::alEliminar);
    connect(ui->btnActualizar, &QPushButton::clicked, this, &VentanaEquipos::alActualizar);

    actualizarLista();
}

VentanaEquipos::~VentanaEquipos() {
    delete ui;
}

// Actualiza la lista de equipos mostrada en pantalla
void VentanaEquipos::actualizarLista() {
    ui->listaEquipos->clear();

    if (!torneoActual) return;

    auto nombres = torneoActual->getNombresEquipos();
    for (const auto& nombre : nombres)
        ui->listaEquipos->addItem(QString::fromStdString(nombre));
}

// Verifica si el equipo está bloqueado por estar en un torneo activo
bool VentanaEquipos::equipoBloqueado(const std::string&) {
return torneoActual->estaActivo();
}

// CRUD ------------------------------------------------------

void VentanaEquipos::alAgregar() {
    if (!torneoActual) return;

    bool ok;
    QString nombre = QInputDialog::getText(this, "Agregar equipo",
                                           "Nombre del equipo:",
                                           QLineEdit::Normal,
                                           "", &ok);

    if (ok && !nombre.isEmpty()) {
        Equipo* nuevo = new Equipo(nombre.toStdString());
        if (!torneoActual->agregarEquipo(nuevo)) {   // nombre del método depende de tu implementación
            QMessageBox::warning(this, "Error", "No se pudo agregar el equipo (nombre duplicado o torneo activo).");
            delete nuevo;
        }
        actualizarLista();
    }
}

void VentanaEquipos::alEditar() {
    auto item = ui->listaEquipos->currentItem();
    if (!item) return;

    std::string nombreViejo = item->text().toStdString();

    if (equipoBloqueado(nombreViejo)) {
        QMessageBox::warning(this, "No permitido", "No puedes editar equipos que ya participan en un torneo activo.");
        return;
    }

    bool ok;
    QString nuevoNombre = QInputDialog::getText(this, "Editar equipo",
                                                "Nuevo nombre del equipo:",
                                                QLineEdit::Normal,
                                                item->text(), &ok);

    if (ok && !nuevoNombre.isEmpty()) {
        // Ideal: llamar a un método editar en la capa de persistencia (si existe).
        // Aquí usamos eliminar + agregar para mantenerlo simple:
        torneoActual->eliminarEquipo(nombreViejo);
        torneoActual->agregarEquipo(new Equipo(nuevoNombre.toStdString()));
        actualizarLista();
    }
}

void VentanaEquipos::alEliminar() {
    auto item = ui->listaEquipos->currentItem();
    if (!item) return;

    std::string nombre = item->text().toStdString();

    if (equipoBloqueado(nombre)) {
        QMessageBox::warning(this, "No permitido", "No puedes eliminar equipos que ya están en un torneo activo.");
        return;
    }

    if (QMessageBox::question(this, "Confirmar eliminación", "¿Deseas eliminar este equipo?")
        == QMessageBox::Yes) {
        torneoActual->eliminarEquipo(nombre);
        actualizarLista();
    }
}

void VentanaEquipos::alActualizar() {
    actualizarLista();
}
