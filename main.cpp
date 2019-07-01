#include "Mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPixmap pixmap(":/icones/QStage.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Carregando", Qt::AlignLeft, Qt::yellow);
    app.processEvents();

    MainWindow window;
    window.show();
    splash.finish(&window);
    return app.exec();

}
