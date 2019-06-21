#include <QIcon>
#include <QPixmap>
#include <QDebug>
#include <QVBoxLayout>
#include <QFrame>
#include <QDebug>
#include <QMessageBox>
#include <QChar>

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

    //Categorias
    ui->filtroCategoria->addItem("Todos os patches");//exibirá todos os patches se selecionado
    for(int i=0; i<=38; i++){
        ui->patchCategory->addItem(categorias[i]);
        ui->filtroCategoria->addItem(categorias[i]);
    }

    //combo de Efeitos
    for(int efx=0; efx<40; efx++ ){
        QString nome = QString("%1 " + Efeito::nomes[efx]).arg(efx+1);
        ui->perfEfeito->addItem(nome, QVariant::fromValue(0));
    }

    //combos de patch common control source 2 e 3
    QString origens[16] = {"OFF","SYS-CTRL1","SYS-CTRL2","MODULATION","BREATH","FOOT","VOLUME","PAN","EXPRESSION","PITCH BEND","AFTERTOUCH","LFO1","LFO2","VELOCITY","KEYFOLLOW","PLAYMATE"};
    for(int i=0; i<16; i++){
        ui->patchControlSource2->addItem(origens[i]);
        ui->patchControlSource3->addItem(origens[i]);
    }
    /*
     * Adiciona Aba de Patch Tone e instancia inúmeros controles
     */
    carregarFiltroCategorias(NULL);
    drawPatchTone();
    setupProperties();
    conectarWidgets();


}

PatchUI::~PatchUI()
{
    delete ui;
}


