#include "parttab.h"
#include "ui_parttab.h"
#include <QComboBox>
#include "patch.h"
#include <QDebug>
#include <QException>
#include <QList>
#include "SysExMessage.h"
#include "PerformancePart.h"
#include "baseaddress.h"

PartTab::PartTab(int parte, MidiControl *jack, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PartTab)
{
    ui->setupUi(this);

    this->jack = jack;
    this->parte = parte;
//    tempoUltimoEnvio = QDateTime::currentDateTime();

    QString notas[12] = {
        "C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
    };

    for(int oitava=-1; oitava<=9; oitava++){
        for(int nota=0; nota<12 && nota*oitava <= 127  ; nota++){
            QString notaOit = QString("%1 %2").arg(notas[nota]).arg(oitava) ;
            ui->minimo->addItem(notaOit, QVariant::fromValue(nota*oitava) );
            ui->maximo->addItem(notaOit, QVariant::fromValue(nota*oitava) );
        }
    }
    ui->maximo->removeItem(131);
    ui->maximo->removeItem(130);
    ui->maximo->removeItem(129);
    ui->maximo->removeItem(128);

    ui->minimo->setCurrentIndex(0);
    ui->maximo->setCurrentIndex(127);

    /*
     * PATCHES
     */
    for(int i=0; i < 1406; i++ ){
        patchObjects[i] = new Patch();
        QRegularExpression re("\t");
        patchObjects[i]->nome = patches[i].replace(re," ");
        patchObjects[i]->categoria = categorias[i];
        patchObjects[i]->categoriaPai = categoriasPai[i];
        patchObjects[i]->groupType = groupType[i];
        patchObjects[i]->groupId = groupId[i];
        patchObjects[i]->number = number[i];
    }
    this->carregarPatches();

}
void PartTab::carregarPatches(QString categoria){
    QComboBox *patch = ui->patch;
    //Patch * p[1406];
    disconnect(patch,SIGNAL(currentIndexChanged(int)), this
               , SLOT(on_patch_currentIndexChanged(int)));
    patch->clear();
    for(int i=0; i < 1406; i++ ){
//        patchObjects[i] = new Patch();
//        QRegularExpression re("\t");
//        patchObjects[i]->nome = patches[i].replace(re," ");
//        patchObjects[i]->categoria = categorias[i];
//        patchObjects[i]->categoriaPai = categoriasPai[i];
//        patchObjects[i]->groupType = groupType[i];
//        patchObjects[i]->groupId = groupId[i];
//        patchObjects[i]->number = number[i];

        if(QString::compare(categoria, QString(""), Qt::CaseInsensitive) == 0 || QString::compare(categoria, patchObjects[i]->categoria, Qt::CaseInsensitive ) == 0 ){
            patch->addItem(patches[i], QVariant::fromValue(patchObjects[i]));
            //qDebug() << patches[i];
        }
    }
    connect(patch,SIGNAL(currentIndexChanged(int)), this
               , SLOT(on_patch_currentIndexChanged(int)));
}

PartTab::~PartTab()
{
    delete ui;
}

int PartTab::getIndexFromPatches(QString nome){
    for(int i=0; i<1406; i++){
        if( QString::compare(patches[i], nome) == 0 ){
            return i;
        }
    }
    qDebug() << "Nao achei o nome na lista de patches: " << nome;
    return -1;
}

void PartTab::enviarMensagem(PerformancePart::Function function, int data){
    QList<SysExMessage*> *dados = new QList<SysExMessage*>();
    PerformancePart pp = PerformancePart(function, parte);
    dados->append(
                new SysExMessage(
                    BaseAddress(BaseAddress::TempPerformance),
                    pp,
                    data));
    qDebug() << QString("Enviando Parte %1 -> %2 = %3").arg(parte).arg(pp.functionName).arg(QString::number(data,16));
    jack->tx(dados);
}

void PartTab::enviar(){

    qDebug() << QString("====> Enviando todas as mensagens de Performance Part #%1").arg(parte);


    QList<SysExMessage*> *dados = new QList<SysExMessage*>();
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::MIDIChannel, parte),ui->canal->value() -1 ));

    Patch *patch = ui->patch->itemData(ui->patch->currentIndex()).value<Patch*>();
    if(ui->patch->currentIndex() > -1){
        qDebug() << "enviando PATCH" << QString::number(patch->groupType,16) << QString::number(patch->groupId, 16) << QString::number(patch->number,16);
        dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::PatchGroupType, parte),patch->groupType));
        dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::PatchGroupID, parte),patch->groupId));
        dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::PatchNumber, parte),patch->number));
    }
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::PartLevel, parte),ui->level->value()));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::PartPan, parte),ui->pan->value()));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::PartCoarseTune, parte),ui->afinacaoBruta->value() +48));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::PartFineTune, parte),ui->afinacaoFina->value() +50));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::OutputAssign, parte),ui->saida->currentIndex()));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::MixEFXSendLevel, parte),ui->sendLevel->value()));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::ChorusSendLevel, parte),ui->chorus->value()));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::ReverbSendLevel, parte),ui->reverb->value()));

    //receive midis:
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::ReceiveProgramChangeSwitch, parte),1));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::ReceiveVolumeSwitch, parte),1));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::ReceiveHold1Switch, parte),1));

    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::KeyboardRangeLower, parte),ui->minimo->currentIndex()));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::KeyboardRangeUpper, parte),ui->maximo->currentIndex()));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::OctaveShift, parte),ui->oitava->value() + 3));
    dados->append(new SysExMessage( BaseAddress(BaseAddress::TempPerformance), PerformancePart(PerformancePart::LocalSwitch, parte),ui->btnLocal->isChecked()? 1 : 0));

    jack->tx(dados);


