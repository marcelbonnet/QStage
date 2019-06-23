#ifndef PATCHUI_H
#define PATCHUI_H

#include <QWidget>
#include <QScrollArea>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include <QList>
#include <QTabWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QThread>
#include <QAction>

#include "MidiControl.h"
#include "PatchTone.h"
#include "SysExMessage.h"
#include "patch.h"
#include "Efeito.h"
#include "Waveform.h"

#include "WorkerSysExRequest.h"


namespace Ui {
class PatchUI;
}

class PatchUI : public QWidget
{
    Q_OBJECT

public:
    explicit PatchUI(MidiControl *jack, QWidget *parent = 0);
    ~PatchUI();

    const QString LABEL_APENAS_MEUS_PATCHES = "Apenas os Meus Patches";
    /**
     * @brief recebe uma QAction do menu da janela principal e executa
     * algo com base no QAction->data
     * @param action
     */
    void processaracaoDoMenu(QAction *action);

protected:
    MidiControl *jack;

private:
    Ui::PatchUI *ui;

    /**
     * @brief monta a lista de dados de Patch Common para enviar DataSet ou persistir os dados no SQLite
     * @return
     */
    QList<int>* montarListaDadosPatchCommon();
    /**
     * @brief monta a lista de dados de Patch Tone para enviar DataSet ou persistir os dados no SQLite
     * @param tone de 0 a 3
     * @return
     */
    QList<int>* montarListaDadosTone(int tone);

    /**
     * @brief enviar Pacotes DataSet1 de PatchTone
     * @param tone de 0 a 3
     */
    void enviarPacotesDS1(int tone);
    /**
     * @brief enviar Pacotes DataSet1 de PatchCommon
     */
    void enviarPacotesDS1PatchCommon();
//    void enviarMensagem(PatchTone *patchTone, int data);
//    void enviarMensagem(Patch *patch, int data);
    void conectarWidgets();
    void desconectarWidgets();
    void setupProperties();
//    void enviarGrupoDeMensagens(PatchTone::Function function, int tone);
    void carregarFiltroCategorias(QString categ);

    QString formatarNomeWaveform(Waveform *w);

    QGridLayout *grid;
    QScrollArea *scrollArea;
    QList<QWidget*> *lista = new QList<QWidget*>();

    //array para facilitar a implementação e envio de mensagens dos controles de destination e depth de cada tone
    PatchTone::Function controles[24] = {
        PatchTone::Controller_1_Destination_1,
        PatchTone::Controller_1_Depth_1,
        PatchTone::Controller_1_Destination_2,
        PatchTone::Controller_1_Depth_2,
        PatchTone::Controller_1_Destination_3,
        PatchTone::Controller_1_Depth_3,
        PatchTone::Controller_1_Destination_4,
        PatchTone::Controller_1_Depth_4,
        PatchTone::Controller_2_Destination_1,
        PatchTone::Controller_2_Depth_1,
        PatchTone::Controller_2_Destination_2,
        PatchTone::Controller_2_Depth_2,
        PatchTone::Controller_2_Destination_3,
        PatchTone::Controller_2_Depth_3,
        PatchTone::Controller_2_Destination_4,
        PatchTone::Controller_2_Depth_4,
        PatchTone::Controller_3_Destination_1,
        PatchTone::Controller_3_Depth_1,
        PatchTone::Controller_3_Destination_2,
        PatchTone::Controller_3_Depth_2,
        PatchTone::Controller_3_Destination_3,
        PatchTone::Controller_3_Depth_3,
        PatchTone::Controller_3_Destination_4,
        PatchTone::Controller_3_Depth_4
    };

    /*
     * Patch Tone
     * */
    //campos apenas de UI
    QList<QLineEdit*> *toneUIFiltroWaveform = new QList<QLineEdit*>();