void PatchUI::onPatchSelected(int i){
    QList<QString>* patch;
    try {
        Patch* patchSelecionado = ui->patch->currentData().value<Patch*>();
        patch = Controller::getPatch( patchSelecionado->id );
    } catch (SQLite::Exception &e) {
        QMessageBox::critical(this,"Erro ao obter o patch do banco de dados", e.what());
    }

    if(patch == nullptr) {
        QMessageBox::critical(this,"Erro ao obter o patch do banco de dados", "Deu um pau loko. Parei tudo.");
        return;
    }

    //tem que enviar tudo numa porrada só
    desconectarWidgets();


    ui->name->setText(patch->at(5));
    //Patch Common
    QStringList common = patch->at(0).split(" ");
    ui->perfEfeito->setCurrentIndex(common.at(12).toInt());
    ui->perfParam1->setValue(common.at(13).toInt());
    ui->perfParam2->setValue(common.at(14).toInt());
    ui->perfParam3->setValue(common.at(15).toInt());
    ui->perfParam4->setValue(common.at(16).toInt());
    ui->perfParam5->setValue(common.at(17).toInt());
    ui->perfParam6->setValue(common.at(18).toInt());
    ui->perfParam7->setValue(common.at(19).toInt());
    ui->perfParam8->setValue(common.at(20).toInt());
    ui->perfParam9->setValue(common.at(21).toInt());
    ui->perfParam10->setValue(common.at(22).toInt());
    ui->perfParam11->setValue(common.at(23).toInt());
    ui->perfParam12->setValue(common.at(24).toInt());
    ui->perfOA->setCurrentIndex(common.at(25).toInt());
    ui->mixOutSendLevel->setValue(common.at(26).toInt());
    ui->chorusSendLevel->setValue(common.at(27).toInt());
    ui->reverbSendLevel->setValue(common.at(28).toInt());
    ui->perfCtrlSrc1->setCurrentIndex(common.at(29).toInt());
    ui->perfCtrlDepth1->setValue(common.at(30).toInt());
    ui->perfCtrlSrc2->setCurrentIndex(common.at(31).toInt());
    ui->perfCtrlDepth2->setValue(common.at(32).toInt());

    ui->perfChorusLevel_3->setValue(common.at(33).toInt());
    ui->perfChorusRate->setValue(common.at(34).toInt());
    ui->perfChorusDepth->setValue(common.at(35).toInt());
    ui->perfChorusPreDelay_3->setValue(common.at(36).toInt());
    ui->perfChorusFeedback->setValue(common.at(37).toInt());
    ui->perfChorusOut->setCurrentIndex(common.at(38).toInt());

    ui->perfReverbType->setCurrentIndex(common.at(39).toInt());
    ui->perfReverbLevel->setValue(common.at(40).toInt());
    ui->perfReverbTime->setValue(common.at(41).toInt());
    ui->perfReverbHFDamp->setCurrentIndex(common.at(42).toInt());
    ui->perfReverbDelayFeedback->setValue(common.at(43).toInt());

    QString tempoString = QString("%1%2").arg(common.at(44)).arg(common.at(45));
    //tempoString = QString::number(tempoString,16);
    bool intStatus;
    int tempo = tempoString.toInt(&intStatus, 16);
    if(!intStatus) QMessageBox::critical(this, "Conversão de QString para int", "Deu pau na conversão");
    ui->tempo->setValue(tempo);
    ui->patchLevel  ->setValue(common.at(46).toInt());
    ui->patchPan    ->setValue(common.at(47).toInt());
    ui->analogFeel  ->setValue(common.at(48).toInt());
    ui->bendMax     ->setValue(common.at(49).toInt());
    ui->bendMin     ->setValue(common.at(50).toInt()*-1);
    ui->btnKeyAssignMode    ->setChecked(common.at(51).toInt() == 1);
    ui->btnSoloLegato       ->setChecked(common.at(52).toInt() == 1);
    ui->btnPortamentoSwitch ->setChecked(common.at(53).toInt() == 1);
    ui->btnPortamentoMode   ->setChecked(common.at(54).toInt() == 1);
    ui->portamentoType      ->setCurrentIndex(common.at(55).toInt());
    ui->portamentoStart     ->setCurrentIndex(common.at(56).toInt());
    ui->portamentoTime              ->setValue(common.at(57).toInt());

    ui->patchControlSource2     ->setCurrentIndex(common.at(58).toInt());
    ui->patchControlSource3     ->setCurrentIndex(common.at(59).toInt());

    ui->efxControl    ->setCurrentIndex(common.at(60).toInt());
    ui->control1    ->setCurrentIndex(common.at(61).toInt());
    ui->control2    ->setCurrentIndex(common.at(62).toInt());
    ui->control3    ->setCurrentIndex(common.at(63).toInt());
    ui->btnVelocityRangeSwitch   ->setChecked(common.at(64).toInt() == 1);
    ui->octave->setValue(common.at(65).toInt() -3);
    ui->stretchTuneDepth->setValue(common.at(66).toInt());
    ui->voicePriority->setCurrentIndex(common.at(67).toInt());
    ui->structure12->setCurrentIndex(common.at(68).toInt());
    ui->booster12->setCurrentIndex(common.at(69).toInt());
    ui->structure34->setCurrentIndex(common.at(70).toInt());
    ui->booster34->setCurrentIndex(common.at(71).toInt());
    ui->clockSource   ->setChecked(common.at(72).toInt() == 1);
    ui->patchCategory->setCurrentIndex(common.at(73).toInt());

    /*
     * ***********************************************************
    *       loop para setValue dos tones de 0 a 3:
    *  ***********************************************************
    */
    int toneLoop = 0;
    setToneWidget:
    //========= GOTO LOOP:
    QStringList t = patch->at(toneLoop+1).split(" ");
    toneSwitchList->at(toneLoop)->setChecked(t.at(0).toInt() == 1);

    //seleção da Waveform da combo
    int wgt = t.at(1).toInt();
    int wgid = t.at(2).toInt();

    QString wnum1 = QString::number(t.at(3).toInt(),16);
    QString wnum2 = QString::number(t.at(4).toInt(),16);
    QString waveNumber = QString("%1%2").arg(wnum1).arg(wnum2);
    int wnum = waveNumber.toInt(nullptr, 16);
    for(int k=0; k<waveIdList->at(toneLoop)->count(); k++){
        Waveform *w = waveIdList->at(toneLoop)->itemData(k).value<Waveform*>();
        if(w->groupType == wgt && w->groupId == wgid && w->number == wnum){
            waveIdList->at(toneLoop)->setCurrentIndex(k);
            break;
        }
    }

    waveGainList->at(toneLoop)->setCurrentIndex(t.at(5).toInt());
    fxmSwitchList->at(toneLoop)->setChecked(t.at(6).toInt() == 1);
    fxmColorList->at(toneLoop)->setValue(t.at(7).toInt()-1);
    fxmDepthList->at(toneLoop)->setValue(t.at(8).toInt()-1);
    toneDelayModeList->at(toneLoop)->setCurrentIndex(t.at(9).toInt());
    toneDelayTimeList->at(toneLoop)->setValue(t.at(10).toInt());

    veocityRangeCrossFadeList->at(toneLoop)->setValue(t.at(11).toInt());
    veocityRangeLowerList->at(toneLoop)->setValue(t.at(12).toInt());
    veocityRangeUpperList->at(toneLoop)->setValue(t.at(13).toInt());
    keyboardRangeLowerList->at(toneLoop)->setCurrentIndex(t.at(14).toInt());
    keyboardRangeUpperList->at(toneLoop)->setCurrentIndex(t.at(15).toInt());
    redamperControlSwitchList->at(toneLoop)->setChecked(t.at(16).toInt() == 1);
    volumeControlSwitchList->at(toneLoop)->setChecked(t.at(17).toInt() == 1);
    hold1ControlSwitchList->at(toneLoop)->setChecked(t.at(18).toInt() == 1);
    pitchBendControlSwitchList->at(toneLoop)->setChecked(t.at(19).toInt() == 1);
    panControlSwitchList->at(toneLoop)->setChecked(t.at(20).toInt() == 1);

    int icontrole =0;
    int lastIndex = 21;
    for(int i=1; i<=12; i++){
        controlerDestinationList->at( (i + (toneLoop)*12)-1 )->setCurrentIndex(t.at(lastIndex).toInt());
        controlerDepthList->at( (i + (toneLoop)*12)-1 )->setValue(t.at(lastIndex+1).toInt());
        icontrole+=2;
        lastIndex+=2;
    }

    lfo1WaveFormList->at(toneLoop)->setCurrentIndex(t.at(45).toInt());
    lfo1KeySyncSwitchList->at(toneLoop)->setChecked(t.at(46).toInt() == 1);
    lfo1RateList->at(toneLoop)->setValue(t.at(47).toInt());
    lfo1OffSetList->at(toneLoop)->setCurrentIndex(t.at(48).toInt());
    lfo1DelayTimeList->at(toneLoop)->setValue(t.at(49).toInt());
    lfo1FadeModeList->at(toneLoop)->setCurrentIndex(t.at(50).toInt());
    lfo1FadeTimeList->at(toneLoop)->setValue(t.at(51).toInt());
    lfo1ExternalSyncList->at(toneLoop)->setCurrentIndex(t.at(52).toInt());
    lfo2WaveFormList->at(toneLoop)->setCurrentIndex(t.at(53).toInt());
    lfo2KeySyncSwitchList->at(toneLoop)->setChecked(t.at(54).toInt() == 1);
    lfo2RateList->at(toneLoop)->setValue(t.at(55).toInt());
    lfo2OffSetList->at(toneLoop)->setCurrentIndex(t.at(56).toInt());
    lfo2DelayTimeList->at(toneLoop)->setValue(t.at(57).toInt());
    lfo2FadeModeList->at(toneLoop)->setCurrentIndex(t.at(58).toInt());
    lfo2FadeTimeList->at(toneLoop)->setValue(t.at(59).toInt());
    lfo2ExternalSyncList->at(toneLoop)->setCurrentIndex(t.at(60).toInt());

    coarseTuneList->at(toneLoop)->setValue(t.at(61).toInt()-48);
    fineTuneList->at(toneLoop)->setValue(t.at(62).toInt()-50);
    randomPitchDepthList->at(toneLoop)->setCurrentIndex(t.at(63).toInt());
    pitchKeyfollowList->at(toneLoop)->setCurrentIndex(t.at(64).toInt());
    pitchEnvelopeDepthList->at(toneLoop)->setValue(t.at(65).toInt());
    pitchEnvelopeVelocitySensList->at(toneLoop)->setValue(t.at(66).toInt());
    pitchEnvelopeVelocityTime1List->at(toneLoop)->setCurrentIndex(t.at(67).toInt());
    pitchEnvelopeVelocityTime4List->at(toneLoop)->setCurrentIndex(t.at(68).toInt());
    pitchEnvelopeTimeKeyfollowList->at(toneLoop)->setCurrentIndex(t.at(69).toInt());
    pitchEnvelopeTime1List->at(toneLoop)->setValue(t.at(70).toInt());
    pitchEnvelopeTime2List->at(toneLoop)->setValue(t.at(71).toInt());
    pitchEnvelopeTime3List->at(toneLoop)->setValue(t.at(72).toInt());
    pitchEnvelopeTime4List->at(toneLoop)->setValue(t.at(73).toInt());
    pitchEnvelopeLevel1List->at(toneLoop)->setValue(t.at(74).toInt());
    pitchEnvelopeLevel2List->at(toneLoop)->setValue(t.at(75).toInt());
    pitchEnvelopeLevel3List->at(toneLoop)->setValue(t.at(76).toInt());
    pitchEnvelopeLevel4List->at(toneLoop)->setValue(t.at(77).toInt());
    pitchLfo1DepthList->at(toneLoop)->setValue(t.at(78).toInt());
    pitchLfo2DepthList->at(toneLoop)->setValue(t.at(79).toInt());

    filterTypeList->at(toneLoop)->setCurrentIndex(t.at(80).toInt());
    CutoffFrequencyList->at(toneLoop)->setValue(t.at(81).toInt());
    CutoffKeyfollowList->at(toneLoop)->setCurrentIndex(t.at(82).toInt());
    resonanceList->at(toneLoop)->setValue(t.at(83).toInt());
    resonanceVelocitySensList->at(toneLoop)->setValue(t.at(84).toInt());
    filterEnvelopeDepthList->at(toneLoop)->setValue(t.at(85).toInt());
    filterEnvelopeVelocityCurveList->at(toneLoop)->setCurrentIndex(t.at(86).toInt());
    filterEnvelopeVelocitySensList->at(toneLoop)->setValue(t.at(87).toInt());
    filterEnvelopeVelocityTime1List->at(toneLoop)->setCurrentIndex(t.at(88).toInt());
    filterEnvelopeVelocityTime4List->at(toneLoop)->setCurrentIndex(t.at(89).toInt());
    filterEnvelopeTimeKeyfollowList->at(toneLoop)->setCurrentIndex(t.at(90).toInt());
    filterEnvelopeTime1List->at(toneLoop)->setValue(t.at(91).toInt());
    filterEnvelopeTime2List->at(toneLoop)->setValue(t.at(92).toInt());
    filterEnvelopeTime3List->at(toneLoop)->setValue(t.at(93).toInt());
    filterEnvelopeTime4List->at(toneLoop)->setValue(t.at(94).toInt());
    filterEnvelopeLevel1List->at(toneLoop)->setValue(t.at(95).toInt());
    filterEnvelopeLevel2List->at(toneLoop)->setValue(t.at(96).toInt());
    filterEnvelopeLevel3List->at(toneLoop)->setValue(t.at(97).toInt());
    filterEnvelopeLevel4List->at(toneLoop)->setValue(t.at(98).toInt());
    filterLfo1DepthList->at(toneLoop)->setValue(t.at(99).toInt());
    filterLfo2DepthList->at(toneLoop)->setValue(t.at(100).toInt());

    toneLevelList->at(toneLoop)->setValue(t.at(101).toInt());
    biasDirectionList->at(toneLoop)->setCurrentIndex(t.at(102).toInt());
    biasPositionList->at(toneLoop)->setCurrentIndex(t.at(103).toInt());
    biasLevelList->at(toneLoop)->setCurrentIndex(t.at(104).toInt());
    levelEnvelopeVelocityCurveList->at(toneLoop)->setCurrentIndex(t.at(105).toInt());
    levelEnvelopeVelocitySensList->at(toneLoop)->setValue(t.at(106).toInt());
    levelEnvelopeVelocityTime1List->at(toneLoop)->setCurrentIndex(t.at(107).toInt());
    levelEnvelopeVelocityTime4List->at(toneLoop)->setCurrentIndex(t.at(108).toInt());
    levelEnvelopeTimeKeyfollowList->at(toneLoop)->setCurrentIndex(t.at(109).toInt());
    levelEnvelopeTime1List->at(toneLoop)->setValue(t.at(110).toInt());
    levelEnvelopeTime2List->at(toneLoop)->setValue(t.at(111).toInt());
    levelEnvelopeTime3List->at(toneLoop)->setValue(t.at(112).toInt());
    levelEnvelopeTime4List->at(toneLoop)->setValue(t.at(113).toInt());
    levelEnvelopeLevel1List->at(toneLoop)->setValue(t.at(114).toInt());
    levelEnvelopeLevel2List->at(toneLoop)->setValue(t.at(115).toInt());
    levelEnvelopeLevel3List->at(toneLoop)->setValue(t.at(116).toInt());
    levelLfo1DepthList->at(toneLoop)->setValue(t.at(117).toInt());
    levelLfo2DepthList->at(toneLoop)->setValue(t.at(118).toInt());
    tonePanList->at(toneLoop)->setValue(t.at(119).toInt());
    panKeyfollowList->at(toneLoop)->setCurrentIndex(t.at(120).toInt());
    randomPanDepthList->at(toneLoop)->setValue(t.at(121).toInt());
    alternatePanDepthList->at(toneLoop)->setValue(t.at(122).toInt());
    panLfo1DepthList->at(toneLoop)->setValue(t.at(123).toInt());
    panLfo2DepthList->at(toneLoop)->setValue(t.at(124).toInt());
    outputAssignList->at(toneLoop)->setCurrentIndex(t.at(125).toInt());
    mixEfxSendLevelList->at(toneLoop)->setValue(t.at(126).toInt());
    ChorusSendLevelList->at(toneLoop)->setValue(t.at(127).toInt());
    ReverbSendLevelList->at(toneLoop)->setValue(t.at(128).toInt());

    /*
     * UMA ITERAÇÃO ENCERRADA
     */
    toneLoop++;
    if(toneLoop<=3)
        goto setToneWidget;
    //======================== LOOP ENCERRADO ===========================

    conectarWidgets();

    for(int i=0; i<4; i++){
        enviarPacotesDS1(i);
        usleep(100000);
    }
    enviarPacotesDS1PatchCommon();
}

