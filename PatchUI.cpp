#include <QIcon>
#include <QPixmap>
#include <QDebug>
#include <QVBoxLayout>
#include <QFrame>
#include <QDebug>

#include "defaults.h"
#include "PatchUI.h"
#include "ui_PatchUI.h"
#include "Waveform.h"
#include "Controller.h"

PatchUI::PatchUI(MidiControl *jack, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatchUI)
{
    this->jack = jack;
    ui->setupUi(this);

    /*
     * Adiciona ícones aos 2 Combos de Estrutura do Chain EFX
     **/
    ui->structure12->setMinimumSize(QSize(250,60));
    ui->structure34->setMinimumSize(QSize(250,60));
    for(int i=1; i<=10; i++){
        QPixmap pixmap(QString(":/icones/struct-type%1.png").arg(i));
        QIcon icon(pixmap);
        ui->structure12->addItem(icon, QString::number(i));
        ui->structure12->setIconSize(QSize(250,60));
        ui->structure34->addItem(icon, QString::number(i));
        ui->structure34->setIconSize(QSize(250,60));
    }


    /*
     * Adiciona Aba de Patch Tone e instancia inúmeros controles
     */
    drawPatchTone();
    setupProperties();
    conectarWidgets();
}

PatchUI::~PatchUI()
{
    delete ui;
}

void PatchUI::enviarMensagem(PatchTone *patchTone, int data){
    QList<SysExMessage*> *dados = new QList<SysExMessage*>();

    dados->append(
                new SysExMessage(
                    BaseAddress(BaseAddress::PatchModeTempPatch),
                    patchTone,
                    data));

    jack->tx(dados);
}

void PatchUI::conectarWidgets()
{
    //deve ser foreach QWidget, lista
    //teste
//    for(int i=0; i<toneSwitchList->count(); i++)
//        connect(toneSwitchList->at(i),SIGNAL( clicked()), this, SLOT(onPatchToneChanged()));


    for(int i=0; i<CutoffKeyfollowList->count(); i++) connect(CutoffKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<biasDirectionList->count(); i++) connect(biasDirectionList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<biasLevelList->count(); i++) connect(biasLevelList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<biasPositionList->count(); i++) connect(biasPositionList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<controlerDestinationList->count(); i++) connect(controlerDestinationList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTimeKeyfollowList->count(); i++) connect(filterEnvelopeTimeKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeVelocityCurveList->count(); i++) connect(filterEnvelopeVelocityCurveList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeVelocityTime1List->count(); i++) connect(filterEnvelopeVelocityTime1List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeVelocityTime4List->count(); i++) connect(filterEnvelopeVelocityTime4List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterTypeList->count(); i++) connect(filterTypeList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<keyboardRangeLowerList->count(); i++) connect(keyboardRangeLowerList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<keyboardRangeUpperList->count(); i++) connect(keyboardRangeUpperList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTimeKeyfollowList->count(); i++) connect(levelEnvelopeTimeKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeVelocityCurveList->count(); i++) connect(levelEnvelopeVelocityCurveList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeVelocityTime1List->count(); i++) connect(levelEnvelopeVelocityTime1List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeVelocityTime4List->count(); i++) connect(levelEnvelopeVelocityTime4List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1ExternalSyncList->count(); i++) connect(lfo1ExternalSyncList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1FadeModeList->count(); i++) connect(lfo1FadeModeList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1OffSetList->count(); i++) connect(lfo1OffSetList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1WaveFormList->count(); i++) connect(lfo1WaveFormList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2ExternalSyncList->count(); i++) connect(lfo2ExternalSyncList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2FadeModeList->count(); i++) connect(lfo2FadeModeList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2OffSetList->count(); i++) connect(lfo2OffSetList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2WaveFormList->count(); i++) connect(lfo2WaveFormList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<outputAssignList->count(); i++) connect(outputAssignList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<panKeyfollowList->count(); i++) connect(panKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTimeKeyfollowList->count(); i++) connect(pitchEnvelopeTimeKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeVelocityTime1List->count(); i++) connect(pitchEnvelopeVelocityTime1List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeVelocityTime4List->count(); i++) connect(pitchEnvelopeVelocityTime4List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchKeyfollowList->count(); i++) connect(pitchKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<randomPitchDepthList->count(); i++) connect(randomPitchDepthList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<toneDelayModeList->count(); i++) connect(toneDelayModeList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<waveGainList->count(); i++) connect(waveGainList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<waveIdList->count(); i++) connect(waveIdList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<fxmSwitchList->count(); i++) connect(fxmSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<hold1ControlSwitchList->count(); i++) connect(hold1ControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<lfo1KeySyncSwitchList->count(); i++) connect(lfo1KeySyncSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<lfo2KeySyncSwitchList->count(); i++) connect(lfo2KeySyncSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<panControlSwitchList->count(); i++) connect(panControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<pitchBendControlSwitchList->count(); i++) connect(pitchBendControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<redamperControlSwitchList->count(); i++) connect(redamperControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<toneSwitchList->count(); i++) connect(toneSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<volumeControlSwitchList->count(); i++) connect(volumeControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<ChorusSendLevelList->count(); i++) connect(ChorusSendLevelList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<CutoffFrequencyList->count(); i++) connect(CutoffFrequencyList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<ReverbSendLevelList->count(); i++) connect(ReverbSendLevelList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<alternatePanDepthList->count(); i++) connect(alternatePanDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeDepthList->count(); i++) connect(filterEnvelopeDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeLevel1List->count(); i++) connect(filterEnvelopeLevel1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeLevel2List->count(); i++) connect(filterEnvelopeLevel2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeLevel3List->count(); i++) connect(filterEnvelopeLevel3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeLevel4List->count(); i++) connect(filterEnvelopeLevel4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTime1List->count(); i++) connect(filterEnvelopeTime1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTime2List->count(); i++) connect(filterEnvelopeTime2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTime3List->count(); i++) connect(filterEnvelopeTime3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTime4List->count(); i++) connect(filterEnvelopeTime4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeVelocitySensList->count(); i++) connect(filterEnvelopeVelocitySensList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterLfo1DepthList->count(); i++) connect(filterLfo1DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterLfo2DepthList->count(); i++) connect(filterLfo2DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeLevel1List->count(); i++) connect(levelEnvelopeLevel1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeLevel2List->count(); i++) connect(levelEnvelopeLevel2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeLevel3List->count(); i++) connect(levelEnvelopeLevel3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTime1List->count(); i++) connect(levelEnvelopeTime1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTime2List->count(); i++) connect(levelEnvelopeTime2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTime3List->count(); i++) connect(levelEnvelopeTime3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTime4List->count(); i++) connect(levelEnvelopeTime4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeVelocitySensList->count(); i++) connect(levelEnvelopeVelocitySensList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelLfo1DepthList->count(); i++) connect(levelLfo1DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelLfo2DepthList->count(); i++) connect(levelLfo2DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1DelayTimeList->count(); i++) connect(lfo1DelayTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1FadeTimeList->count(); i++) connect(lfo1FadeTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1RateList->count(); i++) connect(lfo1RateList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2DelayTimeList->count(); i++) connect(lfo2DelayTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2FadeTimeList->count(); i++) connect(lfo2FadeTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2RateList->count(); i++) connect(lfo2RateList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<mixEfxSendLevelList->count(); i++) connect(mixEfxSendLevelList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<panLfo1DepthList->count(); i++) connect(panLfo1DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<panLfo2DepthList->count(); i++) connect(panLfo2DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeLevel1List->count(); i++) connect(pitchEnvelopeLevel1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeLevel2List->count(); i++) connect(pitchEnvelopeLevel2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeLevel3List->count(); i++) connect(pitchEnvelopeLevel3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeLevel4List->count(); i++) connect(pitchEnvelopeLevel4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTime1List->count(); i++) connect(pitchEnvelopeTime1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTime2List->count(); i++) connect(pitchEnvelopeTime2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTime3List->count(); i++) connect(pitchEnvelopeTime3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTime4List->count(); i++) connect(pitchEnvelopeTime4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeVelocitySensList->count(); i++) connect(pitchEnvelopeVelocitySensList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchLfo1DepthList->count(); i++) connect(pitchLfo1DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchLfo2DepthList->count(); i++) connect(pitchLfo2DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<randomPanDepthList->count(); i++) connect(randomPanDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<resonanceList->count(); i++) connect(resonanceList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<resonanceVelocitySensList->count(); i++) connect(resonanceVelocitySensList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<toneLevelList->count(); i++) connect(toneLevelList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<tonePanList->count(); i++) connect(tonePanList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<coarseTuneList->count(); i++) connect(coarseTuneList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<controlerDepthList->count(); i++) connect(controlerDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<fineTuneList->count(); i++) connect(fineTuneList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<fxmColorList->count(); i++) connect(fxmColorList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<fxmDepthList->count(); i++) connect(fxmDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeDepthList->count(); i++) connect(pitchEnvelopeDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<toneDelayTimeList->count(); i++) connect(toneDelayTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<veocityRangeCrossFadeList->count(); i++) connect(veocityRangeCrossFadeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<veocityRangeLowerList->count(); i++) connect(veocityRangeLowerList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<veocityRangeUpperList->count(); i++) connect(veocityRangeUpperList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));


}

