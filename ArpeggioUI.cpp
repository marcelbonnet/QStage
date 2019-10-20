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
    conectarWidgets();
}

ArpeggioUI::~ArpeggioUI()
{
    delete ui;
}

void ArpeggioUI::onArpeggioChanged(int i){
    //nada para processar. Apenas envie o pacote para o Teclado
    enviarDataSet();
}

void ArpeggioUI::conectarWidgets(){
    connect(ui->motif,  SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));

    connect(ui->part, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->style,  SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->octave, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->beatPattern, SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->accent, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->shuffle, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    connect(ui->velocity, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));

}

void ArpeggioUI::desconectarWidgets(){
    disconnect(ui->part, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->style, SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->octave, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->motif, SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->beatPattern, SIGNAL(currentIndexChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->accent, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->shuffle, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));
    disconnect(ui->velocity, SIGNAL(valueChanged(int)), this, SLOT(onArpeggioChanged(int)));

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
