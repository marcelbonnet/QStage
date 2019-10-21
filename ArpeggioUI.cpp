#include "ArpeggioUI.h"
#include "ui_ArpeggioUI.h"
#include <QDebug>

ArpeggioUI::ArpeggioUI(MidiControl *jack, QSpinBox *performanceTempo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArpeggioUI)
{
    ui->setupUi(this);

    this->jack = jack;
    this->performanceTempo = performanceTempo;
    /*
     * Carga inicial de Styles no Combo
     */
    for(Arpeggio* arp : *Controller::queryArpeggioStyles() ){
        ui->style->addItem(arp->label, QVariant::fromValue(arp));
    }
    carregarMotifs(ui->style->currentText());
    ui->tempo->setValue(performanceTempo->value());

    conectarWidgets();
}

ArpeggioUI::~ArpeggioUI()
{
    delete ui;
}

void ArpeggioUI::onArpeggioChanged(int i){
    desconectarWidgets();
    ui->tempo->setValue(performanceTempo->value());
    conectarWidgets();
    enviarDataSet();
}

void ArpeggioUI::conectarWidgets(){
    //se o tempo do Performance mudar:
    connect(performanceTempo, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));

    connect(ui->motif,  SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->part, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->style,  SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->octave, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->beatPattern, SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->accent, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->shuffle, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->velocity, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->tempo, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));

}

void ArpeggioUI::desconectarWidgets(){
    disconnect(performanceTempo, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));

    disconnect(ui->part, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->style, SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->octave, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->motif, SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->beatPattern, SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->accent, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->shuffle, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->velocity, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->tempo, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
}

void ArpeggioUI::carregarMotifs(QString styleName){
    desconectarWidgets();
    ui->motif->clear();
    for(Arpeggio* arp : *Controller::queryArpeggioMotif(styleName) ){
        ui->motif->addItem(arp->label, QVariant::fromValue(arp));
    }
    conectarWidgets();
}

void ArpeggioUI::carregarBeatPatterns(QString styleName){
    desconectarWidgets();
    ui->beatPattern->clear();
    for(Arpeggio* arp : *Controller::queryArpeggioBeatPattern(styleName) ){
        ui->beatPattern->addItem(arp->label, QVariant::fromValue(arp));
    }
    conectarWidgets();
}

void ArpeggioUI::on_style_currentIndexChanged(int index)
{

    carregarMotifs(ui->style->currentText());
    carregarBeatPatterns(ui->style->currentText());

    /*
     * Após carregar os valores válidos nas combos de Motif e Beat Pattern
     * é preciso enviar imediatamente a nova seleção para que
     * não ocorra algum efeito inesperado no Teclado - o próprio teclado vai
     * setar um valor válido depois de receber um novo Style, então vou
     * enviar um valor que seja conhecido para mim (o primeiro item de cada combo)
     */
    enviarDataSet();
}

void ArpeggioUI::enviarDataSet(){
    QList<int> *dados = new QList<int>();
    int addr = 0x00000058;

    Arpeggio* style         = ui->style->currentData(Qt::UserRole).value<Arpeggio*>();
    Arpeggio* motif         = ui->motif->currentData(Qt::UserRole).value<Arpeggio*>();
    Arpeggio* beatPattern   = ui->beatPattern->currentData(Qt::UserRole).value<Arpeggio*>();

    //Arpeggio
    dados->append(style->value);
    dados->append(motif->value);
    dados->append(beatPattern->value);
    dados->append(ui->accent->value());
    dados->append(ui->shuffle->value());
    dados->append(ui->velocity->value());
    dados->append(ui->octave->value() +3);
    dados->append(ui->part->value() -1);
    /*
     * ============ System Tempo ================
     *  o Arpeggio não usa o Performance Tempo,
     * então vou deixá-lo igual ao do Performance
     */
    int tempo = performanceTempo->value();
    int tempo2 = tempo & 0xF;
    int tempo1 = (tempo >> 4) & 0xF;
    dados->append(tempo1);
    dados->append(tempo2);
    //============================================

    jack->txPacoteDataSet(addr, dados);
}

void ArpeggioUI::on_tempo_valueChanged(int arg1)
{
    emit arpeggioTempoChanged(arg1);
}

void ArpeggioUI::setPart(int i){ ui->part->setValue(i); }
void ArpeggioUI::setStyle(int index){
    ui->style->setCurrentIndex(index);
}

void ArpeggioUI::setMotif(int index){
    carregarMotifs(ui->style->currentText());
    ui->motif->setCurrentIndex(index);
}

void ArpeggioUI::setBeatPattern(int index){
    carregarBeatPatterns(ui->style->currentText());
    ui->beatPattern->setCurrentIndex(index);
}
void ArpeggioUI::setOctave(int i){ui->octave->setValue(i); }
void ArpeggioUI::setAccent(int i){ ui->accent->setValue(i);}
void ArpeggioUI::setShuffle(int i){ui->shuffle->setValue(i); }
void ArpeggioUI::setVelocity(int i){ui->velocity->setValue(i); }
//void ArpeggioUI::setSwitch(bool b){ ui->btnSwitch->setChecked(b); }

//bool ArpeggioUI::isSwitchOn(){ ui->btnSwitch->isChecked(); }
int ArpeggioUI::getPart(){ return ui->part->value(); }
int ArpeggioUI::getStyle(){return ui->style->currentIndex(); }
int ArpeggioUI::getMotif(){return ui->motif->currentIndex(); }
int ArpeggioUI::getBeatPattern(){return ui->beatPattern->currentIndex(); }
int ArpeggioUI::getOctave(){return ui->octave->value(); }
int ArpeggioUI::getAccent(){return ui->accent->value(); }
int ArpeggioUI::getShuffle(){return ui->shuffle->value(); }
int ArpeggioUI::getVelocity(){return ui->velocity->value(); }