void PatchUI::carregarFiltroCategorias(QString categ){
    desconectarWidgets();
    ui->patch->clear();
    for(Patch* p : *Controller::queryPatches() ){
        if(categ == NULL || categorias[p->categId].compare(categ) == 0)
            ui->patch->addItem(p->getFullName(), QVariant::fromValue(p));
    }
    conectarWidgets();
}

void PatchUI::onFiltrarCategoria(int i){
    if(ui->filtroCategoria->currentIndex() == 0)
        carregarFiltroCategorias(NULL);
    else
        carregarFiltroCategorias(ui->filtroCategoria->currentText());
}

void PatchUI::enviarPacotesDS1(int tone){
    QList<int> *dados = new QList<int>();
//    int tone = 0; //de 0 a 3
    int offset = 0x1000; //tone 0
    if(tone == 1) offset = 0x1200;
    if(tone == 2) offset = 0x1400;
    if(tone == 3) offset = 0x1600;
    int addr = 0x03000000 + offset; //patch temporary + patch common|tone

    Waveform *w = waveIdList->at(tone)->currentData().value<Waveform*>();
    int waveNumber2 = w->number & 0xF;
    int waveNumber1 = (w->number >> 4) & 0xF;

    dados->append(toneSwitchList->at(tone)->isChecked()? 1 : 0);
    dados->append(w->groupType);
    dados->append(w->groupId);
    dados->append(waveNumber1);
    dados->append(waveNumber2);
    dados->append(waveGainList->at(tone)->currentIndex());
    dados->append(fxmSwitchList->at(tone)->isChecked()? 1 : 0);
    dados->append(fxmColorList->at(tone)->value());
    dados->append(fxmDepthList->at(tone)->value());
    dados->append(toneDelayModeList->at(tone)->currentIndex());
    dados->append(toneDelayTimeList->at(tone)->value());
    dados->append(veocityRangeCrossFadeList->at(tone)->value());
    dados->append(veocityRangeLowerList ->at(tone)->value());
    dados->append(veocityRangeUpperList ->at(tone)->value());
    dados->append(keyboardRangeLowerList ->at(tone)->currentIndex());
    dados->append(keyboardRangeUpperList ->at(tone)->currentIndex());
    dados->append(redamperControlSwitchList ->at(tone)->isChecked()? 1 : 0);
    dados->append(volumeControlSwitchList ->at(tone)->isChecked()? 1 : 0);
    dados->append(hold1ControlSwitchList ->at(tone)->isChecked()? 1 : 0);
    dados->append(pitchBendControlSwitchList ->at(tone)->isChecked()? 1 : 0);
    dados->append(panControlSwitchList ->at(tone)->isChecked()? 1 : 0);


    int icontrole =0;
    for(int i=1; i<=12; i++){
        int dest = controlerDestinationList->at( (i + (tone)*12)-1 )->currentIndex();
        int depth = controlerDepthList->at( (i + (tone)*12)-1 )->value();
        dados->append(dest);
        dados->append(depth);
        icontrole+=2;
    }

    dados->append(lfo1WaveFormList ->at(tone)->currentIndex());
    dados->append(lfo1KeySyncSwitchList ->at(tone)->isChecked()? 1 : 0);
    dados->append(lfo1RateList ->at(tone)->value());
    dados->append(lfo1OffSetList ->at(tone)->currentIndex());
    dados->append(lfo1DelayTimeList ->at(tone)->value());
    dados->append(lfo1FadeModeList ->at(tone)->currentIndex());
    dados->append(lfo1FadeTimeList ->at(tone)->value());
    dados->append(lfo1ExternalSyncList ->at(tone)->currentIndex());

    dados->append(lfo2WaveFormList ->at(tone)->currentIndex());
    dados->append(lfo2KeySyncSwitchList ->at(tone)->isChecked()? 1 : 0);
    dados->append(lfo2RateList ->at(tone)->value());
    dados->append(lfo2OffSetList ->at(tone)->currentIndex());
    dados->append(lfo2DelayTimeList ->at(tone)->value());
    dados->append(lfo2FadeModeList ->at(tone)->currentIndex());
    dados->append(lfo2FadeTimeList ->at(tone)->value());
    dados->append(lfo2ExternalSyncList ->at(tone)->currentIndex());


    dados->append(coarseTuneList ->at(tone)->value() +48);
    dados->append(fineTuneList ->at(tone)->value() +50);
    dados->append(randomPitchDepthList ->at(tone)->currentIndex());
    dados->append(pitchKeyfollowList ->at(tone)->currentIndex());
    dados->append(pitchEnvelopeDepthList ->at(tone)->value()+12);
    dados->append(pitchEnvelopeVelocitySensList ->at(tone)->value());
    dados->append(pitchEnvelopeVelocityTime1List ->at(tone)->currentIndex());
    dados->append(pitchEnvelopeVelocityTime4List ->at(tone)->currentIndex());
    dados->append(pitchEnvelopeTimeKeyfollowList ->at(tone)->currentIndex());
    dados->append(pitchEnvelopeTime1List ->at(tone)->value());
    dados->append(pitchEnvelopeTime2List ->at(tone)->value());
    dados->append(pitchEnvelopeTime3List ->at(tone)->value());
    dados->append(pitchEnvelopeTime4List ->at(tone)->value());
    dados->append(pitchEnvelopeLevel1List ->at(tone)->value());
    dados->append(pitchEnvelopeLevel2List ->at(tone)->value());
    dados->append(pitchEnvelopeLevel3List ->at(tone)->value());
    dados->append(pitchEnvelopeLevel4List ->at(tone)->value());
    dados->append(pitchLfo1DepthList ->at(tone)->value());
    dados->append(pitchLfo2DepthList ->at(tone)->value());

    dados->append(filterTypeList ->at(tone)->currentIndex());
    dados->append(CutoffFrequencyList ->at(tone)->value());
    dados->append(CutoffKeyfollowList ->at(tone)->currentIndex());
    dados->append(resonanceList ->at(tone)->value());
    dados->append(resonanceVelocitySensList ->at(tone)->value());
    dados->append(filterEnvelopeDepthList ->at(tone)->value());
    dados->append(filterEnvelopeVelocityCurveList ->at(tone)->currentIndex());
    dados->append(filterEnvelopeVelocitySensList ->at(tone)->value());
    dados->append(filterEnvelopeVelocityTime1List ->at(tone)->currentIndex());
    dados->append(filterEnvelopeVelocityTime4List ->at(tone)->currentIndex());
    dados->append(filterEnvelopeTimeKeyfollowList ->at(tone)->currentIndex());
    dados->append(filterEnvelopeTime1List ->at(tone)->value());
    dados->append(filterEnvelopeTime2List ->at(tone)->value());
    dados->append(filterEnvelopeTime3List ->at(tone)->value());
    dados->append(filterEnvelopeTime4List ->at(tone)->value());
    dados->append(filterEnvelopeLevel1List ->at(tone)->value());
    dados->append(filterEnvelopeLevel2List ->at(tone)->value());
    dados->append(filterEnvelopeLevel3List ->at(tone)->value());
    dados->append(filterEnvelopeLevel4List ->at(tone)->value());
    dados->append(filterLfo1DepthList ->at(tone)->value());
    dados->append(filterLfo2DepthList ->at(tone)->value());

    dados->append(toneLevelList ->at(tone)->value());
    dados->append(biasDirectionList ->at(tone)->currentIndex());
    dados->append(biasPositionList ->at(tone)->currentIndex());
    dados->append(biasLevelList ->at(tone)->currentIndex());
    dados->append(levelEnvelopeVelocityCurveList  ->at(tone)->currentIndex());
    dados->append(levelEnvelopeVelocitySensList ->at(tone)->value());
    dados->append(levelEnvelopeVelocityTime1List ->at(tone)->currentIndex());
    dados->append(levelEnvelopeVelocityTime4List ->at(tone)->currentIndex());
    dados->append(levelEnvelopeTimeKeyfollowList ->at(tone)->currentIndex());
    dados->append(levelEnvelopeTime1List ->at(tone)->value());
    dados->append(levelEnvelopeTime2List ->at(tone)->value());
    dados->append(levelEnvelopeTime3List ->at(tone)->value());
    dados->append(levelEnvelopeTime4List ->at(tone)->value());
    dados->append(levelEnvelopeLevel1List ->at(tone)->value());
    dados->append(levelEnvelopeLevel2List ->at(tone)->value());
    dados->append(levelEnvelopeLevel3List ->at(tone)->value());
    dados->append(levelLfo1DepthList ->at(tone)->value());
    dados->append(levelLfo2DepthList ->at(tone)->value());
    dados->append(tonePanList ->at(tone)->value());
    dados->append(panKeyfollowList ->at(tone)->currentIndex());
    dados->append(randomPanDepthList ->at(tone)->value());
    dados->append(alternatePanDepthList ->at(tone)->value());
    dados->append(panLfo1DepthList ->at(tone)->value());
    dados->append(panLfo2DepthList ->at(tone)->value());
    dados->append(outputAssignList ->at(tone)->currentIndex());
    dados->append(mixEfxSendLevelList ->at(tone)->value());
    dados->append(ChorusSendLevelList ->at(tone)->value());

    jack->txPacoteDataSet(addr, dados);
    //próxima mensagem:
    dados->append(ReverbSendLevelList ->at(tone)->value());
    jack->txPacoteDataSet(addr, dados);


}