   //campos do XP30
    QList<QPushButton*> *toneSwitchList = new QList<QPushButton*>();
    QList<QComboBox*> *waveIdList = new QList<QComboBox*>();
    QList<QComboBox*> *waveGainList = new QList<QComboBox*>();
    QList<QSpinBox*> *fxmColorList = new QList<QSpinBox*>();
    QList<QSpinBox*> *fxmDepthList = new QList<QSpinBox*>();
    QList<QComboBox*> *toneDelayModeList = new QList<QComboBox*>();
    QList<QSpinBox*> *toneDelayTimeList = new QList<QSpinBox*>();
    QList<QSpinBox*> *veocityRangeCrossFadeList = new QList<QSpinBox*>();
    QList<QSpinBox*> *veocityRangeLowerList = new QList<QSpinBox*>();
    QList<QSpinBox*> *veocityRangeUpperList = new QList<QSpinBox*>();
    QList<QComboBox*> *keyboardRangeLowerList = new QList<QComboBox*>();
    QList<QComboBox*> *keyboardRangeUpperList = new QList<QComboBox*>();
    QList<QPushButton*> *fxmSwitchList = new QList<QPushButton*>();
    QList<QPushButton*> *redamperControlSwitchList = new QList<QPushButton*>();
    QList<QPushButton*> *volumeControlSwitchList = new QList<QPushButton*>();
    QList<QPushButton*> *hold1ControlSwitchList = new QList<QPushButton*>();
    QList<QPushButton*> *pitchBendControlSwitchList = new QList<QPushButton*>();
    QList<QPushButton*> *panControlSwitchList = new QList<QPushButton*>();
    QList<QComboBox*> *controlerDestinationList = new QList<QComboBox*>();
    QList<QSpinBox*> *controlerDepthList = new QList<QSpinBox*>();
    QList<QComboBox*> *lfo1WaveFormList = new QList<QComboBox*>();
    QList<QPushButton*> *lfo1KeySyncSwitchList = new QList<QPushButton*>();
    QList<QSlider*> *lfo1RateList = new QList<QSlider*>();
    QList<QComboBox*> *lfo1OffSetList = new QList<QComboBox*>();
    QList<QSlider*> *lfo1DelayTimeList = new QList<QSlider*>();
    QList<QComboBox*> *lfo1FadeModeList = new QList<QComboBox*>();
    QList<QSlider*> *lfo1FadeTimeList = new QList<QSlider*>();
    QList<QComboBox*> *lfo1ExternalSyncList = new QList<QComboBox*>();
    QList<QComboBox*> *lfo2WaveFormList = new QList<QComboBox*>();
    QList<QPushButton*> *lfo2KeySyncSwitchList = new QList<QPushButton*>();
    QList<QSlider*> *lfo2RateList = new QList<QSlider*>();
    QList<QComboBox*> *lfo2OffSetList = new QList<QComboBox*>();
    QList<QSlider*> *lfo2DelayTimeList = new QList<QSlider*>();
    QList<QComboBox*> *lfo2FadeModeList = new QList<QComboBox*>();
    QList<QSlider*> *lfo2FadeTimeList = new QList<QSlider*>();
    QList<QComboBox*> *lfo2ExternalSyncList = new QList<QComboBox*>();
    QList<QSpinBox*> *coarseTuneList = new QList<QSpinBox*>();
    QList<QSpinBox*> *fineTuneList = new QList<QSpinBox*>();
    QList<QComboBox*> *randomPitchDepthList = new QList<QComboBox*>();
    QList<QComboBox*> *pitchKeyfollowList = new QList<QComboBox*>();
    QList<QSpinBox*> *pitchEnvelopeDepthList = new QList<QSpinBox*>();
    QList<QSlider*> *pitchEnvelopeVelocitySensList = new QList<QSlider*>();
    QList<QComboBox*> *pitchEnvelopeVelocityTime1List = new QList<QComboBox*>();
    QList<QComboBox*> *pitchEnvelopeVelocityTime4List = new QList<QComboBox*>();
    QList<QComboBox*> *pitchEnvelopeTimeKeyfollowList = new QList<QComboBox*>();
    QList<QSlider*> *pitchEnvelopeTime1List = new QList<QSlider*>();
    QList<QSlider*> *pitchEnvelopeTime2List = new QList<QSlider*>();
    QList<QSlider*> *pitchEnvelopeTime3List = new QList<QSlider*>();
    QList<QSlider*> *pitchEnvelopeTime4List = new QList<QSlider*>();
    QList<QSlider*> *pitchEnvelopeLevel1List = new QList<QSlider*>();
    QList<QSlider*> *pitchEnvelopeLevel2List = new QList<QSlider*>();
    QList<QSlider*> *pitchEnvelopeLevel3List = new QList<QSlider*>();
    QList<QSlider*> *pitchEnvelopeLevel4List = new QList<QSlider*>();
    QList<QSlider*> *pitchLfo1DepthList = new QList<QSlider*>();
    QList<QSlider*> *pitchLfo2DepthList = new QList<QSlider*>();
    QList<QComboBox*> *filterTypeList = new QList<QComboBox*>();
    QList<QSlider*> *CutoffFrequencyList = new QList<QSlider*>();
    QList<QComboBox*> *CutoffKeyfollowList = new QList<QComboBox*>();
    QList<QSlider*> *resonanceList = new QList<QSlider*>();
    QList<QSlider*> *resonanceVelocitySensList = new QList<QSlider*>();
    QList<QSlider*> *filterEnvelopeDepthList = new QList<QSlider*>();
    QList<QComboBox*> *filterEnvelopeVelocityCurveList = new QList<QComboBox*>();
    QList<QSlider*> *filterEnvelopeVelocitySensList = new QList<QSlider*>();
    QList<QComboBox*> *filterEnvelopeVelocityTime1List = new QList<QComboBox*>();
    QList<QComboBox*> *filterEnvelopeVelocityTime4List = new QList<QComboBox*>();
    QList<QComboBox*> *filterEnvelopeTimeKeyfollowList = new QList<QComboBox*>();
    QList<QSlider*> *filterEnvelopeTime1List = new QList<QSlider*>();
    QList<QSlider*> *filterEnvelopeTime2List = new QList<QSlider*>();
    QList<QSlider*> *filterEnvelopeTime3List = new QList<QSlider*>();
    QList<QSlider*> *filterEnvelopeTime4List = new QList<QSlider*>();
    QList<QSlider*> *filterEnvelopeLevel1List = new QList<QSlider*>();
    QList<QSlider*> *filterEnvelopeLevel2List = new QList<QSlider*>();
    QList<QSlider*> *filterEnvelopeLevel3List = new QList<QSlider*>();
    QList<QSlider*> *filterEnvelopeLevel4List = new QList<QSlider*>();
    QList<QSlider*> *filterLfo1DepthList = new QList<QSlider*>();
    QList<QSlider*> *filterLfo2DepthList = new QList<QSlider*>();
    QList<QSlider*> *toneLevelList = new QList<QSlider*>();
    QList<QComboBox*> *biasDirectionList = new QList<QComboBox*>();
    QList<QComboBox*> *biasPositionList = new QList<QComboBox*>();
    QList<QComboBox*> *biasLevelList = new QList<QComboBox*>();
    QList<QComboBox*> *levelEnvelopeVelocityCurveList = new QList<QComboBox*>();
    QList<QSlider*> *levelEnvelopeVelocitySensList = new QList<QSlider*>();
    QList<QComboBox*> *levelEnvelopeVelocityTime1List = new QList<QComboBox*>();
    QList<QComboBox*> *levelEnvelopeVelocityTime4List = new QList<QComboBox*>();
    QList<QComboBox*> *levelEnvelopeTimeKeyfollowList = new QList<QComboBox*>();
    QList<QSlider*> *levelEnvelopeTime1List = new QList<QSlider*>();
    QList<QSlider*> *levelEnvelopeTime2List = new QList<QSlider*>();
    QList<QSlider*> *levelEnvelopeTime3List = new QList<QSlider*>();
    QList<QSlider*> *levelEnvelopeTime4List = new QList<QSlider*>();
    QList<QSlider*> *levelEnvelopeLevel1List = new QList<QSlider*>();
    QList<QSlider*> *levelEnvelopeLevel2List = new QList<QSlider*>();
    QList<QSlider*> *levelEnvelopeLevel3List = new QList<QSlider*>();
    QList<QSlider*> *levelLfo1DepthList = new QList<QSlider*>();
    QList<QSlider*> *levelLfo2DepthList = new QList<QSlider*>();
    QList<QSlider*> *tonePanList = new QList<QSlider*>();
    QList<QComboBox*> *panKeyfollowList = new QList<QComboBox*>();
    QList<QSlider*> *randomPanDepthList = new QList<QSlider*>();
    QList<QSlider*> *alternatePanDepthList = new QList<QSlider*>();
    QList<QSlider*> *panLfo1DepthList = new QList<QSlider*>();
    QList<QSlider*> *panLfo2DepthList = new QList<QSlider*>();
    QList<QComboBox*> *outputAssignList = new QList<QComboBox*>();
    QList<QSlider*> *mixEfxSendLevelList = new QList<QSlider*>();
    QList<QSlider*> *ChorusSendLevelList = new QList<QSlider*>();
    QList<QSlider*> *ReverbSendLevelList = new QList<QSlider*>();

    void drawPatchTone();

private slots:
    void onFiltroWaveformTextChanged();
    void onPatchToneChanged();
    void onPatchToneChanged(int i);
    void onPatchCommonEfx(int i);
    void onPatchCommonEfx();

    void on_perfEfeito_currentIndexChanged(int index);
    void on_pushButton_clicked();


    /**
     * @brief slot chamado quando o combo de categorias é acionado para filtrar os patches por categoria
     * @param i
     */
    void onFiltrarCategoria(int i);
    /**
     * @brief quando um Patch é selecionado esse slot carrega os widgets com os dados de Patch
     * Common e Tones de 0 a 3 obtidos do banco de dados
     */
    void onPatchSelected(int);
};

#endif // PATCHUI_H
