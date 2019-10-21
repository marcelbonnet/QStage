#ifndef ARPEGGIOUI_H
#define ARPEGGIOUI_H

#include <QSpinBox>
#include <QWidget>

#include "Arpeggio.h"
#include "Controller.h"
#include "MidiControl.h"


namespace Ui {
class ArpeggioUI;
}

class ArpeggioUI : public QWidget
{
    Q_OBJECT

public:
    explicit ArpeggioUI(MidiControl *jack, QSpinBox *performanceTempo,  QWidget *parent = nullptr);
    ~ArpeggioUI();

    /**
     * @brief enviarDataSet Envia o DataSet de Arpeggio para o Teclado
     */
    void enviarDataSet();

    void setPart(int i);
    void setStyle(int index);
    void setMotif(int index);
    void setBeatPattern(int index);
    void setOctave(int i);
    void setAccent(int i);
    void setShuffle(int i);
    void setVelocity(int i);
//    void setSwitch(bool b);   //não tem SYSEX para Switch on|off do Arpeggio

//    bool isSwitchOn();
    int getPart();
    int getStyle();
    int getMotif();
    int getBeatPattern();
    int getOctave();
    int getAccent();
    int getShuffle();
    int getVelocity();

signals:
    void arpeggioTempoChanged(int);

private slots:
    void on_style_currentIndexChanged(int index);
    void onArpeggioChanged(int i);  //captura qualquer mudança nos widgets
    void on_tempo_valueChanged(int arg1);


private:
    Ui::ArpeggioUI *ui;
    MidiControl *jack;
    QSpinBox *performanceTempo;

    void conectarWidgets();
    void desconectarWidgets();
    void carregarMotifs(QString styleName);
    void carregarBeatPatterns(QString styleName);

};

#endif // ARPEGGIOUI_H
