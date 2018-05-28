#include "dialogjack.h"
#include "ui_dialogjack.h"
#include <QList>
#include <QString>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QImage>

DialogJack::DialogJack(MidiControl *jack, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogJack)
{
    ui->setupUi(this);

    this->jack = jack;
    QList<QString> * portas = jack->listarPortas();

    for(int i=0; i < portas->length(); i++)
        ui->portas->addItem(portas->at(i));

    QGraphicsScene *scene =  new QGraphicsScene();
    QPixmap pixmap(":/icones/QStage.png");
    scene->addPixmap(pixmap);
    ui->img->setScene(scene);
    ui->img->show();

}

DialogJack::~DialogJack()
{
    delete ui;
}

void DialogJack::on_buttonBox_accepted()
{
    jack->conectarNaPorta(ui->portas->currentText());
    this->close();
}

void DialogJack::on_buttonBox_rejected()
{
    this->hide();
}
