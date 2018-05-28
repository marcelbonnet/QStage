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

        if(QString::compare(categoria, QString(""), Qt::CaseInsensitive) == 0 || QString::compare(categoria, patchObjects[i]->categoria, Qt::CaseInsensitive ) == 0 )
            patch->addItem(patches[i], QVariant::fromValue(patchObjects[i]));
    }
    connect(patch,SIGNAL(currentIndexChanged(int)), this
               , SLOT(on_patch_currentIndexChanged(int)));
}

PartTab::~PartTab()
{
    delete ui;
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
    if(ui->btnLocal->isChecked())
        ui->btnLocal->setText("LIGADO");
    else
        ui->btnLocal->setText("DESLIGADO");

    enviar();

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
    carregarPatches("BEL");
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
