#include "FormSerialMidi.h"
#include "ui_FormSerialMidi.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFrame>
#include <QStringList>
#include <QLabel>
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>


FormSerialMidi::FormSerialMidi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSerialMidi)
{
    ui->setupUi(this);

    QString notas[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    QStringList prg;
    prg << "OFF" << "Nota"
        << "Maior" << "menor"
        << "2sus"
        << "4sus";

    QStringList intervalos;
    intervalos << "IIb" << "II"
               << "IIIb" << "III"
               << "IV" << "IV#" << "V"
               << "VI b" << "VI"
               << "VII b" << "VII" << "VIII"
               << "IX" << "XI";

    cmbProgramas = new QList<QComboBox*>();
    cmbIntervalos = new QList<QListWidget*>();

    QVBoxLayout *vbox = new QVBoxLayout(parent);
    QScrollArea *scroll = new QScrollArea();
    QGridLayout *grid = new QGridLayout(scroll);
    QFrame *frame = new QFrame();

    btnIgnoreNoteOff = new QPushButton("Sustain");
    btnIgnoreNoteOff->setCheckable(true);
    //connect(btnIgnoreNoteOff, SIGNAL(clicked()), this, SLOT(onSustain()));

    int y = 0;
    grid->addWidget(btnIgnoreNoteOff, y, 0, 1, 2, Qt::AlignLeft);
    y++;

    for(int i=0; i<12; i++){
        QLabel *l = new QLabel(notas[i]);
        grid->addWidget(l, y, 0, 1, 1, Qt::AlignLeft);

        QComboBox *cmbPrograma = new QComboBox();
        for(int o=0; o<prg.size(); o++)
            cmbPrograma->addItem(prg.at(o));
        grid->addWidget(cmbPrograma,y,1,1,1,Qt::AlignTop);

        cmbProgramas->append(cmbPrograma);

        QListWidget *lstAdds = new QListWidget();
        lstAdds->setSelectionMode(QListWidget::MultiSelection);
        lstAdds->setMaximumHeight(100);
        for(int o=0; o<intervalos.size(); o++){
            QListWidgetItem * item = new QListWidgetItem();
            item->setText(intervalos.at(o));
            item->setData(Qt::UserRole, QVariant::fromValue(o));
            lstAdds->addItem(item);
        }
        grid->addWidget(lstAdds,y,2,1,1,Qt::AlignTop);

        cmbIntervalos->append(lstAdds);

        y++;
    }

    frame->setLayout(grid);
    scroll->setWidget(frame);
    vbox->addWidget(scroll);
    setLayout(vbox);

}

bool FormSerialMidi::ignorarNoteOff(){
    return btnIgnoreNoteOff->isChecked();
}

QList<int> *FormSerialMidi::getNotas(int notaRaiz){
    QList<int> *notas = new QList<int>();

    int prog = cmbProgramas->at(notaRaiz)->currentIndex();
    QList<QListWidgetItem*> itens = cmbIntervalos->at(notaRaiz)->selectedItems();


    switch (prog) {
    case 0: //OFF
        return NULL;
    case 1://NOTA
        notas->append(0);
        return notas;
    case 2:// Maior
        notas->append(4);
        notas->append(7);
        break;
    case 3:// menor
        notas->append(3);
        notas->append(7);
        break;
    case 4:// 2sus
        notas->append(2);
        notas->append(7);
        break;
    case 5:// 4sus
        notas->append(5);
        notas->append(7);
        break;
    }

    for(QListWidgetItem *item : itens){
        int index = item->data(Qt::UserRole).value<int>();

        if(index <=11)
            notas->append(index+1);

        if(index == 12)
            notas->append(14); // intervalo de 9

        if(index == 14)
            notas->append(17); // intervalo de 11

    }

    return  notas;
}

void FormSerialMidi::setBtnIgnorarNoteOff(bool checked){
    btnIgnoreNoteOff->setChecked(checked);
}

void FormSerialMidi::setPrograma(int nota, int index){
    cmbProgramas->at(nota)->setCurrentIndex(index);
}

void FormSerialMidi::setIntervalos(int nota, QList<int> *notas){
    for(int n : *notas){
        qDebug() << n;

        if(n <=12)
            cmbIntervalos->at(nota)->setCurrentRow(n-1);

        if(n == 14)
            cmbIntervalos->at(nota)->setCurrentRow(12);

        if(n == 17)
            cmbIntervalos->at(nota)->setCurrentRow(14);

    }
}

FormSerialMidi::~FormSerialMidi()
{
    delete ui;
}
