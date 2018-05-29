#include "parttab.h"
#include "ui_parttab.h"
#include <QComboBox>
#include "patch.h"
#include <QDebug>
#include <QException>
#include <QList>

PartTab::PartTab(int parte, MidiControl *jack, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PartTab)
{
    ui->setupUi(this);

    this->jack = jack;
    this->parte = parte;
    tempoUltimoEnvio = QDateTime::currentDateTime();

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
            qDebug() << patches[i];
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

void PartTab::enviar(){
    //intervalo mínimo de tempo entre envios para não estourar o ringbuffer
    QDateTime agora = QDateTime::currentDateTime();
    qint64 intervaloMinimo = 50;

    if(tempoUltimoEnvio.msecsTo(agora) <= intervaloMinimo)
        return;

    QList<int> *dados = new QList<int>();
    dados->append(ui->canal->value());

    Patch *patch = ui->patch->itemData(ui->patch->currentIndex()).value<Patch*>();
    dados->append(patch->groupType);
    dados->append(patch->groupId);
    dados->append(patch->number);

    dados->append(ui->level->value());
    dados->append(ui->pan->value());
    dados->append(ui->afinacaoBruta->value() +48);
    dados->append(ui->afinacaoFina->value() +50);
    dados->append(ui->saida->currentIndex());
    dados->append(ui->sendLevel->value());
    dados->append(ui->chorus->value());
    dados->append(ui->reverb->value());

    //receive midis:
    dados->append(1);
    dados->append(1);
    dados->append(1);

    dados->append(ui->minimo->currentIndex());
    dados->append(ui->maximo->currentIndex());
    dados->append(ui->oitava->value() + 3);
    dados->append(ui->btnLocal->isChecked()? 1 : 0);

    jack->setPerformancePart(parte, dados);
    tempoUltimoEnvio = QDateTime::currentDateTime();
}

void PartTab::on_patch_currentIndexChanged(int index)
{
    enviar();
}



void PartTab::on_btnLocal_clicked()
{
//    if(ui->btnLocal->isChecked())
//        ui->btnLocal->setText("LIGADO");
//    else
//        ui->btnLocal->setText("DESLIGADO");

    enviar();

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
    enviar();
}

void PartTab::on_sendLevel_valueChanged(int value)
{
    enviar();
}

void PartTab::on_reverb_valueChanged(int value)
{
    enviar();
}

void PartTab::on_chorus_valueChanged(int value)
{
    enviar();
}

void PartTab::on_pan_valueChanged(int value)
{
    enviar();
}

void PartTab::on_canal_valueChanged(int arg1)
{
    enviar();
}

void PartTab::on_minimo_currentIndexChanged(int index)
{
    enviar();
}

void PartTab::on_maximo_currentIndexChanged(int index)
{
    enviar();
}

void PartTab::on_oitava_valueChanged(int arg1)
{
    enviar();
}

void PartTab::on_afinacaoBruta_valueChanged(int arg1)
{
    enviar();
}

void PartTab::on_afinacaoFina_valueChanged(int arg1)
{
    enviar();
}

void PartTab::on_saida_currentIndexChanged(int index)
{
    enviar();
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