void PatchUI::desconectarWidgets(){
    for(int i=0; i<CutoffKeyfollowList->count(); i++) disconnect(CutoffKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<biasDirectionList->count(); i++) disconnect(biasDirectionList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<biasLevelList->count(); i++) disconnect(biasLevelList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<biasPositionList->count(); i++) disconnect(biasPositionList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<controlerDestinationList->count(); i++) disconnect(controlerDestinationList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTimeKeyfollowList->count(); i++) disconnect(filterEnvelopeTimeKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeVelocityCurveList->count(); i++) disconnect(filterEnvelopeVelocityCurveList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeVelocityTime1List->count(); i++) disconnect(filterEnvelopeVelocityTime1List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeVelocityTime4List->count(); i++) disconnect(filterEnvelopeVelocityTime4List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterTypeList->count(); i++) disconnect(filterTypeList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<keyboardRangeLowerList->count(); i++) disconnect(keyboardRangeLowerList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<keyboardRangeUpperList->count(); i++) disconnect(keyboardRangeUpperList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTimeKeyfollowList->count(); i++) disconnect(levelEnvelopeTimeKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeVelocityCurveList->count(); i++) disconnect(levelEnvelopeVelocityCurveList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeVelocityTime1List->count(); i++) disconnect(levelEnvelopeVelocityTime1List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeVelocityTime4List->count(); i++) disconnect(levelEnvelopeVelocityTime4List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1ExternalSyncList->count(); i++) disconnect(lfo1ExternalSyncList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1FadeModeList->count(); i++) disconnect(lfo1FadeModeList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1OffSetList->count(); i++) disconnect(lfo1OffSetList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1WaveFormList->count(); i++) disconnect(lfo1WaveFormList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2ExternalSyncList->count(); i++) disconnect(lfo2ExternalSyncList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2FadeModeList->count(); i++) disconnect(lfo2FadeModeList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2OffSetList->count(); i++) disconnect(lfo2OffSetList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2WaveFormList->count(); i++) disconnect(lfo2WaveFormList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<outputAssignList->count(); i++) disconnect(outputAssignList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<panKeyfollowList->count(); i++) disconnect(panKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTimeKeyfollowList->count(); i++) disconnect(pitchEnvelopeTimeKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeVelocityTime1List->count(); i++) disconnect(pitchEnvelopeVelocityTime1List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeVelocityTime4List->count(); i++) disconnect(pitchEnvelopeVelocityTime4List->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchKeyfollowList->count(); i++) disconnect(pitchKeyfollowList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<randomPitchDepthList->count(); i++) disconnect(randomPitchDepthList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<toneDelayModeList->count(); i++) disconnect(toneDelayModeList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<waveGainList->count(); i++) disconnect(waveGainList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<waveIdList->count(); i++) disconnect(waveIdList->at(i),SIGNAL( currentIndexChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<fxmSwitchList->count(); i++) disconnect(fxmSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<hold1ControlSwitchList->count(); i++) disconnect(hold1ControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<lfo1KeySyncSwitchList->count(); i++) disconnect(lfo1KeySyncSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<lfo2KeySyncSwitchList->count(); i++) disconnect(lfo2KeySyncSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<panControlSwitchList->count(); i++) disconnect(panControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<pitchBendControlSwitchList->count(); i++) disconnect(pitchBendControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<redamperControlSwitchList->count(); i++) disconnect(redamperControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<toneSwitchList->count(); i++) disconnect(toneSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<volumeControlSwitchList->count(); i++) disconnect(volumeControlSwitchList->at(i),SIGNAL( clicked() ), this, SLOT(onPatchToneChanged()));
    for(int i=0; i<ChorusSendLevelList->count(); i++) disconnect(ChorusSendLevelList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<CutoffFrequencyList->count(); i++) disconnect(CutoffFrequencyList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<ReverbSendLevelList->count(); i++) disconnect(ReverbSendLevelList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<alternatePanDepthList->count(); i++) disconnect(alternatePanDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeDepthList->count(); i++) disconnect(filterEnvelopeDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeLevel1List->count(); i++) disconnect(filterEnvelopeLevel1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeLevel2List->count(); i++) disconnect(filterEnvelopeLevel2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeLevel3List->count(); i++) disconnect(filterEnvelopeLevel3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeLevel4List->count(); i++) disconnect(filterEnvelopeLevel4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTime1List->count(); i++) disconnect(filterEnvelopeTime1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTime2List->count(); i++) disconnect(filterEnvelopeTime2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTime3List->count(); i++) disconnect(filterEnvelopeTime3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeTime4List->count(); i++) disconnect(filterEnvelopeTime4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterEnvelopeVelocitySensList->count(); i++) disconnect(filterEnvelopeVelocitySensList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterLfo1DepthList->count(); i++) disconnect(filterLfo1DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<filterLfo2DepthList->count(); i++) disconnect(filterLfo2DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeLevel1List->count(); i++) disconnect(levelEnvelopeLevel1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeLevel2List->count(); i++) disconnect(levelEnvelopeLevel2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeLevel3List->count(); i++) disconnect(levelEnvelopeLevel3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTime1List->count(); i++) disconnect(levelEnvelopeTime1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTime2List->count(); i++) disconnect(levelEnvelopeTime2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTime3List->count(); i++) disconnect(levelEnvelopeTime3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeTime4List->count(); i++) disconnect(levelEnvelopeTime4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelEnvelopeVelocitySensList->count(); i++) disconnect(levelEnvelopeVelocitySensList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelLfo1DepthList->count(); i++) disconnect(levelLfo1DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<levelLfo2DepthList->count(); i++) disconnect(levelLfo2DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1DelayTimeList->count(); i++) disconnect(lfo1DelayTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1FadeTimeList->count(); i++) disconnect(lfo1FadeTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo1RateList->count(); i++) disconnect(lfo1RateList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2DelayTimeList->count(); i++) disconnect(lfo2DelayTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2FadeTimeList->count(); i++) disconnect(lfo2FadeTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<lfo2RateList->count(); i++) disconnect(lfo2RateList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<mixEfxSendLevelList->count(); i++) disconnect(mixEfxSendLevelList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<panLfo1DepthList->count(); i++) disconnect(panLfo1DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<panLfo2DepthList->count(); i++) disconnect(panLfo2DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeLevel1List->count(); i++) disconnect(pitchEnvelopeLevel1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeLevel2List->count(); i++) disconnect(pitchEnvelopeLevel2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeLevel3List->count(); i++) disconnect(pitchEnvelopeLevel3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeLevel4List->count(); i++) disconnect(pitchEnvelopeLevel4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTime1List->count(); i++) disconnect(pitchEnvelopeTime1List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTime2List->count(); i++) disconnect(pitchEnvelopeTime2List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTime3List->count(); i++) disconnect(pitchEnvelopeTime3List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeTime4List->count(); i++) disconnect(pitchEnvelopeTime4List->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeVelocitySensList->count(); i++) disconnect(pitchEnvelopeVelocitySensList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchLfo1DepthList->count(); i++) disconnect(pitchLfo1DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchLfo2DepthList->count(); i++) disconnect(pitchLfo2DepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<randomPanDepthList->count(); i++) disconnect(randomPanDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<resonanceList->count(); i++) disconnect(resonanceList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<resonanceVelocitySensList->count(); i++) disconnect(resonanceVelocitySensList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<toneLevelList->count(); i++) disconnect(toneLevelList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<tonePanList->count(); i++) disconnect(tonePanList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<coarseTuneList->count(); i++) disconnect(coarseTuneList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<controlerDepthList->count(); i++) disconnect(controlerDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<fineTuneList->count(); i++) disconnect(fineTuneList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<fxmColorList->count(); i++) disconnect(fxmColorList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<fxmDepthList->count(); i++) disconnect(fxmDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<pitchEnvelopeDepthList->count(); i++) disconnect(pitchEnvelopeDepthList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<toneDelayTimeList->count(); i++) disconnect(toneDelayTimeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<veocityRangeCrossFadeList->count(); i++) disconnect(veocityRangeCrossFadeList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<veocityRangeLowerList->count(); i++) disconnect(veocityRangeLowerList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));
    for(int i=0; i<veocityRangeUpperList->count(); i++) disconnect(veocityRangeUpperList->at(i),SIGNAL( valueChanged(int) ), this, SLOT(onPatchToneChanged(int)));

}