void PatchUI::enviarPacotesDS1PatchCommon(){
    QList<int> *dados = new QList<int>();
    int addr = 0x03000000;

    QString nome = ui->name->text();
    for(int i=nome.length(); i<12; i++)
        nome.append(" ");
    for(QChar c : nome)
        dados->append(c.unicode());

    dados->append(ui->perfEfeito->currentIndex());
    dados->append(ui->perfParam1->value());
    dados->append(ui->perfParam2->value());
    dados->append(ui->perfParam3->value());
    dados->append(ui->perfParam4->value());
    dados->append(ui->perfParam5->value());
    dados->append(ui->perfParam6->value());
    dados->append(ui->perfParam7->value());
    dados->append(ui->perfParam8->value());
    dados->append(ui->perfParam9->value());
    dados->append(ui->perfParam10->value());
    dados->append(ui->perfParam11->value());
    dados->append(ui->perfParam12->value());

    dados->append(ui->perfOA->currentIndex());
    dados->append(ui->mixOutSendLevel->value());
    dados->append(ui->chorusSendLevel->value());
    dados->append(ui->reverbSendLevel->value());

    dados->append(ui->perfCtrlSrc1->currentIndex());
    dados->append(ui->perfCtrlDepth1->value());
    dados->append(ui->perfCtrlSrc2->currentIndex());
    dados->append(ui->perfCtrlDepth2->value());

    dados->append(ui->perfChorusLevel_3->value());
    dados->append(ui->perfChorusRate->value());
    dados->append(ui->perfChorusDepth->value());
    dados->append(ui->perfChorusPreDelay_3->value());
    dados->append(ui->perfChorusFeedback->value());
    dados->append(ui->perfChorusOut->currentIndex());

    dados->append(ui->perfReverbType->currentIndex());
    dados->append(ui->perfReverbLevel->value());
    dados->append(ui->perfReverbTime->value());
    dados->append(ui->perfReverbHFDamp->currentIndex());
    dados->append(ui->perfReverbDelayFeedback->value());

    int tempo = ui->tempo->value();
    int tempo2 = tempo & 0xF;
    int tempo1 = (tempo >> 4) & 0xF;

    dados->append(tempo1);
    dados->append(tempo2);
qDebug() << "TEMPO" << tempo << tempo1 << tempo2;

    dados->append(ui->patchLevel  ->value());
    dados->append(ui->patchPan    ->value());
    dados->append(ui->analogFeel  ->value());
    dados->append(ui->bendMax     ->value());
    dados->append(ui->bendMin     ->value()*-1);
    dados->append(ui->btnKeyAssignMode    ->isChecked() ? 1 : 0);
    dados->append(ui->btnSoloLegato       ->isChecked() ? 1 : 0);
    dados->append(ui->btnPortamentoSwitch ->isChecked() ? 1 : 0);
    dados->append(ui->btnPortamentoMode   ->isChecked() ? 1 : 0);
    dados->append(ui->portamentoType      ->currentIndex());
    dados->append(ui->portamentoStart     ->currentIndex());
    dados->append(ui->portamentoTime              ->value());
    dados->append(ui->patchControlSource2     ->currentIndex());
    dados->append(ui->patchControlSource3     ->currentIndex());
    dados->append(ui->efxControl    ->currentIndex());
    dados->append(ui->control1    ->currentIndex());
    dados->append(ui->control2    ->currentIndex());
    dados->append(ui->control3    ->currentIndex());
    dados->append(ui->btnVelocityRangeSwitch   ->isChecked() ? 1 : 0);
    dados->append(ui->octave->value() +3);
    dados->append(ui->stretchTuneDepth->value());
    dados->append(ui->voicePriority->currentIndex());
    dados->append(ui->structure12->currentIndex());
    dados->append(ui->booster12->currentIndex());
    dados->append(ui->structure34->currentIndex());
    dados->append(ui->booster34->currentIndex());
    dados->append(ui->clockSource   ->isChecked() ? 1 : 0);
    dados->append(ui->patchCategory->currentIndex());

    jack->txPacoteDataSet(addr, dados);

}

