#include "FormQStudio.h"
#include "ui_FormQStudio.h"
#include <QDebug>

FormQStudio::FormQStudio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormQStudio)
{
    ui->setupUi(this);
}

FormQStudio::~FormQStudio()
{
    delete ui;
}

void FormQStudio::playMIDIFile(QString file){
    qDebug() << "MIDI FILE" << file;
    smf_t *smf = smf_load(file.toUtf8());

}

void FormQStudio::on_pushButton_clicked()
{
    playMIDIFile("/studio/OLD/Estudo/Paul\ McCartney\ MIDIs/LondonTown.mid");
}
