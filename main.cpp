#include "Mainwindow.h"
#include <QApplication>


//#include <smf.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //smf_load("/studio/OLD/Estudo/Paul\ McCartney\ MIDIs/LondonTown.mid");



    return a.exec();
}
