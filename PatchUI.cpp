#include "PatchUI.h"
#include "ui_PatchUI.h"
#include <QIcon>
#include <QPixmap>
#include "defaults.h"

PatchUI::PatchUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatchUI)
{
    ui->setupUi(this);

    ui->structure12->setMinimumSize(QSize(250,60));
    ui->structure34->setMinimumSize(QSize(250,60));
    for(int i=1; i<=10; i++){
        QPixmap pixmap(QString(":/icones/struct-type%1.png").arg(i));
        QIcon icon(pixmap);
        ui->structure12->addItem(icon, QString::number(i));
        ui->structure12->setIconSize(QSize(250,60));
        ui->structure34->addItem(icon, QString::number(i));
        ui->structure34->setIconSize(QSize(250,60));
    }

    QComboBox *combo = ui->patchCategory;
    combo->setMinimumSize(QSize(150,60));
    for(int i=0; i<39; i++){
        QIcon icon(QString(":/icones/icons/%1.ico").arg(i));
        combo->addItem(icon, categorias[i]);
        combo->setIconSize(QSize(60,60));
    }
}

PatchUI::~PatchUI()
{
    delete ui;
}
