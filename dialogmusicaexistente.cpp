#include "dialogmusicaexistente.h"
#include "ui_dialogmusicaexistente.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "Mainwindow.h"
#include <QDebug>
#include <QMessageBox>

DialogMusicaExistente::DialogMusicaExistente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMusicaExistente)
{
    ui->setupUi(this);
}

DialogMusicaExistente::~DialogMusicaExistente()
{
    delete ui;
}

void DialogMusicaExistente::showEvent(QShowEvent *){
    try
    {
        ui->comboBox->clear();
        SQLite::Database db(MainWindow::getQStageDatabase().toLatin1().data()
                            , SQLite::OPEN_READONLY);
        SQLite::Statement   query(db, "SELECT musica_id, artista, titulo FROM musicas ORDER BY artista ASC, titulo ASC");

        while(query.executeStep()){
            int id = query.getColumn(0);
            QString nome = QString(query.getColumn(1)) + ": " + QString(query.getColumn(2));
            ui->comboBox->addItem(nome, QVariant::fromValue(id));
        }

    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao obter músicas", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void DialogMusicaExistente::on_buttonBox_accepted()
{
    emit musicaExistenteSelecionada(ui->comboBox->currentData().toInt());
    this->hide();
}

void DialogMusicaExistente::on_buttonBox_rejected()
{
    this->hide();
}
