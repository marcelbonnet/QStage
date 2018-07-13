#include "dialogmusica.h"
#include "ui_dialogmusica.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "Mainwindow.h"
#include <QMessageBox>
#include <QDebug>

DialogMusica::DialogMusica(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMusica)
{
    ui->setupUi(this);
}

DialogMusica::~DialogMusica()
{
    delete ui;
}

void DialogMusica::showEvent(QShowEvent *){
    try
    {
        ui->comboBox->clear();
        SQLite::Database db(MainWindow::getQStageDatabase().toLatin1().data()
                            , SQLite::OPEN_READONLY);
        SQLite::Statement   query(db, "SELECT id, nome, tipo FROM modelos ORDER BY nome ASC");

        while(query.executeStep()){
            int id = query.getColumn(0);
            QString nome = QString(query.getColumn(1));
            QString tipo = QString(query.getColumn(2));
            ui->comboBox->addItem(nome + " [" + tipo + "]", QVariant::fromValue(id));
        }


        if(this->modeloId > 0){
            int index = ui->comboBox->findData(QVariant::fromValue(this->modeloId));
            ui->comboBox->setCurrentIndex(index);
        }
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao obter templates", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void DialogMusica::on_buttonBox_accepted()
{
    if(getModoInclusao())
        emit incluirMusica(getTitulo());
    else
        emit alterarMusica(getTitulo());

    setModoInclusao(true);
    this->close();
}

void DialogMusica::on_buttonBox_rejected()
{
    this->close();
}

int DialogMusica::getModeloId()
{
    modeloId = ui->comboBox->currentData().toInt();
    return modeloId;
}

void DialogMusica::setModeloId(int value)
{
    modeloId = value;
}

bool DialogMusica::getModoInclusao() const
{
    return modoInclusao;
}

void DialogMusica::setModoInclusao(bool value)
{
    modoInclusao = value;
}

QString DialogMusica::getTitulo() const
{
    return ui->lineEdit->text();
}

void DialogMusica::setTitulo(const QString &value)
{
    ui->lineEdit->setText(value);
    titulo = value;
}
