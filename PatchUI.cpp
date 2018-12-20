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

    /*
     * Adiciona ícones aos 2 Combos de Estrutura do Chain EFX
     **/
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

    /*
     * Adiciona ícone
     * */
    QComboBox *combo = ui->patchCategory;
    QComboBox *comboTone1Categ = ui->tone1WaveGroup;
    QComboBox *comboTone2Categ = ui->tone2WaveGroup;
    QComboBox *comboTone3Categ = ui->tone3WaveGroup;
    QComboBox *comboTone4Categ = ui->tone4WaveGroup;
    combo->setMinimumSize(QSize(100,60));
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




    /*
     * Adiciona uma nova TabPage ao TabWidget
     * Controles de Destino e Depth
     * */
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

    /*
     * Adiciona Aba de Patch Tone e instancia inúmeros controles
     */
    drawPatchTone();
}

PatchUI::~PatchUI()
{
    delete ui;
}

void PatchUI::drawPatchTone(){
    /*
     * Aba de Wave de cada Tone
     * */
    QTabWidget *tab = ui->tabWidgetTonesDetails;
    QFrame *frame = new QFrame();
    QGridLayout *grid = new QGridLayout();
    frame->setLayout(grid);

    for(int toneid=0; toneid<4; toneid++){
        QPushButton *toneSwitch = new QPushButton(QString("%1").arg(toneid+1));
        toneSwitch->setCheckable(true);

        QComboBox *waveId = new QComboBox();

        QComboBox *waveGain = new QComboBox();
        waveGain->addItem("-6");        waveGain->addItem("0");
        waveGain->addItem("+6");        waveGain->addItem("+12");
        waveGain->setCurrentIndex(1);

        QPushButton *fxmSwitch = new QPushButton("FXM");
        fxmSwitch->setCheckable(true);

        QSpinBox *fxmColor = new QSpinBox();
        fxmColor->setMinimum(1);         fxmColor->setMaximum(4);

        QSpinBox *fxmDepth = new QSpinBox();
        fxmDepth->setMinimum(1);         fxmDepth->setMaximum(16);

        QComboBox *toneDelayMode = new QComboBox();
        toneDelayMode->addItem("NORMAL");        toneDelayMode->addItem("HOLD");
        toneDelayMode->addItem("PLAYMATE");      toneDelayMode->addItem("CLOCK-SYNC");
        toneDelayMode->addItem("TAP-SYNC");      toneDelayMode->addItem("KEY-OFF-N");
        toneDelayMode->addItem("KEY-OFF-D");     toneDelayMode->addItem("TEMPO-SYNC");
        toneDelayMode->setCurrentIndex(0);

        QSpinBox *toneDelayTime = new QSpinBox();
        toneDelayTime->setMinimum(0);           toneDelayTime->setMaximum(127);
        toneDelayTime->setValue(0);

        QSpinBox *veocityRangeCrossFade = new QSpinBox();
        QSpinBox *veocityRangeLower = new QSpinBox();
        QSpinBox *veocityRangeUpper = new QSpinBox();
        QComboBox *keyboardRangeLower = new QComboBox();
        QComboBox *keyboardRangeUpper = new QComboBox();


        QPushButton *redamperControlSwitch = new QPushButton();
        QPushButton *volumeControlSwitch = new QPushButton();
        QPushButton *hold1ControlSwitch = new QPushButton();
        QPushButton *pitchBendControlSwitch = new QPushButton();
        QPushButton *panControlSwitch = new QPushButton();
        QComboBox *controlerDestination = new QComboBox();
        QSpinBox *controlerDepth = new QSpinBox();

        toneSwitchList->append(toneSwitch);
        waveIdList->append(waveId);
        waveGainList->append(waveGain);
        fxmSwitchList->append(fxmSwitch);
        fxmColorList->append(fxmColor);
        fxmDepthList->append(fxmDepth);
        toneDelayModeList->append(toneDelayMode);
        toneDelayTimeList->append(toneDelayTime);

        int r=0;
        grid->addWidget(toneSwitch, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(waveId, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(waveGain, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(fxmSwitch, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(fxmColor, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(fxmDepth, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(toneDelayMode, r++, toneid, 1, 1, Qt::AlignTop);
        grid->addWidget(toneDelayTime, r++, toneid, 1, 1, Qt::AlignTop);

    }

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(frame);
    tab->addTab(scrollArea, "Patch Tone");
}


