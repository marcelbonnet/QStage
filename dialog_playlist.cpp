#include "dialog_playlist.h"
#include "ui_dialog_playlist.h"

dialog_playlist::dialog_playlist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_playlist)
{
    ui->setupUi(this);
}

dialog_playlist::~dialog_playlist()
{
    delete ui;
}

void dialog_playlist::on_btnOk_clicked()
{
    if(getId() == 0)
        emit registrarPlaylist(ui->lineEdit->text());
    else
        emit alterarPlaylist(getId(), ui->lineEdit->text());
    this->close();

    setId(0);
    setNome("");
}

void dialog_playlist::on_btnCancel_clicked()
{
    this->close();
}

QString dialog_playlist::getNome() const
{
    return ui->lineEdit->text();
}

void dialog_playlist::setNome(const QString &value)
{
    ui->lineEdit->setText(value);
    nome = value;
}

int dialog_playlist::getId() const
{
    return id;
}

void dialog_playlist::setId(int value)
{
    id = value;
}