//    tempoUltimoEnvio = QDateTime::currentDateTime();
}

void PartTab::on_patch_currentIndexChanged(int index)
{
	Patch *patch = ui->patch->itemData(ui->patch->currentIndex()).value<Patch*>();
    if(ui->patch->currentIndex() > -1){
       qDebug() << "enviando PATCH" << QString::number(patch->groupType,16) << QString::number(patch->groupId, 16) << QString::number(patch->number,16);
       enviarMensagem( PerformancePart::PatchGroupType, patch->groupType);
       enviarMensagem( PerformancePart::PatchGroupID, patch->groupId);
       enviarMensagem( PerformancePart::PatchNumber, patch->number);
    }
}



void PartTab::on_btnLocal_clicked()
{
//    if(ui->btnLocal->isChecked())
//        ui->btnLocal->setText("LIGADO");
//    else
//        ui->btnLocal->setText("DESLIGADO");

	enviarMensagem(PerformancePart::LocalSwitch, ui->btnLocal->isChecked()? 1 : 0 );
}

Patch *PartTab::getPatch(){
    Patch *patch = ui->patch->itemData(ui->patch->currentIndex()).value<Patch*>();
    return patch;
}

int PartTab::getRegiaoMin(){
    return ui->minimo->currentIndex();
}

int PartTab::getRegiaoMax(){
    return ui->maximo->currentIndex();
}

int PartTab::getOitava(){
    return ui->oitava->value();
}

int PartTab::getCanalMidi(){
    return ui->canal->value();
}

int PartTab::getSaida(){
    return ui->saida->currentIndex();
}

int PartTab::getAfinacaoBruta(){
    return ui->afinacaoBruta->value();
}

int PartTab::getAfinacaoFina(){
    return ui->afinacaoFina->value();
}

int PartTab::getVolume(){
    return ui->level->value();
}

int PartTab::getPan(){
    return ui->pan->value();
}

int PartTab::getMixEfxLevel(){
    return ui->sendLevel->value();
}

int PartTab::getChorusLevel(){
    return ui->chorus->value();
}

int PartTab::getReverbLevel(){
    return ui->reverb->value();
}

int PartTab::isLocalOn(){
    return ui->btnLocal->isChecked() ? 1 : 0;
}
/*
void PartTab::setPatch(Patch *p){
    for(int i=0; i<1406; i++){
        Patch *corrente = ui->patch->itemData(i).value<Patch*>();
        if(p->groupId == corrente->groupId &&
                p->groupType == corrente->groupType &&
                p->number == corrente->number &&
                QString::compare(p->nome, corrente->nome) == 0){
            ui->patch->setCurrentIndex(i);
            break;
        }
    }
}
*/
void PartTab::setPatch(int i){
    ui->patch->setCurrentIndex(i);
}

void PartTab::setRegiaoMin(int i){
    ui->minimo->setCurrentIndex(i);
}
void PartTab::setRegiaoMax(int i){
    ui->maximo->setCurrentIndex(i);
}
void PartTab::setOitava(int i){
    ui->oitava->setValue(i);
}
void PartTab::setCanalMidi(int i){
    ui->canal->setValue(i);
}
void PartTab::setSaida(int i){
    ui->saida->setCurrentIndex(i);
}
void PartTab::setAfinacaoBruta(int i){
    ui->afinacaoBruta->setValue(i);
}
void PartTab::setAfinacaoFina(int i){
    ui->afinacaoFina->setValue(i);
}
void PartTab::setVolume(int i){
    ui->level->setValue(i);
}
void PartTab::setPan(int i){
    ui->pan->setValue(i);
}
void PartTab::setMixEfxLevel(int i){
    ui->sendLevel->setValue(i);
}
void PartTab::setChorusLevel(int i){
    ui->chorus->setValue(i);
}
void PartTab::setReverbLevel(int i){
    ui->reverb->setValue(i);
}
void PartTab::setLocalOn(int i){
    ui->btnLocal->setChecked( i == 1 ? true : false);
}

void PartTab::on_level_valueChanged(int value)
{
    enviarMensagem(PerformancePart::PartLevel, ui->level->value() );
}

void PartTab::on_sendLevel_valueChanged()
{
    enviarMensagem(PerformancePart::MixEFXSendLevel, ui->sendLevel->value() );
    //issue #18
    on_chorus_valueChanged();
}

void PartTab::on_reverb_valueChanged()
{
    enviarMensagem(PerformancePart::ReverbSendLevel, ui->reverb->value() );
}