void PatchUI::setupProperties(){
    /*
     * Falta:
     * - waveId
     * - controles de destination e depth
     */
    for(int i=0; i<toneSwitchList->count(); i++) toneSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Switch, i+1)));
    for(int i=0; i<waveGainList->count(); i++) waveGainList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Wave_Gain, i+1)));
    for(int i=0; i<fxmColorList->count(); i++) fxmColorList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::FXM_Color, i+1)));
    for(int i=0; i<fxmDepthList->count(); i++) fxmDepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::FXM_Depth, i+1)));
    for(int i=0; i<toneDelayModeList->count(); i++) toneDelayModeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Delay_Mode, i+1)));
    for(int i=0; i<toneDelayTimeList->count(); i++) toneDelayTimeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Delay_Time, i+1)));
    for(int i=0; i<veocityRangeCrossFadeList->count(); i++) veocityRangeCrossFadeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Velocity_Cross_Fade, i+1)));
    for(int i=0; i<veocityRangeLowerList->count(); i++) veocityRangeLowerList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Velocity_Range_Lower, i+1)));
    for(int i=0; i<veocityRangeUpperList->count(); i++) veocityRangeUpperList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Velocity_Range_Upper, i+1)));
    for(int i=0; i<keyboardRangeLowerList->count(); i++) keyboardRangeLowerList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Keyboard_Range_Lower, i+1)));
    for(int i=0; i<keyboardRangeUpperList->count(); i++) keyboardRangeUpperList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Keyboard_Range_Upper, i+1)));
    for(int i=0; i<fxmSwitchList->count(); i++) fxmSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::FXM_Switch, i+1)));
    for(int i=0; i<redamperControlSwitchList->count(); i++) redamperControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Redamper_Control_Switch, i+1)));
    for(int i=0; i<volumeControlSwitchList->count(); i++) volumeControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Volume_Control_Switch, i+1)));
    for(int i=0; i<hold1ControlSwitchList->count(); i++) hold1ControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Hold1_Control_Switch, i+1)));
    for(int i=0; i<pitchBendControlSwitchList->count(); i++) pitchBendControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Bend_Control_Switch, i+1)));
    for(int i=0; i<panControlSwitchList->count(); i++) panControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pan_Control_Switch, i+1)));
    for(int i=0; i<lfo1WaveFormList->count(); i++) lfo1WaveFormList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Waveform, i+1)));
    for(int i=0; i<lfo1KeySyncSwitchList->count(); i++) lfo1KeySyncSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Key_Sync, i+1)));
    for(int i=0; i<lfo1RateList->count(); i++) lfo1RateList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Rate, i+1)));
    for(int i=0; i<lfo1OffSetList->count(); i++) lfo1OffSetList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Offset, i+1)));
    for(int i=0; i<lfo1DelayTimeList->count(); i++) lfo1DelayTimeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Delay_Time, i+1)));
    for(int i=0; i<lfo1FadeModeList->count(); i++) lfo1FadeModeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Fade_Mode, i+1)));
    for(int i=0; i<lfo1FadeTimeList->count(); i++) lfo1FadeTimeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Fade_Time, i+1)));
    for(int i=0; i<lfo1ExternalSyncList->count(); i++) lfo1ExternalSyncList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_External_Sync, i+1)));
    for(int i=0; i<lfo2WaveFormList->count(); i++) lfo2WaveFormList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_Waveform, i+1)));
    for(int i=0; i<lfo2KeySyncSwitchList->count(); i++) lfo2KeySyncSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_Key_Sync, i+1)));
    for(int i=0; i<lfo2RateList->count(); i++) lfo2RateList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_Rate, i+1)));
    for(int i=0; i<lfo2OffSetList->count(); i++) lfo2OffSetList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_Offset, i+1)));
    for(int i=0; i<lfo2DelayTimeList->count(); i++) lfo2DelayTimeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_Delay_Time, i+1)));
    for(int i=0; i<lfo2FadeModeList->count(); i++) lfo2FadeModeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_Fade_Mode, i+1)));
    for(int i=0; i<lfo2FadeTimeList->count(); i++) lfo2FadeTimeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_Fade_Time, i+1)));
    for(int i=0; i<lfo2ExternalSyncList->count(); i++) lfo2ExternalSyncList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_External_Sync, i+1)));
    for(int i=0; i<coarseTuneList->count(); i++) coarseTuneList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Coarse_Tune, i+1)));
    for(int i=0; i<fineTuneList->count(); i++) fineTuneList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Fine_Tune, i+1)));
    for(int i=0; i<randomPitchDepthList->count(); i++) randomPitchDepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Random_Pitch_Depth, i+1)));
    for(int i=0; i<pitchKeyfollowList->count(); i++) pitchKeyfollowList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Keyfollow, i+1)));
    for(int i=0; i<pitchEnvelopeDepthList->count(); i++) pitchEnvelopeDepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Depth, i+1)));
    for(int i=0; i<pitchEnvelopeVelocitySensList->count(); i++) pitchEnvelopeVelocitySensList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Velocity_Sens, i+1)));
    for(int i=0; i<pitchEnvelopeVelocityTime1List->count(); i++) pitchEnvelopeVelocityTime1List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Velocity_Time1, i+1)));
    for(int i=0; i<pitchEnvelopeVelocityTime4List->count(); i++) pitchEnvelopeVelocityTime4List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Velocity_Time4, i+1)));
    for(int i=0; i<pitchEnvelopeTimeKeyfollowList->count(); i++) pitchEnvelopeTimeKeyfollowList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Time_Keyfollow, i+1)));
    for(int i=0; i<pitchEnvelopeTime1List->count(); i++) pitchEnvelopeTime1List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Time_1, i+1)));
    for(int i=0; i<pitchEnvelopeTime2List->count(); i++) pitchEnvelopeTime2List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Time_2, i+1)));
    for(int i=0; i<pitchEnvelopeTime3List->count(); i++) pitchEnvelopeTime3List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Time_3, i+1)));
    for(int i=0; i<pitchEnvelopeTime4List->count(); i++) pitchEnvelopeTime4List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Time_4, i+1)));
    for(int i=0; i<pitchEnvelopeLevel1List->count(); i++) pitchEnvelopeLevel1List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Level_1, i+1)));
    for(int i=0; i<pitchEnvelopeLevel2List->count(); i++) pitchEnvelopeLevel2List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Level_2, i+1)));
    for(int i=0; i<pitchEnvelopeLevel3List->count(); i++) pitchEnvelopeLevel3List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Level_3, i+1)));
    for(int i=0; i<pitchEnvelopeLevel4List->count(); i++) pitchEnvelopeLevel4List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Envelope_Level_4, i+1)));
    for(int i=0; i<pitchLfo1DepthList->count(); i++) pitchLfo1DepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_LFO1_Depth, i+1)));
    for(int i=0; i<pitchLfo2DepthList->count(); i++) pitchLfo2DepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_LFO2_Depth, i+1)));
    for(int i=0; i<filterTypeList->count(); i++) filterTypeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Type, i+1)));
    for(int i=0; i<CutoffFrequencyList->count(); i++) CutoffFrequencyList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Cutoff_Frequency, i+1)));
    for(int i=0; i<CutoffKeyfollowList->count(); i++) CutoffKeyfollowList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Cutoff_Keyfollow, i+1)));
    for(int i=0; i<resonanceList->count(); i++) resonanceList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Resonance, i+1)));
    for(int i=0; i<resonanceVelocitySensList->count(); i++) resonanceVelocitySensList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Resonance_Velocity_Sens, i+1)));
    for(int i=0; i<filterEnvelopeDepthList->count(); i++) filterEnvelopeDepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Depth, i+1)));
    for(int i=0; i<filterEnvelopeVelocityCurveList->count(); i++) filterEnvelopeVelocityCurveList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Velocity_Curve, i+1)));
    for(int i=0; i<filterEnvelopeVelocitySensList->count(); i++) filterEnvelopeVelocitySensList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Velocity_Sens, i+1)));
    for(int i=0; i<filterEnvelopeVelocityTime1List->count(); i++) filterEnvelopeVelocityTime1List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Velocity_Time1, i+1)));
    for(int i=0; i<filterEnvelopeVelocityTime4List->count(); i++) filterEnvelopeVelocityTime4List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Velocity_Time4, i+1)));
    for(int i=0; i<filterEnvelopeTimeKeyfollowList->count(); i++) filterEnvelopeTimeKeyfollowList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Time_Keyfollow, i+1)));
    for(int i=0; i<filterEnvelopeTime1List->count(); i++) filterEnvelopeTime1List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Time_1, i+1)));
    for(int i=0; i<filterEnvelopeTime2List->count(); i++) filterEnvelopeTime2List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Time_2, i+1)));
    for(int i=0; i<filterEnvelopeTime3List->count(); i++) filterEnvelopeTime3List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Time_3, i+1)));
    for(int i=0; i<filterEnvelopeTime4List->count(); i++) filterEnvelopeTime4List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Time_4, i+1)));
    for(int i=0; i<filterEnvelopeLevel1List->count(); i++) filterEnvelopeLevel1List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Level_1, i+1)));
    for(int i=0; i<filterEnvelopeLevel2List->count(); i++) filterEnvelopeLevel2List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Level_2, i+1)));
    for(int i=0; i<filterEnvelopeLevel3List->count(); i++) filterEnvelopeLevel3List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Level_3, i+1)));
    for(int i=0; i<filterEnvelopeLevel4List->count(); i++) filterEnvelopeLevel4List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_Envelope_Level_4, i+1)));
    for(int i=0; i<filterLfo1DepthList->count(); i++) filterLfo1DepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_LFO1_Depth, i+1)));
    for(int i=0; i<filterLfo2DepthList->count(); i++) filterLfo2DepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Filter_LFO2_Depth, i+1)));
    for(int i=0; i<toneLevelList->count(); i++) toneLevelList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Level, i+1)));
    for(int i=0; i<biasDirectionList->count(); i++) biasDirectionList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Bias_Direction, i+1)));
    for(int i=0; i<biasPositionList->count(); i++) biasPositionList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Bias_Position, i+1)));
    for(int i=0; i<biasLevelList->count(); i++) biasLevelList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Bias_Level, i+1)));
    for(int i=0; i<levelEnvelopeVelocityCurveList->count(); i++) levelEnvelopeVelocityCurveList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Velocity_Curve, i+1)));
    for(int i=0; i<levelEnvelopeVelocitySensList->count(); i++) levelEnvelopeVelocitySensList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Velocity_Sens, i+1)));
    for(int i=0; i<levelEnvelopeVelocityTime1List->count(); i++) levelEnvelopeVelocityTime1List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Velocity_Time1, i+1)));
    for(int i=0; i<levelEnvelopeVelocityTime4List->count(); i++) levelEnvelopeVelocityTime4List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Velocity_Time4, i+1)));
    for(int i=0; i<levelEnvelopeTimeKeyfollowList->count(); i++) levelEnvelopeTimeKeyfollowList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Time_Keyfollow, i+1)));
    for(int i=0; i<levelEnvelopeTime1List->count(); i++) levelEnvelopeTime1List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Time_1, i+1)));
    for(int i=0; i<levelEnvelopeTime2List->count(); i++) levelEnvelopeTime2List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Time_2, i+1)));
    for(int i=0; i<levelEnvelopeTime3List->count(); i++) levelEnvelopeTime3List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Time_3, i+1)));
    for(int i=0; i<levelEnvelopeTime4List->count(); i++) levelEnvelopeTime4List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Time_4, i+1)));
    for(int i=0; i<levelEnvelopeLevel1List->count(); i++) levelEnvelopeLevel1List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Level_1, i+1)));
    for(int i=0; i<levelEnvelopeLevel2List->count(); i++) levelEnvelopeLevel2List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Level_2, i+1)));
    for(int i=0; i<levelEnvelopeLevel3List->count(); i++) levelEnvelopeLevel3List->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_Envelope_Level_3, i+1)));
    for(int i=0; i<levelLfo1DepthList->count(); i++) levelLfo1DepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_LFO1_Depth, i+1)));
    for(int i=0; i<levelLfo2DepthList->count(); i++) levelLfo2DepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Level_LFO2_Depth, i+1)));
    for(int i=0; i<tonePanList->count(); i++) tonePanList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Pan, i+1)));
    for(int i=0; i<panKeyfollowList->count(); i++) panKeyfollowList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pan_Keyfollow, i+1)));
    for(int i=0; i<randomPanDepthList->count(); i++) randomPanDepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Random_Pan_Depth, i+1)));
    for(int i=0; i<alternatePanDepthList->count(); i++) alternatePanDepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Alternate_Pan_Depth, i+1)));
    for(int i=0; i<panLfo1DepthList->count(); i++) panLfo1DepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pan_LFO1_Depth, i+1)));
    for(int i=0; i<panLfo2DepthList->count(); i++) panLfo2DepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pan_LFO2_Depth, i+1)));
    for(int i=0; i<outputAssignList->count(); i++) outputAssignList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Output_Assign, i+1)));
    for(int i=0; i<mixEfxSendLevelList->count(); i++) mixEfxSendLevelList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::MixEFX_Send_Level, i+1)));
    for(int i=0; i<ChorusSendLevelList->count(); i++) ChorusSendLevelList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Chorus_Send_Level, i+1)));
    for(int i=0; i<ReverbSendLevelList->count(); i++) ReverbSendLevelList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Reverb_Send_Level, i+1)));


}

