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
#include "defaults.h"
#include "QStageException.h"

PartTab::PartTab(int parte, MidiControl *jack, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PartTab)
{
    ui->setupUi(this);

    this->jack = jack;
    this->parte = parte;

    int oitava = -1;
    int nota=0;
    for(int i=0; i<=127; i++){
        QString notaOit = QString("%1 %2").arg(Defaults::notas()[nota]).arg(oitava) ;
        ui->minimo->addItem(notaOit, QVariant::fromValue(nota*oitava) );
        ui->maximo->addItem(notaOit, QVariant::fromValue(nota*oitava) );
        nota++;
        if(nota>11) {nota=0; oitava++;}
    }



    ui->minimo->setCurrentIndex(0);
    ui->maximo->setCurrentIndex(127);


    this->carregarPatches();

    //Utils: copiar esta part para outra, exceto part 10 (RHYTHM)
    for(int i=1; i<=16; i++){
        if(i == parte || i == 10) continue;
        ui->partUtilDestino->addItem(QString::number(i), QVariant::fromValue(i));
    }

}

void PartTab::conectarWidgets(){
    connect(ui->patch,SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchChanged(int)));
    connect(ui->oitava,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->minimo,SIGNAL(currentIndexChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->maximo,SIGNAL(currentIndexChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->canal,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->afinacaoFina,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->afinacaoBruta,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->level,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->pan,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->reverb,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->chorus,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->sendLevel,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->saida,SIGNAL(currentIndexChanged(int)), this, SLOT(onPartChanged(int)));
    connect(ui->btnLocal,SIGNAL(clicked()), this, SLOT(onPartChanged()));

}

void PartTab::desconectarWidgets(){
    disconnect(ui->patch,SIGNAL(currentIndexChanged(int)), this, SLOT(onPatchChanged(int)));
    disconnect(ui->oitava,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->minimo,SIGNAL(currentIndexChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->maximo,SIGNAL(currentIndexChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->canal,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->afinacaoFina,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->afinacaoBruta,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->level,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->pan,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->reverb,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->chorus,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->sendLevel,SIGNAL(valueChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->saida,SIGNAL(currentIndexChanged(int)), this, SLOT(onPartChanged(int)));
    disconnect(ui->btnLocal,SIGNAL(clicked()), this, SLOT(onPartChanged()));
}

void PartTab::enviarPacote(){
    QList<int> *dados = new QList<int>();
    int part0 = parte - 1;
    Patch *patch = ui->patch->currentData().value<Patch*>();

    int num2 = patch->getNumber() & 0xF;
    int num1 = (patch->getNumber() >> 4) & 0xF;

    //se o Patche foi feito no QStage, é preciso salvá-lo no Patch::USER primeiro
    if(!patch->isRoland()){
        //carregar o patch e salvar entre USER:112 e USER:128 (inclusive)
        try {
            QList<QString>* patchData = Controller::getPatch(patch->id);

            int addr = 0x10000000 + (( (112 + part0) << 16));
            qDebug() << "PATCH QStage" << QString("%1 %2").arg(addr, 0, 16).arg(addr + 0x00001000, 0, 16);
            jack->txPacoteDataSetString(addr + 0x00001000, patchData->at(1));
            jack->txPacoteDataSetString(addr + 0x00001200, patchData->at(2));
            jack->txPacoteDataSetString(addr + 0x00001400, patchData->at(3));
            jack->txPacoteDataSetString(addr + 0x00001600, patchData->at(4));
            jack->txPacoteDataSetString(addr, patchData->at(0));//common
        } catch (SQLite::Exception &e) {
            qDebug() << e.what();
        }
        //aguardar um tempo para o XP-30 processar ?
        usleep(100000);
    }

    //conferir os bytes hardcoded com os da versão anterior que envia byte a byte
    qDebug() << "ENVIO DO PERFORMANCE PART #" << parte << QString("%1").arg(0x02000000 + ((part0 << 8) | 0x1000),0,16);
    dados->append(1);//Receive Switch
    dados->append(ui->canal->value()-1);
    dados->append(patch->getGroupType());
    dados->append(patch->getGroupId());
    dados->append(num1);
    dados->append(num2);

    dados->append(ui->level->value());
    dados->append(ui->pan->value());
    dados->append(ui->afinacaoBruta->value()+48);
    dados->append(ui->afinacaoFina->value()+50);
    dados->append(ui->saida->currentIndex());
    dados->append(ui->sendLevel->value());
    dados->append(ui->chorus->value());
    dados->append(ui->reverb->value());
    dados->append(0);//Receive program change
    dados->append(1);//Receive volume switch
    dados->append(1);//Receive hold-1 switch
    dados->append(ui->minimo->currentIndex());
    dados->append(ui->maximo->currentIndex());
    dados->append(ui->oitava->value()+3);
    dados->append(ui->btnLocal->isChecked()? 1 : 0);
    dados->append(1);//Transmit switch
    dados->append(0);//Transmit bank select group
    dados->append(8);//Transmit volume (2 bytes)
    dados->append(0);//Transmit volume (2 bytes)

    jack->txPacoteDataSet(0x01000000 + ((part0 << 8) | 0x1000), dados );
}

void PartTab::onPartChanged(){
    enviarPacote();
}

void PartTab::onPartChanged(int i){
    enviarPacote();
}


void PartTab::carregarPatches(QString categoria){
    QComboBox *patchui = ui->patch;

    desconectarWidgets();
    patchui->clear();
    for(Patch* patch : *Controller::queryPatches()){
        if(QString::compare(categoria, QString(""), Qt::CaseInsensitive) == 0
                || QString::compare(categoria, patch->categoria, Qt::CaseInsensitive ) == 0 ){
            patchui->addItem(patch->getFullName(), QVariant::fromValue(patch));

        }
    }
    conectarWidgets();
}

PartTab::~PartTab()
{
    delete ui;
}

void PartTab::setPatchSelected(int patchId) noexcept(false){
    QComboBox *patchui = ui->patch;
    desconectarWidgets();
    bool achou = false;
    for(int i=0; i<patchui->count(); i++){
        Patch *patch = patchui->itemData(i).value<Patch*>();
        if(patchId == patch->id ){
            patchui->setCurrentIndex(i);
            achou = true;
            break;
        }
    }
    conectarWidgets();
    if(!achou)
        throw new QStageException(QString("Patch ID \"%1\" não existe na lista.").arg(patchId));
}


void PartTab::onPatchChanged(int index){
    enviarPacote();
}



Patch *PartTab::getPatch(){
    Patch *patch = ui->patch->itemData(ui->patch->currentIndex()).value<Patch*>();
    return patch;
}

int PartTab::getPatchIndex(){
    return ui->patch->currentIndex();
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

int PartTab::getVoiceReserve(){
    return ui->voiceReserve->value();
}

void PartTab::setVoiceReserve(int i){
    ui->voiceReserve->setValue(i);
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

void PartTab::on_partUtilDestinoBtn_clicked()
{
    emit partUtilsCopiarPerformancePart(
                //origem
                parte,
                //destino
                ui->partUtilDestino->currentData().toInt()
                );
}
