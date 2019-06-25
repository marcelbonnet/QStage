#-------------------------------------------------
#
# Project created by QtCreator 2018-05-28T10:07:48
#
#-------------------------------------------------
#PRE_TARGETDEPS += /home/marcelbonnet/devel/workspace-audio/libsmf

# webkitwidgets if QT >= 5.9 - https://doc-snapshots.qt.io/qt5-5.9/qtwebenginewidgets-qtwebkitportingguide.html
QT       += core gui webkitwidgets webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QStage
TEMPLATE = app
#CONFIG += debug_and_release
CONFIG += release


QMAKE_CXXFLAGS += `pkgconf -cflags jack`
#QMAKE_CXXFLAGS += `pkgconf --cflags glib-2.0`
#QMAKE_LFLAGS += `pkgconf -libs jack` -l/usr/local/lib/libjackcpp.a
QMAKE_LFLAGS += `pkgconf -libs jack` -v
#QMAKE_LFLAGS += `pkgconf --libs "glib-2.0 >= 2.2"`
QMAKE_LFLAGS += -L/usr/local/lib -lglib-2.0 -lintl
QMAKE_LIBS += -L/home/marcelbonnet/devel/workspace-audio/libsmf -lsmf
QMAKE_LIBS += -lsqlite3 -L/home/marcelbonnet/devel/cpp/SQLiteCpp-master/build -lSQLiteCpp
QMAKE_LIBS += -linotify

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
    dialogjack.cpp \
    musica.cpp \
    kqueue.cpp \
    dialog_playlist.cpp \
    dialogmusica.cpp \
    dialogmusicaexistente.cpp \
    dialogdocumenteditor.cpp \
    Controller.cpp \
    SysExMessage.cpp \
    DataSet1.cpp \
    baseaddress.cpp \
    SystemCommon.cpp \
    PerformanceCommon.cpp \
    PerformancePart.cpp \
    PatchUI.cpp \
    MainWindow2.cpp \
    Waveform.cpp \
    PatchTone.cpp \
    WorkerSerialMidi.cpp \
    FormSerialMidi.cpp \
    QStageException.cpp \
    DialogTemaEditor.cpp \
    DataRequest.cpp \
    DataSysExType.cpp \
    WorkerSysExRequest.cpp \
    defaults.cpp


HEADERS  += Mainwindow.h \
    Efeito.h \
    MidiControl.h \
    parttab.h \
    patch.h \
    dialogjack.h \
    ijack.h \
    musica.h \
    kqueue.h \
    dialog_playlist.h \
    dialogmusica.h \
    dialogmusicaexistente.h \
    dialogdocumenteditor.h \
    Controller.h \
    SysExMessage.h \
    DataSet1.h \
    baseaddress.h \
    SystemCommon.h \
    PerformanceCommon.h \
    PerformancePart.h \
    PatchUI.h \
    defaults.h \
    MainWindow2.h \
    Waveform.h \
    PatchTone.h \
    WorkerSerialMidi.h \
    FormSerialMidi.h \
    QStageException.h \
    DialogTemaEditor.h \
    DataRequest.h \
    DataSysExType.h \
    WorkerSysExRequest.h


FORMS    += Mainwindow.ui \
    parttab.ui \
    dialogjack.ui \
    dialog_playlist.ui \
    dialogmusica.ui \
    dialogmusicaexistente.ui \
    dialogdocumenteditor.ui \
    PatchUI.ui \
    MainWindow2.ui \
    FormSerialMidi.ui \
    DialogTemaEditor.ui


DISTFILES += \
    patches.ods \
    OnStage.pro.user \
    map_de_enderecos.txt \
    icones/qjackctl.png


INCLUDEPATH += /usr/local/include
INCLUDEPATH += /home/marcelbonnet/devel/workspace-audio/libsmf
INCLUDEPATH += /usr/local/include/glib-2.0 /usr/local/lib/glib-2.0/include /usr/local/include
INCLUDEPATH += /home/marcelbonnet/devel/cpp/SQLiteCpp-master/include/



RESOURCES += recursos.qrc