void PatchUI::onPatchToneChanged(){
    /*
    //teste:
    QObject* sender = QObject::sender();
    qDebug() << sender->metaObject()->className();
    //============================================
*/
//    QWidget *w = qobject_cast<QWidget*>(sender());
    QPushButton *w = qobject_cast<QPushButton*>(sender());
    QVariant v = w->property("function");

//    PatchTone *patchTone = qobject_cast<PatchTone*>(w->property("function"));
    PatchTone *patchTone = w->property("function").value<PatchTone*>();
    enviarMensagem( patchTone, w->isChecked()? 1 : 0 );

}
void PatchUI::onPatchToneChanged(int i){
    QObject* o = QObject::sender();
    PatchTone *patchTone = o->property("function").value<PatchTone*>();
    int offset = 0;
    int theTone = patchTone->whichTone-1;

    if(patchTone->function == PatchTone::Coarse_Tune)
        offset = 48;
    if(patchTone->function == PatchTone::Fine_Tune)
        offset = 50;
    if(patchTone->function == PatchTone::Filter_Envelope_Depth)
        offset = 24;

    /*
     * Validações
     * */
    if(patchTone->function == PatchTone::Velocity_Range_Upper
        && veocityRangeUpperList->at(theTone)->value() < veocityRangeLowerList->at(theTone)->value() ){
        veocityRangeUpperList->at(theTone)->setValue(veocityRangeLowerList->at(theTone)->value());
        return;
    }
    if(patchTone->function == PatchTone::Velocity_Range_Lower
            && veocityRangeUpperList->at(theTone)->value() < veocityRangeLowerList->at(theTone)->value() ){
        veocityRangeLowerList->at(theTone)->setValue(veocityRangeUpperList->at(theTone)->value());
        return;
    }


    if(QString::compare(o->metaObject()->className(), "QComboBox") == 0){
        QComboBox *c = qobject_cast<QComboBox*>(o);
        enviarMensagem( patchTone, c->currentIndex() );
    }
    if(QString::compare(o->metaObject()->className(), "QSpinBox") == 0){
        QSpinBox *s = qobject_cast<QSpinBox*>(o);
        enviarMensagem( patchTone, s->value() + offset );
    }
    if(QString::compare(o->metaObject()->className(), "QSlider") == 0){
        QSlider *s = qobject_cast<QSlider*>(o);
        enviarMensagem( patchTone, s->value() + offset );
    }

    /*
     * Workarounds para comportamentos inesperados do
     * Teclado depois de receber certas mensagens
     * */
    //FXM Color => Depth
    if(patchTone->function == PatchTone::FXM_Color)
        enviarMensagem( new PatchTone(PatchTone::FXM_Depth, theTone+1), fxmDepthList->at(theTone)->value() );

    //Delay Mode => Time. Vou seguir o Teclado até saber o que é melhor
    if(patchTone->function == PatchTone::Tone_Delay_Mode){
        desconectarWidgets();
        toneDelayTimeList->at(theTone)->setValue(0);
        conectarWidgets();
    }

    //Tone Delay Time zera o Velocity Cross Fade
    if(patchTone->function == PatchTone::Tone_Delay_Time){
        enviarMensagem( new PatchTone(PatchTone::Velocity_Cross_Fade, theTone+1), veocityRangeCrossFadeList->at(theTone)->value() );
    }

    //Velocity Hi zera Keyboard Range Lower
    if(patchTone->function == PatchTone::Velocity_Range_Upper){
        enviarMensagem( new PatchTone(PatchTone::Keyboard_Range_Lower, theTone+1), keyboardRangeLowerList->at(theTone)->currentIndex() );
    }

}

