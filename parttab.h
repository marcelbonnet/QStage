#ifndef PARTTAB_H
#define PARTTAB_H

#include <QWidget>
#include <QString>
#include <MidiControl.h>
#include <QDateTime>
#include "patch.h"
#include "Controller.h"

namespace Ui {
class PartTab;
}

class PartTab : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief PartTab
     * @param parte de recebe de 1 a 16
     * @param jack
     * @param parent
     */
    explicit PartTab(int parte, MidiControl *jack, QWidget *parent = 0);
    ~PartTab();


    void conectarWidgets();
    void desconectarWidgets();
    void enviarPacote();

    void carregarPatches(int categoria=-1);
    void setPatchSelected(int patchId) noexcept(false);

    Patch *getPatch();
    int getPatchIndex();
    int getRegiaoMin();
    int getRegiaoMax();
    int getOitava();
    int getCanalMidi();
    int getSaida();
    int getAfinacaoBruta();
    int getAfinacaoFina();
    int getVolume();
    int getPan();
    int getMixEfxLevel();
    int getChorusLevel();
    int getReverbLevel();
    int isLocalOn();
    int getVoiceReserve();

    //void     setPatch(Patch *p);
    void     setPatch(int i);
    void     setRegiaoMin(int i);
    void     setRegiaoMax(int i);
    void     setOitava(int i);
    void     setCanalMidi(int i);
    void     setSaida(int i);
    void     setAfinacaoBruta(int i);
    void     setAfinacaoFina(int i);
    void     setVolume(int i);
    void     setPan(int i);
    void     setMixEfxLevel(int i);
    void     setChorusLevel(int i);
    void     setReverbLevel(int i);
    void     setLocalOn(int i);
    void     setVoiceReserve(int i);


private slots:
    /**
     * @brief quando há uma mudança na configuração da parte que deve ser transmitida ao XP-30
     */
    void onPartChanged();
    /**
     * @brief quando há uma mudança na configuração da parte que deve ser transmitida ao XP-30
     */
    void onPartChanged(int);
    /**
     * @brief disparado quando o patch da combo box é alterado
     * @param index
     */
    void onPatchChanged(int index);


/*
 * Botões de filtrar categoria de patches
 * */
    void on_btn_clicked();

    void on_btn_2_clicked();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

    void on_btn_8_clicked();

    void on_btn_7_clicked();

    void on_btn_9_clicked();

    void on_btn_10_clicked();

    void on_btn_11_clicked();

    void on_btn_12_clicked();

    void on_btn_13_clicked();

    void on_btn_14_clicked();

    void on_btn_15_clicked();

    void on_btn_16_clicked();

    void on_btn_17_clicked();

    void on_btn_19_clicked();

    void on_btn_18_clicked();

    void on_btn_20_clicked();

    void on_btn_21_clicked();

    void on_btn_22_clicked();

    void on_btn_23_clicked();

    void on_btn_24_clicked();

    void on_btn_25_clicked();

    void on_btn_27_clicked();

    void on_btn_26_clicked();

    void on_btn_28_clicked();

    void on_btn_29_clicked();

    void on_btn_30_clicked();

    void on_btn_31_clicked();

    void on_btn_32_clicked();

    void on_btn_33_clicked();

    void on_btn_34_clicked();

    void on_btn_35_clicked();

    void on_btn_36_clicked();

    void on_btn_37_clicked();

    void on_btn_38_clicked();

    //copia os valores desta Part para outra (aba)
    void on_partUtilDestinoBtn_clicked();

    void on_btnQStage_clicked();

    void on_btnRoland_clicked();

    void on_btnTodos_clicked();

signals:
    void partUtilsCopiarPerformancePart(int, int);

protected:
    MidiControl *jack;
    int parte;//parte que essa aba representa no perform, de 1 a 16
    QDateTime tempoUltimoEnvio;

private:
    Ui::PartTab *ui;

};

#endif // PARTTAB_H
