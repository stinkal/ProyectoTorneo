QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    estructuras/arbol.cpp \
    estructuras/colapartidos.cpp \
    estructuras/equiposPer.cpp \
    estructuras/persistencia.cpp \
    estructuras/torneo.cpp \
    main.cpp \
    mainwindow.cpp \
    modelos/equipo.cpp \
    modelos/grupo.cpp \
    modelos/partido.cpp \
    ventanacalendario.cpp \
    ventanaequipos.cpp \
    ventanagrupos.cpp

HEADERS += \
    estructuras/arbol.h \
    estructuras/colapartidos.h \
    estructuras/equiposPer.h \
    estructuras/persistencia.h \
    estructuras/torneo.h \
    mainwindow.h \
    modelos/equipo.h \
    modelos/grupo.h \
    modelos/partido.h \
    ventanacalendario.h \
    ventanaequipos.h \
    ventanagrupos.h

FORMS += \
    mainwindow.ui \
    ventanacalendario.ui \
    ventanaequipos.ui \
    ventanagrupos.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
