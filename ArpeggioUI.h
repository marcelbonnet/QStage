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

private slots:
    void on_style_currentIndexChanged(int index);
    void onArpeggioChanged(int i);  //captura qualquer mudança nos widgets

private:
    Ui::ArpeggioUI *ui;
    MidiControl *jack;
    QSpinBox *performanceTempo;

    void conectarWidgets();
    void desconectarWidgets();
    void carregarMotifs(QString styleName);
    void carregarBeatPatterns(QString styleName);
    void enviarDataSet();
};

#endif // ARPEGGIOUI_H
