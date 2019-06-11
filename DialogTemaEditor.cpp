#include "DialogTemaEditor.h"
#include "ui_DialogTemaEditor.h"
#include "Controller.h"
#include "QStageException.h"

#include <QMessageBox>

DialogTemaEditor::DialogTemaEditor(MainWindow *w, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTemaEditor)
{
    ui->setupUi(this);

    this->w = w;

    carregarTemas();

}


DialogTemaEditor::~DialogTemaEditor()
{
    delete ui;
}

void DialogTemaEditor::carregarTemas(){
    QMap<QString, QString> temas = Controller::queryTemasUI();

    ui->cmbTemas->clear();
    for(int i=0; i < temas.count(); i++){
        QString nomeTema = temas.keys().at(i);
        QString css = temas.value(temas.keys().at(i));
        ui->cmbTemas->addItem(nomeTema, css);
    }
}

void DialogTemaEditor::on_pushButton_3_clicked()
{
    w->setStyleSheet(ui->estilo->document()->toPlainText());
}

void DialogTemaEditor::on_cmbTemas_currentIndexChanged(int index)
{
    ui->estilo->document()->setPlainText( ui->cmbTemas->currentData().toString() );
    ui->nomeTema->setText(ui->cmbTemas->currentText());
}

void DialogTemaEditor::on_pushButton_clicked()
{
    try {
        Controller::persistTema(ui->nomeTema->text(), ui->estilo->document()->toPlainText());
    } catch (QStageException e) {
        QMessageBox::warning(this,"Erro ao persistir tema", e.getMessage() + "\n" + e.what());
    } catch (SQLite::Exception e){
        QMessageBox::warning(this,"Erro ao persistir tema", e.what());
    }
}

void DialogTemaEditor::on_btnRemover_clicked()
{
    QString key = ui->cmbTemas->currentText();
    QMessageBox::StandardButton resposta = QMessageBox::question(this, "Remover", "Remover o tema \""+ key + "\"?", QMessageBox::Yes | QMessageBox::No);
    if(resposta != QMessageBox::Yes)
        return;

    try {
        Controller::deleteTema(key);
        carregarTemas();
    } catch (SQLite::Exception e) {
        QMessageBox::warning(this,"Erro ao persistir tema", e.what());
    }
}