void PartTab::on_chorus_valueChanged()
{
    enviarMensagem(PerformancePart::ChorusSendLevel, ui->chorus->value() );
    //issue #18
    on_reverb_valueChanged();
}

void PartTab::on_pan_valueChanged(int value)
{
    enviarMensagem(PerformancePart::PartPan, ui->pan->value() );
}

void PartTab::on_canal_valueChanged(int arg1)
{
    enviarMensagem(PerformancePart::MIDIChannel, ui->canal->value()-1 );
}

void PartTab::on_minimo_currentIndexChanged(int index)
{
    enviarMensagem(PerformancePart::KeyboardRangeLower, ui->minimo->currentIndex() );
    //issue #18
    on_maximo_currentIndexChanged(ui->maximo->currentIndex());

}

void PartTab::on_maximo_currentIndexChanged(int index)
{
    enviarMensagem(PerformancePart::KeyboardRangeUpper, ui->maximo->currentIndex() );
    //issue #18
    on_oitava_valueChanged();

}

void PartTab::on_oitava_valueChanged()
{
    enviarMensagem(PerformancePart::OctaveShift, ui->oitava->value() + 3);
    //issue #18
    on_btnLocal_clicked();
}

void PartTab::on_afinacaoBruta_valueChanged(int arg1)
{
    enviarMensagem(PerformancePart::PartCoarseTune, ui->afinacaoBruta->value() +48 );
}

void PartTab::on_afinacaoFina_valueChanged(int arg1)
{
    enviarMensagem(PerformancePart::PartFineTune, ui->afinacaoFina->value() +50 );
}

void PartTab::on_saida_currentIndexChanged(int index)
{
    enviarMensagem(PerformancePart::OutputAssign, ui->saida->currentIndex() );
    //issue #18
    on_sendLevel_valueChanged();
}

void PartTab::on_btn_clicked()
{
    carregarPatches("PNO");
}

void PartTab::on_btn_2_clicked()
{
    carregarPatches("EP");
}

void PartTab::on_btn_3_clicked()
{
    carregarPatches("KEY");
}

void PartTab::on_btn_4_clicked()
{
    carregarPatches("BELL");
}

void PartTab::on_btn_5_clicked()
{
    carregarPatches("MLT");
}

void PartTab::on_btn_6_clicked()
{
    carregarPatches("ORG");
}

void PartTab::on_btn_8_clicked()
{
    carregarPatches("ACD");
}

void PartTab::on_btn_7_clicked()
{
    carregarPatches("HRM");
}

void PartTab::on_btn_9_clicked()
{
    carregarPatches("AGT");
}

void PartTab::on_btn_10_clicked()
{
    carregarPatches("EGT");
}

void PartTab::on_btn_11_clicked()
{
    carregarPatches("DGT");
}

void PartTab::on_btn_12_clicked()
{
    carregarPatches("BS");
}

void PartTab::on_btn_13_clicked()
{
    carregarPatches("SBS");
}

void PartTab::on_btn_14_clicked()
{
    carregarPatches("STR");
}

void PartTab::on_btn_15_clicked()
{
    carregarPatches("ORC");
}

void PartTab::on_btn_16_clicked()
{
    carregarPatches("HIT");
}

void PartTab::on_btn_17_clicked()
{
    carregarPatches("WND");
}

void PartTab::on_btn_19_clicked()
{
    carregarPatches("FLT");
}

void PartTab::on_btn_18_clicked()
{
    carregarPatches("BRS");
}

void PartTab::on_btn_20_clicked()
{
    carregarPatches("SBR");
}

void PartTab::on_btn_21_clicked()
{
    carregarPatches("SAX");
}

void PartTab::on_btn_22_clicked()
{
    carregarPatches("HLD");
}

void PartTab::on_btn_23_clicked()
{
    carregarPatches("SLD");
}

void PartTab::on_btn_24_clicked()
{
    carregarPatches("TEK");
}

void PartTab::on_btn_25_clicked()
{
    carregarPatches("PLS");
}

void PartTab::on_btn_27_clicked()
{
    carregarPatches("FX");
}

void PartTab::on_btn_26_clicked()
{
    carregarPatches("SYN");
}

void PartTab::on_btn_28_clicked()
{
    carregarPatches("BPD");
}

void PartTab::on_btn_29_clicked()
{
    carregarPatches("SPD");
}

void PartTab::on_btn_30_clicked()
{
    carregarPatches("VOX");
}

void PartTab::on_btn_31_clicked()
{
    carregarPatches("PLK");
}

void PartTab::on_btn_32_clicked()
{
    carregarPatches("ETH");
}

void PartTab::on_btn_33_clicked()
{
    carregarPatches("FRT");
}

void PartTab::on_btn_34_clicked()
{
    carregarPatches("PRC");
}

void PartTab::on_btn_35_clicked()
{
    carregarPatches("SFX");
}

void PartTab::on_btn_36_clicked()
{
    carregarPatches("BTS");
}

void PartTab::on_btn_37_clicked()
{
    carregarPatches("DRM");
}

void PartTab::on_btn_38_clicked()
{
    carregarPatches("CMB");
}
