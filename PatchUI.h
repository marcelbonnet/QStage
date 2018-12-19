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

namespace Ui {
class PatchUI;
}

class PatchUI : public QWidget
{
    Q_OBJECT

public:
    explicit PatchUI(QWidget *parent = 0);
    ~PatchUI();

private:
    Ui::PatchUI *ui;

    QGridLayout *grid;
    QScrollArea *scrollArea;
    QList<QWidget*> *lista = new QList<QWidget*>();

    /*
     * Patch Tone
     * */
    QList<QPushButton*> *toneSwitchList = new QList<QPushButton*>();
    //TODO waves ...
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
    QList<QPushButton*> *tedamperControlSwitchList = new QList<QPushButton*>();
    QList<QPushButton*> *volumeControlSwitchList = new QList<QPushButton*>();
    QList<QPushButton*> *hold1ControlSwitchList = new QList<QPushButton*>();
    QList<QPushButton*> *pitchBendControlSwitchList = new QList<QPushButton*>();
    QList<QPushButton*> *panControlSwitchList = new QList<QPushButton*>();
    QList<QComboBox*> *controlerDestinationList = new QList<QComboBox*>();
    QList<QSpinBox*> *controlerDepthList = new QList<QSpinBox*>();
    /*
     * LFO 1
     * */
    QList<QComboBox*> *lfo1WaveFormList = new QList<QComboBox*>();
    QList<QPushButton*> *lfo1KeySyncSwitchList = new QList<QPushButton*>();
    QList<QSlider*> *lfo1RateList = new QList<QSlider*>();
    QList<QComboBox*> *lfo1OffSetList = new QList<QComboBox*>();
    QList<QSlider*> *lfo1DelayTimeList = new QList<QSlider*>();
    QList<QComboBox*> *lfo1FadeModeList = new QList<QComboBox*>();
    QList<QSlider*> *lfo1FadeTimeList = new QList<QSlider*>();
    QList<QComboBox*> *lfo1ExternalSyncList = new QList<QComboBox*>();
    /*
     * LFO 2
     * */
    QList<QComboBox*> *lfo2WaveFormList = new QList<QComboBox*>();
    QList<QPushButton*> *lfo2KeySyncSwitchList = new QList<QPushButton*>();
    QList<QSlider*> *lfo2RateList = new QList<QSlider*>();
    QList<QComboBox*> *lfo2OffSetList = new QList<QComboBox*>();
    QList<QSlider*> *lfo2DelayTimeList = new QList<QSlider*>();
    QList<QComboBox*> *lfo2FadeModeList = new QList<QComboBox*>();
    QList<QSlider*> *lfo2FadeTimeList = new QList<QSlider*>();
    QList<QComboBox*> *lfo2ExternalSyncList = new QList<QComboBox*>();
    /*
     * Pitch
     * */
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
    /*
     * Filter
     * */
    QList<QComboBox*> *filterTypeList = new QList<QComboBox*>();
    QList<QSlider*> *CutoffFrequencyList = new QList<QSlider*>();
    QList<QComboBox*> *CutoffKeyfollowList = new QList<QComboBox*>();
    QList<QSlider*> *ReasonanceList = new QList<QSlider*>();
    QList<QSlider*> *ReasonanceVelocitySensList = new QList<QSlider*>();
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
    /*
     * Level/Amplitude (TVA)
     */
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
    QList<QSlider*> *alternatePanDepthList = new QList<QSlider*>();
    QList<QSlider*> *panLfo1DepthList = new QList<QSlider*>();
    QList<QSlider*> *panLfo2DepthList = new QList<QSlider*>();
    /*
     * mix
     * */
    QList<QComboBox*> *outputAssignList = new QList<QComboBox*>();
    QList<QSlider*> *mixEfxSendLevelList = new QList<QSlider*>();
    QList<QSlider*> *ChorusSendLevelList = new QList<QSlider*>();
    QList<QSlider*> *ReverbSendLevelList = new QList<QSlider*>();


};

#endif // PATCHUI_H
