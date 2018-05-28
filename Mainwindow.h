#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include "MidiControl.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_listWidget_itemSelectionChanged();

    void on_perfBtnEnviar_clicked();

    void on_perfIntervaloNotas_clicked();

    void on_actionConectar_triggered();

private:
    Ui::MainWindow *ui;
    QString configMusicasDir;
    void loadTextFile(QString file);


    QWidget *tabParts[16];    //Tab
                            //TODO Receive Switch
    QSpinBox *partMidiCh[16]; //MIDI CHANNEL

 protected:
    MidiControl *jack;


};

#endif // MAINWINDOW_H
