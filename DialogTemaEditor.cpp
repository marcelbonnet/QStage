#include "DialogTemaEditor.h"
#include "ui_DialogTemaEditor.h"
#include "Controller.h"

DialogTemaEditor::DialogTemaEditor(MainWindow *w, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTemaEditor)
{
    ui->setupUi(this);

    this->w = w;

    QMap<QString, QString> temas = Controller::queryTemasUI();
    for(int i=0; i < temas.count(); i++){

        QString nomeTema = temas.keys().at(i);
        QString css = temas.value(temas.keys().at(i));

        ui->cmbTemas->addItem(nomeTema, css);

    }

}


DialogTemaEditor::~DialogTemaEditor()
{
    delete ui;
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