/*
void PatchUI::enviarMensagem(PatchTone *patchTone, int data){
    QList<SysExMessage*> *dados = new QList<SysExMessage*>();

    dados->append(
                new SysExMessage(
                    BaseAddress(BaseAddress::PatchModeTempPatch),
                    patchTone,
                    data));

    jack->tx(dados);
}

void PatchUI::enviarMensagem(Patch *patch, int data){
    QList<SysExMessage*> *dados = new QList<SysExMessage*>();

    dados->append(
                new SysExMessage(
                    BaseAddress(BaseAddress::PatchModeTempPatch),
                    patch,
                    data));

    jack->tx(dados);
}
*/
void PatchUI::conectarWidgets()
{
    //deve ser foreach QWidget, lista
    //teste
    //    for(int i=0; i<toneSwitchList->count(); i++)
    //        connect(toneSwitchList->at(i),SIGNAL( clicked()), this, SLOT(onPatchToneChanged()));

    connect(ui->filtroCategoria, SIGNAL(currentIndexChanged(int)), this, SLOT(onFiltrarCategoria(int)));
    connect(ui->patch, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchSelected(int)));

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



    /*
     * PATCH COMMON: EFX
     * */
    connect(ui->perfEfeito, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfOA, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfCtrlSrc1, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfCtrlSrc2, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfChorusOut, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfReverbType, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfReverbHFDamp, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->mixOutSendLevel, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->chorusSendLevel, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->reverbSendLevel, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfCtrlDepth1, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfCtrlDepth2, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam1, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam2, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam3, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam4, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam5, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam6, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam7, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam8, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam9, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam10, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam11, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfParam11, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfChorusRate, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfChorusDepth, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfChorusLevel_3, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfChorusFeedback, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfChorusPreDelay_3, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfReverbTime, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfReverbLevel, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->perfReverbDelayFeedback, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );


    //PATCH COMMON
    connect(ui->patchLevel  , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->patchPan    , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->analogFeel  , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->bendMax     , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->bendMin     , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->btnKeyAssignMode    , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    connect(ui->btnSoloLegato       , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    connect(ui->btnPortamentoSwitch , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    connect(ui->btnPortamentoMode   , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    connect(ui->portamentoType      , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->portamentoStart     , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->portamentoTime              , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->patchControlSource2     , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->patchControlSource3     , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->efxControl    , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->control1    , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->control2    , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->control3    , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->btnVelocityRangeSwitch   , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    connect(ui->octave, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->stretchTuneDepth, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->voicePriority, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->structure12, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->booster12, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->structure34, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->booster34, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    connect(ui->clockSource   , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    connect(ui->patchCategory, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
}

void PatchUI::desconectarWidgets(){
    disconnect(ui->filtroCategoria, SIGNAL(currentIndexChanged(int)), this, SLOT(onFiltrarCategoria(int)));
    disconnect(ui->patch, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchSelected(int)));

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


    /*
     * PATCH COMMON: EFX
     * */
    disconnect(ui->perfEfeito, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfOA, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfCtrlSrc1, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfCtrlSrc2, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfChorusOut, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfReverbType, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfReverbHFDamp, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->mixOutSendLevel, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->chorusSendLevel, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->reverbSendLevel, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfCtrlDepth1, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfCtrlDepth2, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam1, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam2, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam3, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam4, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam5, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam6, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam7, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam8, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam9, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam10, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam11, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfParam11, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfChorusRate, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfChorusDepth, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfChorusLevel_3, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfChorusFeedback, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfChorusPreDelay_3, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfReverbTime, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfReverbLevel, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->perfReverbDelayFeedback, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );

    //PATCH COMMON
    disconnect(ui->patchLevel  , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->patchPan    , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->analogFeel  , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->bendMax     , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->bendMin     , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->btnKeyAssignMode    , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    disconnect(ui->btnSoloLegato       , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    disconnect(ui->btnPortamentoSwitch , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    disconnect(ui->btnPortamentoMode   , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    disconnect(ui->portamentoType      , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->portamentoStart     , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->portamentoTime              , SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->patchControlSource2     , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->patchControlSource3     , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->efxControl    , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->control1    , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->control2    , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->control3    , SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->btnVelocityRangeSwitch   , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    disconnect(ui->octave, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->stretchTuneDepth, SIGNAL(valueChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->voicePriority, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->structure12, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->booster12, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->structure34, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->booster34, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );
    disconnect(ui->clockSource   , SIGNAL(clicked()), this, SLOT(onPatchCommonEfx()) );
    disconnect(ui->patchCategory, SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchCommonEfx(int)) );

}

void PatchUI::setupProperties(){
    /*
     * Falta:
     * - waveId
     */
    for(int i=0; i<waveIdList->count(); i++) waveIdList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Wave_Number, i+1)));

    //controllers: dest e depth


    int indexDestination = 0;
    for(int tone=1; tone<=4; tone++){
        for(int i=1; i<=12; i++){
            controlerDestinationList->at( (i + (tone-1)*12)-1 )
                    ->setProperty("function", QVariant::fromValue(new PatchTone(controles[indexDestination], tone)));
            controlerDepthList->at( (i + (tone-1)*12)-1 )
                    ->setProperty("function", QVariant::fromValue(new PatchTone(controles[indexDestination+1], tone)));

            indexDestination+=2;
            if( indexDestination%24 == 0)
                indexDestination=0;
        }
    }





    for(int i=0; i<toneSwitchList->count(); i++){
        toneSwitchList->at(i)->setAccessibleName("toggle-btn");
        toneSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Switch, i+1)));
    }
    for(int i=0; i<waveGainList->count(); i++) waveGainList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Wave_Gain, i+1)));
    for(int i=0; i<fxmColorList->count(); i++) fxmColorList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::FXM_Color, i-1)));
    for(int i=0; i<fxmDepthList->count(); i++) fxmDepthList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::FXM_Depth, i-1)));
    for(int i=0; i<toneDelayModeList->count(); i++) toneDelayModeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Delay_Mode, i+1)));
    for(int i=0; i<toneDelayTimeList->count(); i++) toneDelayTimeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Delay_Time, i+1)));
    for(int i=0; i<veocityRangeCrossFadeList->count(); i++) veocityRangeCrossFadeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Velocity_Cross_Fade, i+1)));
    for(int i=0; i<veocityRangeLowerList->count(); i++) veocityRangeLowerList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Velocity_Range_Lower, i+1)));
    for(int i=0; i<veocityRangeUpperList->count(); i++) veocityRangeUpperList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Velocity_Range_Upper, i+1)));
    for(int i=0; i<keyboardRangeLowerList->count(); i++) keyboardRangeLowerList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Keyboard_Range_Lower, i+1)));
    for(int i=0; i<keyboardRangeUpperList->count(); i++) keyboardRangeUpperList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Keyboard_Range_Upper, i+1)));
    for(int i=0; i<fxmSwitchList->count(); i++) {
        fxmSwitchList->at(i)->setAccessibleName("toggle-btn");
        fxmSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::FXM_Switch, i+1)));
    }
    for(int i=0; i<redamperControlSwitchList->count(); i++){
        redamperControlSwitchList->at(i)->setAccessibleName("toggle-btn");
        redamperControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Redamper_Control_Switch, i+1)));
    }
    for(int i=0; i<volumeControlSwitchList->count(); i++) {
        volumeControlSwitchList->at(i)->setAccessibleName("toggle-btn");
        volumeControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Volume_Control_Switch, i+1)));
    }
    for(int i=0; i<hold1ControlSwitchList->count(); i++){
        hold1ControlSwitchList->at(i)->setAccessibleName("toggle-btn");
        hold1ControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Hold1_Control_Switch, i+1)));
    }
    for(int i=0; i<pitchBendControlSwitchList->count(); i++){
        pitchBendControlSwitchList->at(i)->setAccessibleName("toggle-btn");
        pitchBendControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pitch_Bend_Control_Switch, i+1)));
    }
    for(int i=0; i<panControlSwitchList->count(); i++) {
        panControlSwitchList->at(i)->setAccessibleName("toggle-btn");
        panControlSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Pan_Control_Switch, i+1)));
    }
    for(int i=0; i<lfo1WaveFormList->count(); i++) lfo1WaveFormList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Waveform, i+1)));
    for(int i=0; i<lfo1KeySyncSwitchList->count(); i++) {
        lfo1KeySyncSwitchList->at(i)->setAccessibleName("toggle-btn");
        lfo1KeySyncSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Key_Sync, i+1)));
    }
    for(int i=0; i<lfo1RateList->count(); i++) lfo1RateList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Rate, i+1)));
    for(int i=0; i<lfo1OffSetList->count(); i++) lfo1OffSetList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Offset, i+1)));
    for(int i=0; i<lfo1DelayTimeList->count(); i++) lfo1DelayTimeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Delay_Time, i+1)));
    for(int i=0; i<lfo1FadeModeList->count(); i++) lfo1FadeModeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Fade_Mode, i+1)));
    for(int i=0; i<lfo1FadeTimeList->count(); i++) lfo1FadeTimeList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_Fade_Time, i+1)));
    for(int i=0; i<lfo1ExternalSyncList->count(); i++) lfo1ExternalSyncList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO1_External_Sync, i+1)));
    for(int i=0; i<lfo2WaveFormList->count(); i++) lfo2WaveFormList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_Waveform, i+1)));
    for(int i=0; i<lfo2KeySyncSwitchList->count(); i++) {
        lfo2KeySyncSwitchList->at(i)->setAccessibleName("toggle-btn");
        lfo2KeySyncSwitchList->at(i)->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::LFO2_Key_Sync, i+1)));
    }
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

    enviarPacotesDS1(patchTone->whichTone-1);
    return;


