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

#include "MidiControl.h"
#include "PatchTone.h"
#include "SysExMessage.h"
#include "patch.h"
#include "Efeito.h"

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

protected:
    MidiControl *jack;
private:
    Ui::PatchUI *ui;

    void enviarPacotesDS1(int tone);
    void enviarMensagem(PatchTone *patchTone, int data);
    void enviarMensagem(Patch *patch, int data);
    void conectarWidgets();
    void desconectarWidgets();
    void setupProperties();
    void enviarGrupoDeMensagens(PatchTone::Function function, int tone);


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
    void onPatchToneChanged();
    void onPatchToneChanged(int i);
    void onPatchCommonEfx(int i);
    /*
    void onChangeCutoffKeyfollowList(int data);
    void onChangebiasDirectionList(int data);
    void onChangebiasLevelList(int data);
    void onChangebiasPositionList(int data);
    void onChangecontrolerDestinationList(int data);
    void onChangefilterEnvelopeTimeKeyfollowList(int data);
    void onChangefilterEnvelopeVelocityCurveList(int data);
    void onChangefilterEnvelopeVelocityTime1List(int data);
    void onChangefilterEnvelopeVelocityTime4List(int data);
    void onChangefilterTypeList(int data);
    void onChangekeyboardRangeLowerList(int data);
    void onChangekeyboardRangeUpperList(int data);
    void onChangelevelEnvelopeTimeKeyfollowList(int data);
    void onChangelevelEnvelopeVelocityCurveList(int data);
    void onChangelevelEnvelopeVelocityTime1List(int data);
    void onChangelevelEnvelopeVelocityTime4List(int data);
    void onChangelfo1ExternalSyncList(int data);
    void onChangelfo1FadeModeList(int data);
    void onChangelfo1OffSetList(int data);
    void onChangelfo1WaveFormList(int data);
    void onChangelfo2ExternalSyncList(int data);
    void onChangelfo2FadeModeList(int data);
    void onChangelfo2OffSetList(int data);
    void onChangelfo2WaveFormList(int data);
    void onChangeoutputAssignList(int data);
    void onChangepanKeyfollowList(int data);
    void onChangepitchEnvelopeTimeKeyfollowList(int data);
    void onChangepitchEnvelopeVelocityTime1List(int data);
    void onChangepitchEnvelopeVelocityTime4List(int data);
    void onChangepitchKeyfollowList(int data);
    void onChangerandomPitchDepthList(int data);
    void onChangetoneDelayModeList(int data);
    void onChangewaveGainList(int data);
    void onChangewaveIdList(int data);
    void onChangefxmSwitchList(int data);
    void onChangehold1ControlSwitchList(int data);
    void onChangelfo1KeySyncSwitchList(int data);
    void onChangelfo2KeySyncSwitchList(int data);
    void onChangepanControlSwitchList(int data);
    void onChangepitchBendControlSwitchList(int data);
    void onChangeredamperControlSwitchList(int data);
    void onChangetoneSwitchList(int data);
    void onChangevolumeControlSwitchList(int data);
    void onChangeChorusSendLevelList(int data);
    void onChangeCutoffFrequencyList(int data);
    void onChangeReverbSendLevelList(int data);
    void onChangealternatePanDepthList(int data);
    void onChangefilterEnvelopeDepthList(int data);
    void onChangefilterEnvelopeLevel1List(int data);
    void onChangefilterEnvelopeLevel2List(int data);
    void onChangefilterEnvelopeLevel3List(int data);
    void onChangefilterEnvelopeLevel4List(int data);
    void onChangefilterEnvelopeTime1List(int data);
    void onChangefilterEnvelopeTime2List(int data);
    void onChangefilterEnvelopeTime3List(int data);
    void onChangefilterEnvelopeTime4List(int data);
    void onChangefilterEnvelopeVelocitySensList(int data);
    void onChangefilterLfo1DepthList(int data);
    void onChangefilterLfo2DepthList(int data);
    void onChangelevelEnvelopeLevel1List(int data);
    void onChangelevelEnvelopeLevel2List(int data);
    void onChangelevelEnvelopeLevel3List(int data);
    void onChangelevelEnvelopeTime1List(int data);
    void onChangelevelEnvelopeTime2List(int data);
    void onChangelevelEnvelopeTime3List(int data);
    void onChangelevelEnvelopeTime4List(int data);
    void onChangelevelEnvelopeVelocitySensList(int data);
    void onChangelevelLfo1DepthList(int data);
    void onChangelevelLfo2DepthList(int data);
    void onChangelfo1DelayTimeList(int data);
    void onChangelfo1FadeTimeList(int data);
    void onChangelfo1RateList(int data);
    void onChangelfo2DelayTimeList(int data);
    void onChangelfo2FadeTimeList(int data);
    void onChangelfo2RateList(int data);
    void onChangemixEfxSendLevelList(int data);
    void onChangepanLfo1DepthList(int data);
    void onChangepanLfo2DepthList(int data);
    void onChangepitchEnvelopeLevel1List(int data);
    void onChangepitchEnvelopeLevel2List(int data);
    void onChangepitchEnvelopeLevel3List(int data);
    void onChangepitchEnvelopeLevel4List(int data);
    void onChangepitchEnvelopeTime1List(int data);
    void onChangepitchEnvelopeTime2List(int data);
    void onChangepitchEnvelopeTime3List(int data);
    void onChangepitchEnvelopeTime4List(int data);
    void onChangepitchEnvelopeVelocitySensList(int data);
    void onChangepitchLfo1DepthList(int data);
    void onChangepitchLfo2DepthList(int data);
    void onChangerandomPanDepthList(int data);
    void onChangeresonanceList(int data);
    void onChangeresonanceVelocitySensList(int data);
    void onChangetoneLevelList(int data);
    void onChangetonePanList(int data);
    void onChangecoarseTuneList(int data);
    void onChangecontrolerDepthList(int data);
    void onChangefineTuneList(int data);
    void onChangefxmColorList(int data);
    void onChangefxmDepthList(int data);
    void onChangepitchEnvelopeDepthList(int data);
    void onChangetoneDelayTimeList(int data);
    void onChangeveocityRangeCrossFadeList(int data);
    void onChangeveocityRangeLowerList(int data);
    void onChangeveocityRangeUpperList(int data);
*/

    void on_name_editingFinished();
    void on_tempo_editingFinished();
    void on_patchCategory_currentIndexChanged(int index);
    void on_analogFeel_valueChanged(int value);
    void on_patchPan_valueChanged(int value);
    void on_patchLevel_valueChanged(int value);
    void on_btnKeyAssignMode_clicked();
    void on_btnSoloLegato_clicked();
    void on_bendMin_valueChanged(int arg1);
    void on_bendMax_valueChanged(int arg1);
    void on_btnPortamentoSwitch_clicked();
    void on_btnPortamentoMode_clicked();
    void on_portamentoTime_valueChanged(int value);
    void on_portamentoType_currentIndexChanged(int index);
    void on_portamentoStart_currentIndexChanged(int index);
    void on_efxControl_currentIndexChanged(int index);
    void on_control1_currentIndexChanged(int index);
    void on_control2_currentIndexChanged(int index);
    void on_control3_currentIndexChanged(int index);
    void on_octave_valueChanged(int arg1);
    void on_stretchTuneDepth_valueChanged(int arg1);
    void on_voicePriority_currentIndexChanged(int index);
    void on_btnVelocityRangeSwitch_clicked();
    void on_structure12_currentIndexChanged(int index);
    void on_booster12_currentIndexChanged(int index);
    void on_structure34_currentIndexChanged(int index);
    void on_booster34_currentIndexChanged(int index);
    void on_perfEfeito_currentIndexChanged(int index);
    void on_pushButton_clicked();

    void onSysExRequestFinished();
};

#endif // PATCHUI_H
