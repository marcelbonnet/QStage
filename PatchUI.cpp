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

PatchUI::PatchUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatchUI)
{
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
    conectarWidgets();
}

PatchUI::~PatchUI()
{
    delete ui;
}

void PatchUI::enviarMensagem(enum PatchTone::Function func, int data){
    qDebug() << "ENVIAR: " << func << QString::number(data);
}

void PatchUI::conectarWidgets()
{
    //deve ser foreach QWidget, lista
    //teste
    for(int i=0; i<toneSwitchList->count(); i++)
        connect(toneSwitchList->at(i),SIGNAL(clicked()), this, SLOT(onPatchToneChanged()));

}

void PatchUI::desconectarWidgets(){

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
    qDebug() << v;
    qDebug() << v.value<PatchTone*>();
//    PatchTone *patchTone = qobject_cast<PatchTone*>(w->property("function"));
    PatchTone *patchTone = w->property("function").value<PatchTone*>();
    enviarMensagem( patchTone->function, w->isChecked()? 1 : 0 );

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
        toneSwitch->setProperty("function", QVariant::fromValue(new PatchTone(PatchTone::Tone_Switch)));

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
        setSliderRange(filterEnvelopeDepth, -63,63);
        filterEnvelopeDepth->setValue(0);
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
        setSliderRange(filterLfo1Depth,-63,63);
        filterLfo1Depth->setValue(0);
        setSliderRange(filterLfo2Depth,-63,63);
        filterLfo2Depth->setValue(0);
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