//    enviarMensagem( patchTone, w->isChecked()? 1 : 0 );





}
void PatchUI::onPatchToneChanged(int i){
    QObject* o = QObject::sender();
    PatchTone *patchTone = o->property("function").value<PatchTone*>();




//    int offset = 0;
    int theTone = patchTone->whichTone-1;

    /*
     * Validações
     * */
    if(veocityRangeUpperList->at(theTone)->value() < veocityRangeLowerList->at(theTone)->value() )
        veocityRangeUpperList->at(theTone)->setValue(veocityRangeLowerList->at(theTone)->value());

    if(veocityRangeUpperList->at(theTone)->value() < veocityRangeLowerList->at(theTone)->value() )
        veocityRangeLowerList->at(theTone)->setValue(veocityRangeUpperList->at(theTone)->value());


    /*
     * Três envios quando selecionada a forma de onda:
     */
//    if(patchTone->function == PatchTone::Wave_Number){
//        isTxDiferenciada = true;
//        QComboBox *c = qobject_cast<QComboBox*>(o);
//        Waveform *w =  c->currentData().value<Waveform*>();
//        enviarMensagem( new PatchTone(PatchTone::Wave_Group_Type, patchTone->whichTone), w->groupType );
//        enviarMensagem( new PatchTone(PatchTone::Wave_Group_ID, patchTone->whichTone), w->groupId );
//        enviarMensagem( new PatchTone(PatchTone::Wave_Number, patchTone->whichTone), w->number );

//        qDebug() <<"======================================";
//        qDebug() << "WAVE GROUP TYPE: " << w->groupType ;
//        qDebug() << "WAVE GROUP ID  : " << w->groupId;
//        qDebug() << "WAVE NUMBER    # " << w->number ;
//        qDebug() << "WAVE NOME      : " << w->nome ;
//        qDebug() <<"======================================";
//    }



//        if(QString::compare(o->metaObject()->className(), "QComboBox") == 0){
//            QComboBox *c = qobject_cast<QComboBox*>(o);
//            enviarMensagem( patchTone, c->currentIndex() );
//        }
//        if(QString::compare(o->metaObject()->className(), "QSpinBox") == 0){
//            QSpinBox *s = qobject_cast<QSpinBox*>(o);
//            enviarMensagem( patchTone, s->value() + offset );
//        }
//        if(QString::compare(o->metaObject()->className(), "QSlider") == 0){
//            QSlider *s = qobject_cast<QSlider*>(o);
//            enviarMensagem( patchTone, s->value() + offset );
//        }



//    if(enviarControlesDestDepth){
//        int icontrole =0;
//        for(int i=1; i<=12; i++){
//            enviarMensagem(
//                        new PatchTone(controles[icontrole], theTone+1),
//                        controlerDestinationList->at( (i + (theTone)*12)-1 )->currentIndex()
//                        );
//            enviarMensagem(
//                        new PatchTone(controles[icontrole+1], theTone+1),
//                    controlerDepthList->at( (i + (theTone)*12)-1 )->value()
//                    );

//            icontrole+=2;
//        }
//    }
    // =========================================


    enviarPacotesDS1(patchTone->whichTone-1);
}

void PatchUI::onPatchCommonEfx(int i){
    enviarPacotesDS1PatchCommon();
}

void PatchUI::onPatchCommonEfx(){
    enviarPacotesDS1PatchCommon();
}