void PatchUI::drawPatchTone(){
    /*
     * Aba de Wave de cada Tone
     * */
    QTabWidget *tab = ui->tabWidgetTonesDetails;
    QFrame *frame = new QFrame();
    QGridLayout *grid = new QGridLayout();
    frame->setLayout(grid);

    /*
     * adiciona QLabels para os strips de widgets
     * */
    int lr=0;
    for(int i=0; i<PATCH_TONE_LABELS_COUNT; i++)
        grid->addWidget(new QLabel(PATCH_TONE_LABELS[i]), lr++, 0, 1, 1, Qt::AlignTop);

    /*
     * adiciona widgets interativos
     * */
    for(int toneid=1; toneid<5; toneid++){
        QPushButton *toneSwitch = new QPushButton(QString("%1").arg(toneid));
        toneSwitch->setCheckable(true);
//        toneSwitch->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Switch, toneid)));

        QComboBox *waveId = new QComboBox();

        QComboBox *waveGain = new QComboBox();
        waveGain->addItem("-6");        waveGain->addItem("0");
        waveGain->addItem("+6");        waveGain->addItem("+12");
        waveGain->setCurrentIndex(1);

        QPushButton *fxmSwitch = new QPushButton("FXM");
        fxmSwitch->setCheckable(true);

        QSpinBox *fxmColor = new QSpinBox();
        setSpinRange(fxmColor, 0, 3);

        QSpinBox *fxmDepth = new QSpinBox();
        setSpinRange(fxmDepth,0,15);

        QComboBox *toneDelayMode = new QComboBox();
        toneDelayMode->addItem("NORMAL");        toneDelayMode->addItem("HOLD");
        toneDelayMode->addItem("PLAYMATE");      toneDelayMode->addItem("CLOCK-SYNC");
        toneDelayMode->addItem("TAP-SYNC");      toneDelayMode->addItem("KEY-OFF-N");
        toneDelayMode->addItem("KEY-OFF-D");     toneDelayMode->addItem("TEMPO-SYNC");
        toneDelayMode->setCurrentIndex(0);

        QSpinBox *toneDelayTime = new QSpinBox();
        setSpinRange(toneDelayTime,0,127);
        toneDelayTime->setValue(0);

        QSpinBox *veocityRangeCrossFade = new QSpinBox();
        setSpinRange(veocityRangeCrossFade,0,127);

        QSpinBox *veocityRangeLower = new QSpinBox();
        setSpinRange(veocityRangeLower,1,127);

        QSpinBox *veocityRangeUpper = new QSpinBox();
        setSpinRange(veocityRangeUpper,1,127);

        QComboBox *keyboardRangeLower = new QComboBox();
        addNotas(keyboardRangeLower);
        keyboardRangeLower->setCurrentIndex(0);

        QComboBox *keyboardRangeUpper = new QComboBox();
        addNotas(keyboardRangeUpper);
        keyboardRangeUpper->setCurrentIndex(keyboardRangeUpper->count()-1);

        QPushButton *redamperControlSwitch = new QPushButton("ON");
        redamperControlSwitch->setCheckable(true);

        QPushButton *volumeControlSwitch = new QPushButton("ON");
        volumeControlSwitch->setCheckable(true);

        QPushButton *hold1ControlSwitch = new QPushButton("ON");
        hold1ControlSwitch->setCheckable(true);

        QPushButton *pitchBendControlSwitch = new QPushButton("ON");
        pitchBendControlSwitch->setCheckable(true);

        QPushButton *panControlSwitch = new QPushButton("ON");
        panControlSwitch->setCheckable(true);

        toneSwitchList->append(toneSwitch);
        waveIdList->append(waveId);
        waveGainList->append(waveGain);
        fxmSwitchList->append(fxmSwitch);
        fxmColorList->append(fxmColor);
        fxmDepthList->append(fxmDepth);
        toneDelayModeList->append(toneDelayMode);
        toneDelayTimeList->append(toneDelayTime);
        veocityRangeCrossFadeList->append(veocityRangeCrossFade);
        veocityRangeLowerList->append(veocityRangeLower);
        veocityRangeUpperList->append(veocityRangeUpper);
        keyboardRangeLowerList->append(keyboardRangeLower);
        keyboardRangeUpperList->append(keyboardRangeUpper);
        redamperControlSwitchList->append(redamperControlSwitch);
        volumeControlSwitchList->append(volumeControlSwitch);
        hold1ControlSwitchList->append(hold1ControlSwitch);
        pitchBendControlSwitchList->append(pitchBendControlSwitch);
        panControlSwitchList->append(panControlSwitch);

        int r=0;
        grid->addWidget(toneSwitch, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(waveId, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(waveGain, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(fxmSwitch, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(fxmColor, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(fxmDepth, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(toneDelayMode, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(toneDelayTime, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(veocityRangeCrossFade, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(veocityRangeLower, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(veocityRangeUpper, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(keyboardRangeLower, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(keyboardRangeUpper, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(redamperControlSwitch, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(volumeControlSwitch, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(hold1ControlSwitch, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(pitchBendControlSwitch, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(panControlSwitch, r++, toneid, 1, 1, Qt::AlignTop);

        /*
         * Controles de Destination e Depth
         * */
        for(int j=1; j<=12; j++){
            QComboBox *controlerDestination = new QComboBox();
            addPatchToneControllerDestinations(controlerDestination);
            controlerDestination->setMaximumSize(QSize(120,60));
            QSpinBox *controlerDepth = new QSpinBox();
            setSpinRange(controlerDepth,0,126);
            controlerDepth->setValue(63);

            controlerDestinationList->append(controlerDestination);
            controlerDepthList->append(controlerDepth);
            grid->addWidget(controlerDestination, r++, toneid, 1, 1, Qt::AlignTop);
            grid->addWidget(controlerDepth, r++, toneid, 1, 1, Qt::AlignTop);

            lista->append(controlerDestination);
            lista->append(controlerDepth);
        }

        /*
         * Widgets adicionados com macro do vim editor
         * */
        QComboBox *lfo1WaveForm = new QComboBox();
        lfo1WaveFormList->append(lfo1WaveForm );
        grid->addWidget(lfo1WaveForm , r++, toneid, 1,1,Qt::AlignTop);
        QPushButton *lfo1KeySyncSwitch = new QPushButton();
        lfo1KeySyncSwitchList->append(lfo1KeySyncSwitch );
        grid->addWidget(lfo1KeySyncSwitch , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *lfo1Rate = new QSlider(Qt::Horizontal);
        lfo1RateList->append(lfo1Rate );
        grid->addWidget(lfo1Rate , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *lfo1OffSet = new QComboBox();
        lfo1OffSetList->append(lfo1OffSet );
        grid->addWidget(lfo1OffSet , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *lfo1DelayTime = new QSlider(Qt::Horizontal);
        lfo1DelayTimeList->append(lfo1DelayTime );
        grid->addWidget(lfo1DelayTime , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *lfo1FadeMode = new QComboBox();
        lfo1FadeModeList->append(lfo1FadeMode );
        grid->addWidget(lfo1FadeMode , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *lfo1FadeTime = new QSlider(Qt::Horizontal);
        lfo1FadeTimeList->append(lfo1FadeTime );
        grid->addWidget(lfo1FadeTime , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *lfo1ExternalSync = new QComboBox();
        lfo1ExternalSyncList->append(lfo1ExternalSync );
        grid->addWidget(lfo1ExternalSync , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *lfo2WaveForm = new QComboBox();
        lfo2WaveFormList->append(lfo2WaveForm );
        grid->addWidget(lfo2WaveForm , r++, toneid, 1, 1, Qt::AlignTop);
        QPushButton *lfo2KeySyncSwitch = new QPushButton();
        lfo2KeySyncSwitchList->append(lfo2KeySyncSwitch );
        grid->addWidget(lfo2KeySyncSwitch , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *lfo2Rate = new QSlider(Qt::Horizontal);
        lfo2RateList->append(lfo2Rate );
        grid->addWidget(lfo2Rate , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *lfo2OffSet = new QComboBox();
        lfo2OffSetList->append(lfo2OffSet );
        grid->addWidget(lfo2OffSet , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *lfo2DelayTime = new QSlider(Qt::Horizontal);
        lfo2DelayTimeList->append(lfo2DelayTime );
        grid->addWidget(lfo2DelayTime , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *lfo2FadeMode = new QComboBox();
        lfo2FadeModeList->append(lfo2FadeMode );
        grid->addWidget(lfo2FadeMode , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *lfo2FadeTime = new QSlider(Qt::Horizontal);
        lfo2FadeTimeList->append(lfo2FadeTime );
        grid->addWidget(lfo2FadeTime , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *lfo2ExternalSync = new QComboBox();
        lfo2ExternalSyncList->append(lfo2ExternalSync );
        grid->addWidget(lfo2ExternalSync , r++, toneid, 1, 1, Qt::AlignTop);
        QSpinBox *coarseTune = new QSpinBox();
        coarseTuneList->append(coarseTune );
        grid->addWidget(coarseTune , r++, toneid, 1, 1, Qt::AlignTop);
        QSpinBox *fineTune = new QSpinBox();
        fineTuneList->append(fineTune );
        grid->addWidget(fineTune , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *randomPitchDepth = new QComboBox();
        randomPitchDepthList->append(randomPitchDepth );
        grid->addWidget(randomPitchDepth , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *pitchKeyfollow = new QComboBox();
        pitchKeyfollowList->append(pitchKeyfollow );
        grid->addWidget(pitchKeyfollow , r++, toneid, 1, 1, Qt::AlignTop);
        QSpinBox *pitchEnvelopeDepth = new QSpinBox();
        pitchEnvelopeDepthList->append(pitchEnvelopeDepth );
        grid->addWidget(pitchEnvelopeDepth , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchEnvelopeVelocitySens = new QSlider(Qt::Horizontal);
        pitchEnvelopeVelocitySensList->append(pitchEnvelopeVelocitySens );
        grid->addWidget(pitchEnvelopeVelocitySens , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *pitchEnvelopeVelocityTime1 = new QComboBox();
        pitchEnvelopeVelocityTime1List->append(pitchEnvelopeVelocityTime1 );
        grid->addWidget(pitchEnvelopeVelocityTime1 , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *pitchEnvelopeVelocityTime4 = new QComboBox();
        pitchEnvelopeVelocityTime4List->append(pitchEnvelopeVelocityTime4 );
        grid->addWidget(pitchEnvelopeVelocityTime4 , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *pitchEnvelopeTimeKeyfollow = new QComboBox();
        pitchEnvelopeTimeKeyfollowList->append(pitchEnvelopeTimeKeyfollow );
        grid->addWidget(pitchEnvelopeTimeKeyfollow , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchEnvelopeTime1 = new QSlider(Qt::Horizontal);
        pitchEnvelopeTime1List->append(pitchEnvelopeTime1 );
        grid->addWidget(pitchEnvelopeTime1 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchEnvelopeTime2 = new QSlider(Qt::Horizontal);
        pitchEnvelopeTime2List->append(pitchEnvelopeTime2 );
        grid->addWidget(pitchEnvelopeTime2 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchEnvelopeTime3 = new QSlider(Qt::Horizontal);
        pitchEnvelopeTime3List->append(pitchEnvelopeTime3 );
        grid->addWidget(pitchEnvelopeTime3 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchEnvelopeTime4 = new QSlider(Qt::Horizontal);
        pitchEnvelopeTime4List->append(pitchEnvelopeTime4 );
        grid->addWidget(pitchEnvelopeTime4 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchEnvelopeLevel1 = new QSlider(Qt::Horizontal);
        pitchEnvelopeLevel1List->append(pitchEnvelopeLevel1 );
        grid->addWidget(pitchEnvelopeLevel1 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchEnvelopeLevel2 = new QSlider(Qt::Horizontal);
        pitchEnvelopeLevel2List->append(pitchEnvelopeLevel2 );
        grid->addWidget(pitchEnvelopeLevel2 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchEnvelopeLevel3 = new QSlider(Qt::Horizontal);
        pitchEnvelopeLevel3List->append(pitchEnvelopeLevel3 );
        grid->addWidget(pitchEnvelopeLevel3 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchEnvelopeLevel4 = new QSlider(Qt::Horizontal);
        pitchEnvelopeLevel4List->append(pitchEnvelopeLevel4 );
        grid->addWidget(pitchEnvelopeLevel4 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchLfo1Depth = new QSlider(Qt::Horizontal);
        pitchLfo1DepthList->append(pitchLfo1Depth );
        grid->addWidget(pitchLfo1Depth , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *pitchLfo2Depth = new QSlider(Qt::Horizontal);
        pitchLfo2DepthList->append(pitchLfo2Depth );
        grid->addWidget(pitchLfo2Depth , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *filterType = new QComboBox();
        filterTypeList->append(filterType );
        grid->addWidget(filterType , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *CutoffFrequency = new QSlider(Qt::Horizontal);
        CutoffFrequencyList->append(CutoffFrequency );
        grid->addWidget(CutoffFrequency , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *CutoffKeyfollow = new QComboBox();
        CutoffKeyfollowList->append(CutoffKeyfollow );
        grid->addWidget(CutoffKeyfollow , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *resonance = new QSlider(Qt::Horizontal);
        resonanceList->append(resonance );
        grid->addWidget(resonance , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *resonanceVelocitySens = new QSlider(Qt::Horizontal);
        resonanceVelocitySensList->append(resonanceVelocitySens );
        grid->addWidget(resonanceVelocitySens , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeDepth = new QSlider(Qt::Horizontal);
        filterEnvelopeDepthList->append(filterEnvelopeDepth );
        grid->addWidget(filterEnvelopeDepth , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *filterEnvelopeVelocityCurve = new QComboBox();
        filterEnvelopeVelocityCurveList->append(filterEnvelopeVelocityCurve );
        grid->addWidget(filterEnvelopeVelocityCurve , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeVelocitySens = new QSlider(Qt::Horizontal);
        filterEnvelopeVelocitySensList->append(filterEnvelopeVelocitySens );
        grid->addWidget(filterEnvelopeVelocitySens , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *filterEnvelopeVelocityTime1 = new QComboBox();
        filterEnvelopeVelocityTime1List->append(filterEnvelopeVelocityTime1 );
        grid->addWidget(filterEnvelopeVelocityTime1 , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *filterEnvelopeVelocityTime4 = new QComboBox();
        filterEnvelopeVelocityTime4List->append(filterEnvelopeVelocityTime4 );
        grid->addWidget(filterEnvelopeVelocityTime4 , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *filterEnvelopeTimeKeyfollow = new QComboBox();
        filterEnvelopeTimeKeyfollowList->append(filterEnvelopeTimeKeyfollow );
        grid->addWidget(filterEnvelopeTimeKeyfollow , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeTime1 = new QSlider(Qt::Horizontal);
        filterEnvelopeTime1List->append(filterEnvelopeTime1 );
        grid->addWidget(filterEnvelopeTime1 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeTime2 = new QSlider(Qt::Horizontal);
        filterEnvelopeTime2List->append(filterEnvelopeTime2 );
        grid->addWidget(filterEnvelopeTime2 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeTime3 = new QSlider(Qt::Horizontal);
        filterEnvelopeTime3List->append(filterEnvelopeTime3 );
        grid->addWidget(filterEnvelopeTime3 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeTime4 = new QSlider(Qt::Horizontal);
        filterEnvelopeTime4List->append(filterEnvelopeTime4 );
        grid->addWidget(filterEnvelopeTime4 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeLevel1 = new QSlider(Qt::Horizontal);
        filterEnvelopeLevel1List->append(filterEnvelopeLevel1 );
        grid->addWidget(filterEnvelopeLevel1 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeLevel2 = new QSlider(Qt::Horizontal);
        filterEnvelopeLevel2List->append(filterEnvelopeLevel2 );
        grid->addWidget(filterEnvelopeLevel2 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeLevel3 = new QSlider(Qt::Horizontal);
        filterEnvelopeLevel3List->append(filterEnvelopeLevel3 );
        grid->addWidget(filterEnvelopeLevel3 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterEnvelopeLevel4 = new QSlider(Qt::Horizontal);
        filterEnvelopeLevel4List->append(filterEnvelopeLevel4 );
        grid->addWidget(filterEnvelopeLevel4 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterLfo1Depth = new QSlider(Qt::Horizontal);
        filterLfo1DepthList->append(filterLfo1Depth );
        grid->addWidget(filterLfo1Depth , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *filterLfo2Depth = new QSlider(Qt::Horizontal);
        filterLfo2DepthList->append(filterLfo2Depth );
        grid->addWidget(filterLfo2Depth , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *toneLevel = new QSlider(Qt::Horizontal);
        toneLevelList->append(toneLevel );
        grid->addWidget(toneLevel , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *biasDirection = new QComboBox();
        biasDirectionList->append(biasDirection );
        grid->addWidget(biasDirection , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *biasPosition = new QComboBox();
        biasPositionList->append(biasPosition );
        grid->addWidget(biasPosition , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *biasLevel = new QComboBox();
        biasLevelList->append(biasLevel );
        grid->addWidget(biasLevel , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *levelEnvelopeVelocityCurve = new QComboBox();
        levelEnvelopeVelocityCurveList->append(levelEnvelopeVelocityCurve );
        grid->addWidget(levelEnvelopeVelocityCurve , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelEnvelopeVelocitySens = new QSlider(Qt::Horizontal);
        levelEnvelopeVelocitySensList->append(levelEnvelopeVelocitySens );
        grid->addWidget(levelEnvelopeVelocitySens , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *levelEnvelopeVelocityTime1 = new QComboBox();
        levelEnvelopeVelocityTime1List->append(levelEnvelopeVelocityTime1 );
        grid->addWidget(levelEnvelopeVelocityTime1 , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *levelEnvelopeVelocityTime4 = new QComboBox();
        levelEnvelopeVelocityTime4List->append(levelEnvelopeVelocityTime4 );
        grid->addWidget(levelEnvelopeVelocityTime4 , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *levelEnvelopeTimeKeyfollow = new QComboBox();
        levelEnvelopeTimeKeyfollowList->append(levelEnvelopeTimeKeyfollow );
        grid->addWidget(levelEnvelopeTimeKeyfollow , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelEnvelopeTime1 = new QSlider(Qt::Horizontal);
        levelEnvelopeTime1List->append(levelEnvelopeTime1 );
        grid->addWidget(levelEnvelopeTime1 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelEnvelopeTime2 = new QSlider(Qt::Horizontal);
        levelEnvelopeTime2List->append(levelEnvelopeTime2 );
        grid->addWidget(levelEnvelopeTime2 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelEnvelopeTime3 = new QSlider(Qt::Horizontal);
        levelEnvelopeTime3List->append(levelEnvelopeTime3 );
        grid->addWidget(levelEnvelopeTime3 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelEnvelopeTime4 = new QSlider(Qt::Horizontal);
        levelEnvelopeTime4List->append(levelEnvelopeTime4 );
        grid->addWidget(levelEnvelopeTime4 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelEnvelopeLevel1 = new QSlider(Qt::Horizontal);
        levelEnvelopeLevel1List->append(levelEnvelopeLevel1 );
        grid->addWidget(levelEnvelopeLevel1 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelEnvelopeLevel2 = new QSlider(Qt::Horizontal);
        levelEnvelopeLevel2List->append(levelEnvelopeLevel2 );
        grid->addWidget(levelEnvelopeLevel2 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelEnvelopeLevel3 = new QSlider(Qt::Horizontal);
        levelEnvelopeLevel3List->append(levelEnvelopeLevel3 );
        grid->addWidget(levelEnvelopeLevel3 , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelLfo1Depth = new QSlider(Qt::Horizontal);
        levelLfo1DepthList->append(levelLfo1Depth );
        grid->addWidget(levelLfo1Depth , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *levelLfo2Depth = new QSlider(Qt::Horizontal);
        levelLfo2DepthList->append(levelLfo2Depth );
        grid->addWidget(levelLfo2Depth , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *tonePan = new QSlider(Qt::Horizontal);
        tonePanList->append(tonePan );
        grid->addWidget(tonePan , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *panKeyfollow = new QComboBox();
        panKeyfollowList->append(panKeyfollow );
        grid->addWidget(panKeyfollow , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *randomPanDepth = new QSlider(Qt::Horizontal);
        randomPanDepthList->append(randomPanDepth );
        grid->addWidget(randomPanDepth , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *alternatePanDepth = new QSlider(Qt::Horizontal);
        alternatePanDepthList->append(alternatePanDepth );
        grid->addWidget(alternatePanDepth , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *panLfo1Depth = new QSlider(Qt::Horizontal);
        panLfo1DepthList->append(panLfo1Depth );
        grid->addWidget(panLfo1Depth , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *panLfo2Depth = new QSlider(Qt::Horizontal);
        panLfo2DepthList->append(panLfo2Depth );
        grid->addWidget(panLfo2Depth , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *outputAssign = new QComboBox();
        outputAssignList->append(outputAssign );
        grid->addWidget(outputAssign , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *mixEfxSendLevel = new QSlider(Qt::Horizontal);
        mixEfxSendLevelList->append(mixEfxSendLevel );
        grid->addWidget(mixEfxSendLevel , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *ChorusSendLevel = new QSlider(Qt::Horizontal);
        ChorusSendLevelList->append(ChorusSendLevel );
        grid->addWidget(ChorusSendLevel , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *ReverbSendLevel = new QSlider(Qt::Horizontal);
        ReverbSendLevelList->append(ReverbSendLevel );
        grid->addWidget(ReverbSendLevel , r++, toneid, 1, 1, Qt::AlignTop);

        setLfoWaveform(lfo1WaveForm);
        setLfoWaveform(lfo2WaveForm);
        lfo1KeySyncSwitch->setCheckable(true);
        lfo1KeySyncSwitch->setText("ON");
        lfo2KeySyncSwitch->setCheckable(true);
        lfo2KeySyncSwitch->setText("ON");
        setSliderRange(lfo1Rate, 0, 127);
        setSliderRange(lfo2Rate, 0, 127);
        setLfoOffset(lfo1OffSet);
        setLfoOffset(lfo2OffSet);
        lfo1OffSet->setCurrentIndex(2);
        lfo2OffSet->setCurrentIndex(2);
        setSliderRange(lfo1DelayTime, 0, 127);
        setSliderRange(lfo2DelayTime, 0, 127);
        setLfoFadeMode(lfo1FadeMode);
        setLfoFadeMode(lfo2FadeMode);
        setSliderRange(lfo1FadeTime, 0, 127);
        setSliderRange(lfo2FadeTime, 0, 127);
        setExternalSync(lfo1ExternalSync);
        setExternalSync(lfo2ExternalSync);
        setSpinRange(coarseTune,-48, 48);
        coarseTune->setValue(0);
        setSpinRange(fineTune, -50, 50);
        fineTune->setValue(0);
        setRandomPitchDepth(randomPitchDepth);
        setKeyfollow15(pitchKeyfollow);
        pitchKeyfollow->setCurrentIndex(5);
        setSpinRange(pitchEnvelopeDepth,-12,12);
        pitchEnvelopeDepth->setValue(0);
        setSliderRange(pitchEnvelopeVelocitySens,0,125);
        setTime17(pitchEnvelopeVelocityTime1);
        pitchEnvelopeVelocityTime1->setCurrentIndex(7);
        setTime17(pitchEnvelopeVelocityTime4);
        pitchEnvelopeVelocityTime4->setCurrentIndex(7);
        setTime17(pitchEnvelopeTimeKeyfollow);
        pitchEnvelopeTimeKeyfollow->setCurrentIndex(7);
        setSliderRange(pitchEnvelopeTime1,0,127);
        setSliderRange(pitchEnvelopeTime2,0,127);
        setSliderRange(pitchEnvelopeTime3,0,127);
        setSliderRange(pitchEnvelopeTime4,0,127);
        setSliderRange(pitchEnvelopeLevel1,0,126);
        setSliderRange(pitchEnvelopeLevel2,0,126);
        setSliderRange(pitchEnvelopeLevel3,0,126);
        setSliderRange(pitchEnvelopeLevel4,0,126);
        pitchEnvelopeLevel1->setValue(63);
        pitchEnvelopeLevel2->setValue(63);
        pitchEnvelopeLevel3->setValue(63);
        pitchEnvelopeLevel4->setValue(63);
        setSliderRange(pitchLfo1Depth,0,126);
        setSliderRange(pitchLfo2Depth,0,126);
        pitchLfo1Depth->setValue(63);
        pitchLfo2Depth->setValue(63);
        setFilterType18(filterType);
        setSliderRange(CutoffFrequency,0,127);
        setKeyfollow15(CutoffKeyfollow);
        setSliderRange(resonance,0,127);
        setSliderRange(resonanceVelocitySens,0,125);
        setSliderRange(filterEnvelopeDepth, 0,126);
        filterEnvelopeDepth->setValue(63);
        setVelocityCurves(filterEnvelopeVelocityCurve);
        setSliderRange(filterEnvelopeVelocitySens,0,125);
        setTime17(filterEnvelopeVelocityTime1);
        setTime17(filterEnvelopeVelocityTime4);
        setTime17(filterEnvelopeTimeKeyfollow);
        setSliderRange(filterEnvelopeTime1,0,127);
        setSliderRange(filterEnvelopeTime2,0,127);
        setSliderRange(filterEnvelopeTime3,0,127);
        setSliderRange(filterEnvelopeTime4,0,127);
        setSliderRange(filterEnvelopeLevel1,0,127);
        setSliderRange(filterEnvelopeLevel2,0,127);
        setSliderRange(filterEnvelopeLevel3,0,127);
        setSliderRange(filterEnvelopeLevel4,0,127);
        setSliderRange(filterLfo1Depth,0,126);
        filterLfo1Depth->setValue(63);
        setSliderRange(filterLfo2Depth,0,126);
        filterLfo2Depth->setValue(63);
        setSliderRange(toneLevel,0,127);
        toneLevel->setValue(100);
        setPatchTone19(biasDirection);
        addNotas(biasPosition);
        setTime17(biasLevel);
        biasLevel->setCurrentIndex(7);
        setVelocityCurves(levelEnvelopeVelocityCurve);
        setSliderRange(levelEnvelopeVelocitySens,0,125);
        setTime17(levelEnvelopeVelocityTime1);
        setTime17(levelEnvelopeVelocityTime4);
        setTime17(levelEnvelopeTimeKeyfollow);
        setSliderRange(levelEnvelopeTime1,0,127);
        setSliderRange(levelEnvelopeTime2,0,127);
        setSliderRange(levelEnvelopeTime3,0,127);
        setSliderRange(levelEnvelopeTime4,0,127);
        setSliderRange(levelEnvelopeLevel1,0,127);
        setSliderRange(levelEnvelopeLevel2,0,127);
        setSliderRange(levelEnvelopeLevel3,0,127);
        setSliderRange(levelLfo1Depth,0,126);
        levelLfo1Depth->setValue(63);
        setSliderRange(levelLfo2Depth,0,126);
        levelLfo2Depth->setValue(63);
        setSliderRange(tonePan,0,127);
        tonePan->setValue(63);
        setTime17(panKeyfollow);
        panKeyfollow->setCurrentIndex(7);
        setSliderRange(randomPanDepth,0,63);
        setSliderRange(alternatePanDepth,1,127);
        alternatePanDepth->setValue(63);
        setOutputAssign(outputAssign);
        setSliderRange(mixEfxSendLevel,0,127);
        setSliderRange(ChorusSendLevel,0,127);
        setSliderRange(ReverbSendLevel,0,127);

        try {
            foreach (Waveform *w, Controller::queryWaveforms()) {
                QString nome = QString("%3 [%1 %2] %4")
                        .arg(w->waveGroup)
                        .arg(QString::number(w->number))
                        .arg(w->nome)
                        .arg((w->oneShot == 1? "*" : ""))
                        ;
                waveId->addItem(nome, QVariant::fromValue(w));
            }
            waveId->setMaximumWidth(120);
        } catch (std::exception& e)
        {
            qDebug() << "exception: " << e.what();
        }


        //adiciona todos Widgets restantes numa Lista de QWidget. Os de Controller Destination e Depth foram inseridos antes, durante sua criação em loop de 24 widgets.
        lista->append(toneSwitch); /*lista->append(waveId);*/ lista->append(waveGain); lista->append(fxmColor); lista->append(fxmDepth); lista->append(toneDelayMode); lista->append(toneDelayTime); lista->append(veocityRangeCrossFade); lista->append(veocityRangeLower); lista->append(veocityRangeUpper); lista->append(keyboardRangeLower); lista->append(keyboardRangeUpper); lista->append(fxmSwitch); lista->append(redamperControlSwitch); lista->append(volumeControlSwitch); lista->append(hold1ControlSwitch); lista->append(pitchBendControlSwitch); lista->append(panControlSwitch); lista->append(lfo1WaveForm); lista->append(lfo1KeySyncSwitch); lista->append(lfo1Rate); lista->append(lfo1OffSet); lista->append(lfo1DelayTime); lista->append(lfo1FadeMode); lista->append(lfo1FadeTime); lista->append(lfo1ExternalSync); lista->append(lfo2WaveForm); lista->append(lfo2KeySyncSwitch); lista->append(lfo2Rate); lista->append(lfo2OffSet); lista->append(lfo2DelayTime); lista->append(lfo2FadeMode); lista->append(lfo2FadeTime); lista->append(lfo2ExternalSync); lista->append(coarseTune); lista->append(fineTune); lista->append(randomPitchDepth); lista->append(pitchKeyfollow); lista->append(pitchEnvelopeDepth); lista->append(pitchEnvelopeVelocitySens); lista->append(pitchEnvelopeVelocityTime1); lista->append(pitchEnvelopeVelocityTime4); lista->append(pitchEnvelopeTimeKeyfollow); lista->append(pitchEnvelopeTime1); lista->append(pitchEnvelopeTime2); lista->append(pitchEnvelopeTime3); lista->append(pitchEnvelopeTime4); lista->append(pitchEnvelopeLevel1); lista->append(pitchEnvelopeLevel2); lista->append(pitchEnvelopeLevel3); lista->append(pitchEnvelopeLevel4); lista->append(pitchLfo1Depth); lista->append(pitchLfo2Depth); lista->append(filterType); lista->append(CutoffFrequency); lista->append(CutoffKeyfollow); lista->append(resonance); lista->append(resonanceVelocitySens); lista->append(filterEnvelopeDepth); lista->append(filterEnvelopeVelocityCurve); lista->append(filterEnvelopeVelocitySens); lista->append(filterEnvelopeVelocityTime1); lista->append(filterEnvelopeVelocityTime4); lista->append(filterEnvelopeTimeKeyfollow); lista->append(filterEnvelopeTime1); lista->append(filterEnvelopeTime2); lista->append(filterEnvelopeTime3); lista->append(filterEnvelopeTime4); lista->append(filterEnvelopeLevel1); lista->append(filterEnvelopeLevel2); lista->append(filterEnvelopeLevel3); lista->append(filterEnvelopeLevel4); lista->append(filterLfo1Depth); lista->append(filterLfo2Depth); lista->append(toneLevel); lista->append(biasDirection); lista->append(biasPosition); lista->append(biasLevel); lista->append(levelEnvelopeVelocityCurve); lista->append(levelEnvelopeVelocitySens); lista->append(levelEnvelopeVelocityTime1); lista->append(levelEnvelopeVelocityTime4); lista->append(levelEnvelopeTimeKeyfollow); lista->append(levelEnvelopeTime1); lista->append(levelEnvelopeTime2); lista->append(levelEnvelopeTime3); lista->append(levelEnvelopeTime4); lista->append(levelEnvelopeLevel1); lista->append(levelEnvelopeLevel2); lista->append(levelEnvelopeLevel3); lista->append(levelLfo1Depth); lista->append(levelLfo2Depth); lista->append(tonePan); lista->append(panKeyfollow); lista->append(randomPanDepth); lista->append(alternatePanDepth); lista->append(panLfo1Depth); lista->append(panLfo2Depth); lista->append(outputAssign); lista->append(mixEfxSendLevel); lista->append(ChorusSendLevel); lista->append(ReverbSendLevel);
    }//fim for de 1 a 4 tones

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(frame);
    tab->addTab(scrollArea, "Patch Tone");
}


