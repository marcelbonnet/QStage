#include "PatchUI.h"
#include "ui_PatchUI.h"
#include <QIcon>
#include <QPixmap>
#include "defaults.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QFrame>
#include <QDebug>
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
    QComboBox *comboTone1Categ = ui->tone1WaveGroup;
    QComboBox *comboTone2Categ = ui->tone2WaveGroup;
    QComboBox *comboTone3Categ = ui->tone3WaveGroup;
    QComboBox *comboTone4Categ = ui->tone4WaveGroup;
    combo->setMinimumSize(QSize(150,60));
    for(int i=0; i<39; i++){
        QIcon icon(QString(":/icones/icons/%1.ico").arg(i));
        combo->addItem(icon, categorias[i]);
        combo->setIconSize(QSize(60,60));
        /*
         * Wave group nome e type adicionar do banco de dados
        comboTone1Categ->addItem(icon, categorias[i]);
        comboTone2Categ->addItem(icon, categorias[i]);
        comboTone3Categ->addItem(icon, categorias[i]);
        comboTone4Categ->addItem(icon, categorias[i]);
        */
    }

    QTabWidget *tab = ui->tabWidget_2;
    scrollArea = new QScrollArea();
    QFrame *frameControles = new QFrame();
    grid = new QGridLayout();
    int row=0;
    for(int ig=0; ig<3; ig++){
        int col = 0;
        QFrame *frame = new QFrame();
        for(int ic=0; ic<4; ic++){
            QComboBox *dest = new QComboBox();

            for(int opt=0; opt<19; opt++){
                dest->addItem(CONTROLES_DEST_DEPTH[opt]);
            }

            QSlider *depth = new QSlider(Qt::Horizontal);
            depth->setMinimum(0);
            depth->setMaximum(126);
            depth->setValue(63);

            lista->append(dest);
            lista->append(depth);

            /*
            grid->addWidget(dest,ig, col, 1, 1, Qt::AlignTop);
            col++;
            grid->addWidget(depth,ig, col, 1, 1, Qt::AlignTop);
            col++;
            */
            grid->addWidget(dest,row, ic, 1, 1, Qt::AlignTop);
            row++;
            grid->addWidget(depth,row, ic, 1, 1, Qt::AlignTop);
            row--;
        }
        row+=2;
    }

    frameControles->setLayout(grid);
    scrollArea->setWidget(frameControles);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//    QVBoxLayout *vbox = new QVBoxLayout(tab);
//    tab->setLayout(vbox);
//    scrollArea->setViewport(grid);
    tab->addTab(scrollArea,"Controles");
}

PatchUI::~PatchUI()
{
    delete ui;
}