/**
 * @brief Adiciona widgets na tela e seta valores default
 */
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
        setSpinRange(fxmColor, 1, 4);

        QSpinBox *fxmDepth = new QSpinBox();
        setSpinRange(fxmDepth,1,16);

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
//        lfo1FadeMode->setEnabled(false);//XP30 BUG
        lfo1FadeModeList->append(lfo1FadeMode );
        grid->addWidget(lfo1FadeMode , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *lfo1FadeTime = new QSlider(Qt::Horizontal);
        lfo1FadeTimeList->append(lfo1FadeTime );
        grid->addWidget(lfo1FadeTime , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *lfo1ExternalSync = new QComboBox();
//        lfo1ExternalSync->setEnabled(false);//XP30 BUG
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
//        lfo2FadeMode->setEnabled(false);//XP30 BUG
        lfo2FadeModeList->append(lfo2FadeMode );
        grid->addWidget(lfo2FadeMode , r++, toneid, 1, 1, Qt::AlignTop);
        QSlider *lfo2FadeTime = new QSlider(Qt::Horizontal);
        lfo2FadeTimeList->append(lfo2FadeTime );
        grid->addWidget(lfo2FadeTime , r++, toneid, 1, 1, Qt::AlignTop);
        QComboBox *lfo2ExternalSync = new QComboBox();
        lfo2ExternalSyncList->append(lfo2ExternalSync );
//        lfo2ExternalSync->setEnabled(false);//XP30 BUG
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

        setSliderRange(panLfo1Depth,0,126);
        panLfo1Depth->setValue(63);
        setSliderRange(panLfo2Depth,0,126);
        panLfo2Depth->setValue(63);

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



void PatchUI::on_perfEfeito_currentIndexChanged(int index)
{
    QStringList *labels = Efeito::getLabels(index);
    QList<int> *max = Efeito::getParametrosMaxVal(index);

    ui->perfParamLbl_25->setText(labels->at(0));
    ui->perfParamLbl_26->setText(labels->at(1));
    ui->perfParamLbl_27->setText(labels->at(2));
    ui->perfParamLbl_28->setText(labels->at(3));
    ui->perfParamLbl_29->setText(labels->at(4));
    ui->perfParamLbl_30->setText(labels->at(5));
    ui->perfParamLbl_31->setText(labels->at(6));
    ui->perfParamLbl_32->setText(labels->at(7));
    ui->perfParamLbl_33->setText(labels->at(8));
    ui->perfParamLbl_34->setText(labels->at(9));
    ui->perfParamLbl_35->setText(labels->at(10));
    ui->perfParamLbl_36->setText(labels->at(11));

    ui->perfParam1->setMaximum(max->at(0));
    ui->perfParam2->setMaximum(max->at(1));
    ui->perfParam3->setMaximum(max->at(2));
    ui->perfParam4->setMaximum(max->at(3));
    ui->perfParam5->setMaximum(max->at(4));
    ui->perfParam6->setMaximum(max->at(5));
    ui->perfParam7->setMaximum(max->at(6));
    ui->perfParam8->setMaximum(max->at(7));
    ui->perfParam9->setMaximum(max->at(8));
    ui->perfParam10->setMaximum(max->at(9));
    ui->perfParam11->setMaximum(max->at(10));
    ui->perfParam12->setMaximum(max->at(11));
}

void PatchUI::on_pushButton_clicked(){

//FUNCIONA:

    try {
        QList<QString>* patch = Controller::getPatch(ui->spinBox->value());

        jack->txPacoteDataSetString(0x03001000, patch->at(1));
        jack->txPacoteDataSetString(0x03001200, patch->at(2));
        jack->txPacoteDataSetString(0x03001400, patch->at(3));
        jack->txPacoteDataSetString(0x03001600, patch->at(4));
        jack->txPacoteDataSetString(0x03000000, patch->at(0));//common
    } catch (SQLite::Exception &e) {
        qDebug() << e.what();
    }


    qDebug() << "FIM TESTE";
    return;

    int msg = 0;
    //cada grupo de pacotes para compor o dump de um patch
    QList<int> patch;
    QList<int> tone0;
    QList<int> tone1;
    QList<int> tone2;
    QList<int> tone3;
    for (int i=0;i<jack->sysxin->length();i++) {

        if(jack->sysxin->at(i)==0xf0){
            msg++;
            i+=9;
//            continue;
        }

        if(jack->sysxin->at(i)!=0xf0 && jack->sysxin->at(i)!=0xf7)
            switch (msg) {
            case 1:
                patch.append(jack->sysxin->at(i));
                break;
            case 2:
                tone0.append(jack->sysxin->at(i));
                break;
            case 3:
                tone1.append(jack->sysxin->at(i));
                break;
            case 4:
                tone2.append(jack->sysxin->at(i));
                break;
            case 5:
                tone3.append(jack->sysxin->at(i));
                break;
            }

        if(msg==5 && jack->sysxin->at(i)==0xf7){

            patch.removeLast();
            patch.removeLast();

            tone0.removeLast();
            tone1.removeLast();
            tone2.removeLast();
            patch.removeLast();
            tone0.removeLast();
            tone1.removeLast();
            tone2.removeLast();

            tone3.removeLast(); //só uma vez

            QString nome;
            for (int c=0; c<12; c++) {
                nome += (char) patch.at(c);
            }
            qDebug() << "patch" << nome;

            QString commonStr, t0Str, t1Str, t2Str, t3Str;
            for(int h : patch)
                commonStr+=QString("%1 ").arg(h);
            commonStr = commonStr.trimmed();
            for(int h : tone0)
                t0Str+=QString("%1 ").arg(h);
            t0Str = t0Str.trimmed();
            for(int h : tone1)
                t1Str+=QString("%1 ").arg(h);
            t1Str = t1Str.trimmed();
            for(int h : tone2)
                t2Str+=QString("%1 ").arg(h);
            t2Str = t2Str.trimmed();
            for(int h : tone3)
                t3Str+=QString("%1 ").arg(h);
            t3Str = t3Str.trimmed();


            try {
                Controller::insertPatch(
                            0,
                            0,
                            0,
                            nome,
                            commonStr,
                            t0Str, t1Str, t2Str, t3Str
                            );
                patch.clear();
                tone0.clear();
                tone1.clear();
                tone2.clear();
                tone3.clear();
            } catch (SQLite::Exception &e) {
                qDebug() << e.what();
            }
            msg=0;
        }
    }//fim for
    qDebug() << "Apagando cache de SYSEX";
    jack->sysxin->clear();

}
/*void PatchUI::on_pushButton_clicked()
{
    //dump de Patches do teclado. Provavelmente só usarei isso uma vez na vida para salvar no SQLite

    QList<QList<int>>* patches = Controller::queryDefaultPatches();
    //vou enviar uma mudança de patch em bulk, então tenho que enviar o data dos endereços anteriors ao Patch
    QList<int> *patch;
    for (int i=0; i<patches->length(); i++) {
        patch = new QList<int>();
            patch->append(1);//sound mode = patch
            patch->append(0);//performance number, tanto faz
        //o patch da vez
        patch->append(patches->at(i).at(0)); //patch group type
        patch->append(patches->at(i).at(1)); // patch group id
        int number = patches->at(i).at(2);
        int n2 = number & 0xF;
        int n1 = (number >> 4) & 0xF;
        patch->append(n1);
        patch->append(n2);
        //seleciona o patch da vez
//        qDebug() << "PATCH len=" << patch->length()
//                 << QString::number(patch->at(0),16)
//                 << QString::number(patch->at(1),16)
//                 << QString::number(patch->at(2),16)
//                 << QString::number(patch->at(3),16)
//                 << QString::number(patch->at(4),16)
//                 << QString::number(patch->at(5),16)
//                 ;
        jack->txPacoteDataSet(0x00000000, patch);

        usleep(100000);

        //páginas de memória que serão requisitadas para dump:
        QList<int> *dados = new QList<int>();
        dados->append(0x03000000);      //patch common #75
        dados->append(0x4a);      //patch common #75
        dados->append(0x03001000);     //tone 1 #128
        dados->append(0x7f);     //tone 1 #128
        dados->append(0x0300107f);
        dados->append(0x01);
        dados->append(0x03001100);
        dados->append(0x01);
        dados->append(0x03001200);
        dados->append(0x7f);
        dados->append(0x0300127f);
        dados->append(0x01);
        dados->append(0x03001300);
        dados->append(0x01);
        dados->append(0x03001400);
        dados->append(0x7f);
        dados->append(0x0300147f);
        dados->append(0x01);
        dados->append(0x03001500);
        dados->append(0x01);
        dados->append(0x03001600);
        dados->append(0x7f);
        dados->append(0x0300167f);
        dados->append(0x01);
        dados->append(0x03001700);
        dados->append(0x01);


        int oldlen = jack->sysxin->length();
        //para cada página farei uma requisição e aguardarei um tempo para garantir o recebimento dos dados
        for (int di=0;di<dados->length();di+=2) {
//            qDebug() << "LANÇANDO THREAD";

            QThread* thread = new QThread;
            WorkerSysExRequest* worker = new WorkerSysExRequest(jack, dados->at(di), dados->at(di+1));
            worker->moveToThread(thread);
        //    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
            connect(thread, SIGNAL(started()), worker, SLOT(process()));
            connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
            connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
        //    connect(worker, SIGNAL(finished()), this, SLOT(onSysExRequestFinished()));
            connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
            connect(thread, SIGNAL(finished()), this, SLOT(onSysExRequestFinished()));
            thread->start();
//            usleep(500000);//tem que esperar antes de lançar outra thread
            int qtde = 0;
            for (;;) {
                if(oldlen == jack->sysxin->length()){
                    usleep(100000);
                    qtde++;
//                    qDebug() << "Esperou " << qtde;
                }
                else {
                    oldlen = jack->sysxin->length();
                    break;
                }
            }

        }

    }

    //separando os SYSEX recebidos em pacotes. Cada patch tem 5 pacotes: common e tones
    int bytesPorMensagem = 735;
    for (int i=0;i<jack->sysxin->length();i+=bytesPorMensagem) {
//        dados->append({ 0x03000000, 0x4a});      //patch common #75
//        dados->append({ 0x03001000, 0x7f });     //tone 1 #128
//        dados->append({ 0x03001100, 0x01 });     //tone Reverb Send Level #1
//        dados->append({ 0x03001200, 0x7f });     //tone 2 #128
//        dados->append({ 0x03001300, 0x01 });     //tone Reverb Send Level #1
//        dados->append({ 0x03001400, 0x7f });     //tone 3
//        dados->append({ 0x03001500, 0x01 });     //tone Reverb Send Level
//        dados->append({ 0x03001600, 0x7f });     //tone 4
//        dados->append({ 0x03001700, 0x01 });     //tone Reverb Send Level
//        qDebug() <<  QString::number(jack->sysxin->at(i),16) << QString::number(jack->sysxin->at(i+bytesPorMensagem-1),16) ;

        //cada grupo de pacotes para compor o dump de um patch
        QList<int> patch;
        QList<int> tone0;
        QList<int> tone1;
        QList<int> tone2;
        QList<int> tone3;
        int pacote = 0;
        for (int index=i;index<i+bytesPorMensagem;index++) {

            if(jack->sysxin->at(index) == 0xf0){
                index+=9;
                //common: 11 + 74 => 29 trios de bytes = 87
                if(pacote == 1){
                    patch.removeAt(patch.length()-1);//byte 0
                    patch.removeAt(patch.length()-1);//byte 0
                    patch.removeAt(patch.length()-1);//0xf7
                    patch.removeAt(patch.length()-1);//checksum
                }
//                //tone0
//                if(pacote == 2 || pacote == 3){
//                    tone0.removeAt(tone0.length()-1);
//                    tone0.removeAt(tone0.length()-1);

//                }
//                if(pacote == 2 || pacote == 3){
//                    tone0.removeAt(tone0.length()-1);
//                    tone0.removeAt(tone0.length()-1);

//                }
//                //tone1
//                if(pacote == 4 || pacote == 5){
//                   tone1.removeAt(tone1.length()-1);
//                    tone1.removeAt(tone1.length()-1);
//                }
//                //tone2
//                if(pacote == 6 || pacote == 7){
//                    tone2.removeAt(tone2.length()-1);
//                    tone2.removeAt(tone2.length()-1);
//                }
//                //tone3
//                if(pacote == 8 || pacote == 9){
//                    tone3.removeAt(tone3.length()-1);
//                    tone3.removeAt(tone3.length()-1);
//                }

                //tone0
                if(pacote >=2 && pacote <=4 ){
                    tone0.removeAt(tone0.length()-1);
                    tone0.removeAt(tone0.length()-1);
//                    tone0.removeAt(tone0.length()-1);
//                    tone0.removeAt(tone0.length()-1);

                }

                //tone1
                if(pacote >=5 && pacote <=7){
                    tone1.removeAt(tone1.length()-1);
                    tone1.removeAt(tone1.length()-1);
//                    tone1.removeAt(tone1.length()-1);
//                    tone1.removeAt(tone1.length()-1);
                }

                //tone2
                if(pacote >=8 && pacote <=10){
                    tone2.removeAt(tone2.length()-1);
                    tone2.removeAt(tone2.length()-1);
//                    tone2.removeAt(tone2.length()-1);
//                    tone2.removeAt(tone2.length()-1);
                }
                //tone3
                if(pacote >=11 && pacote <=13){
//                    tone3.removeAt(tone3.length()-1);
//                    tone3.removeAt(tone3.length()-1);
//                    tone3.removeAt(tone3.length()-1);
//                    tone3.removeAt(tone3.length()-1);
                }

                pacote++;
            }
            //patch common
            if(pacote == 1)
                patch.append(jack->sysxin->at(index));
            //tone0
            if(pacote >=2 && pacote <=4 )
                tone0.append(jack->sysxin->at(index));

            //tone1
            if(pacote >=5 && pacote <=7)
                tone1.append(jack->sysxin->at(index));

            //tone2
            if(pacote >=8 && pacote <=10)
                tone2.append(jack->sysxin->at(index));

            //tone3
            if(pacote >=11 && pacote <=13){
                tone3.append(jack->sysxin->at(index));
            }

        }

        //agora remove os dois últimos da mensagem inteira do patch
        tone3.removeAt(tone3.length()-1);
        tone3.removeAt(tone3.length()-1);
        tone3.removeAt(tone3.length()-2);
        tone3.removeAt(tone3.length()-2);
        tone3.removeAt(tone3.length()-3);
        tone3.removeAt(tone3.length()-3);


        QString nome;
        for (int c=0; c<12; c++) {
            nome += (char) patch.at(c);
        }
        QString dados;
//        for(int i : patch)
//            dados += QString("%1 ").arg(i);
        QString t0;
        for(int k=tone0.length()-10; k<tone0.length();k++)
            t0 += QString("%1 ").arg(tone0.at(k));
        QString t1;
        for(int k=tone1.length()-10; k<tone1.length();k++)
            t1 += QString("%1 ").arg(tone1.at(k));
        QString t2;
        for(int k=tone2.length()-10; k<tone2.length();k++)
            t2 += QString("%1 ").arg(tone2.at(k));
        QString t3;
        for(int k=tone3.length()-10; k<tone3.length();k++)
            t3 += QString("%1 ").arg(tone3.at(k));

//        qDebug() << "patch" << nome << "DADOS: " << dados << "#" << patch.length() << patch.at(0) << "/" << patch.at(patch.length()-1)
//                 << "tone0 #" << tone0.length() << tone0.at(0) << "/" << tone0.at(tone0.length()-1) << t0
//                 << "tone1 #" << tone1.length() << tone1.at(0) << "/" << tone0.at(tone1.length()-1) << t1
//                 << "tone2 #" << tone2.length() << tone2.at(0) << "/" << tone0.at(tone2.length()-1) << t2
//                 << "tone3 #" << tone3.length() << tone3.at(0) << "/" << tone0.at(tone3.length()-1) << t3
//                    ;

        qDebug() << "patch" << nome;

        QString commonStr, t0Str, t1Str, t2Str, t3Str;
        for(int h : patch)
            commonStr+=QString("%1 ").arg(h);
        commonStr = commonStr.trimmed();
        for(int h : tone0)
            t0Str+=QString("%1 ").arg(h);
        t0Str = t0Str.trimmed();
        for(int h : tone1)
            t1Str+=QString("%1 ").arg(h);
        t1Str = t1Str.trimmed();
        for(int h : tone2)
            t2Str+=QString("%1 ").arg(h);
        t2Str = t2Str.trimmed();
        for(int h : tone3)
            t3Str+=QString("%1 ").arg(h);
        t3Str = t3Str.trimmed();

        try {
            Controller::insertPatch(
                        patches->at(i).at(0),
                        patches->at(i).at(1),
                        patches->at(i).at(2),
                        nome,
                        commonStr,
                        t0Str, t1Str, t2Str, t3Str
                        );
        } catch (SQLite::Exception &e) {
            qDebug() << e.what();
        }

    };
//    qDebug() << "ENCERROU (para 5 patches deve ter 2955 ou 3110 622/cada ) " << jack->sysxin->length();
    qDebug() << "ENCERROU #" << jack->sysxin->length();

}
*/

void PatchUI::onSysExRequestFinished(){
//a cada loop recebo: common, depois tone 0, 1, 2 e 3
//    qDebug() << "Fim da RQ. total# " << jack->sysxin->length();
//    jack->sysxin->clear();
}
