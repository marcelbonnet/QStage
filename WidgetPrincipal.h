#ifndef TEXTFINDER_H
#define TEXTFINDER_H

#include <QWidget>
#include <QListWidgetItem>
#include <QSpinBox>
#include <QComboBox>
#include "MidiControl.h"

namespace Ui {
class TextFinder;
}

class TextFinder : public QWidget
{
    Q_OBJECT

public:
    explicit TextFinder(MidiControl *jack, QWidget *parent = 0);
    ~TextFinder();


private slots:
    void on_listWidget_itemSelectionChanged();

    void on_perfBtnEnviar_clicked();

    void on_perfIntervaloNotas_clicked();

private:
    Ui::TextFinder *ui;
    QString configMusicasDir;
    void loadTextFile(QString file);


    QWidget *tabParts[16];    //Tab
                            //TODO Receive Switch
    QSpinBox *partMidiCh[16]; //MIDI CHANNEL

 protected:
    MidiControl *jack;

};

#endif // TEXTFINDER_H
