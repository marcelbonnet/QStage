#-------------------------------------------------
#
# Project created by QtCreator 2018-05-28T10:07:48
#
#-------------------------------------------------

QT       += core gui webkitwidgets webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QStage
TEMPLATE = app
CONFIG += debug_and_release

QMAKE_CXXFLAGS += `pkgconf -cflags jack`
#QMAKE_LFLAGS += `pkgconf -libs jack` -l/usr/local/lib/libjackcpp.a
QMAKE_LFLAGS += `pkgconf -libs jack` -v

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        Mainwindow.cpp \
    Efeito.cpp \
    MidiControl.cpp \
    parttab.cpp \
    patch.cpp \
    dialogjack.cpp


HEADERS  += Mainwindow.h \
    Efeito.h \
    MidiControl.h \
    parttab.h \
    patch.h \
    dialogjack.h


FORMS    += Mainwindow.ui \
    parttab.ui \
    dialogjack.ui


DISTFILES += \
    patches.ods \
    OnStage.pro.user \
    map_de_enderecos.txt \
    icones/qjackctl.png

INCLUDEPATH+= /usr/local/include

RESOURCES += \
    recursos.qrc
