#define DEBUG
#ifdef DEBUG
#include <QDebug>
#endif

#include "WidgetPrincipal.h"

#include "parttab.h"
#include "Efeito.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringList>
#include <QStandardPaths>
#include <QTabWidget>
#include <QUrl>
//#include <QShortcut>
#include <QtWebKitWidgets/QWebView>
#include "MidiControl.h"
#include <jack/jack.h>
#include <QList>
#include <QDebug>
#include <QToolBar>

TextFinder::TextFinder(MidiControl *jack, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextFinder)
{

    QWidget::showMaximized();
    ui->setupUi(this);

    this->jack = jack;

    //loadTextFile();

    /*
    QString musicas[] = {
        "Maior Abandonado",
        "Bete Balanço",
        "Cuidado",
    };
    QListWidgetItem *item[3];
    for(int i=0; i<3; i++ ){
        item[i] = new QListWidgetItem(ui->listWidget);
        item[i]->setText(musicas[i]);
        ui->listWidget->addItem(item[i]);
    }
    */

    configMusicasDir = QDir::homePath() + "/.config/setlist/songs";// QStandardPaths::locate(QStandardPaths::HomeLocation, "songs", QStandardPaths::LocateFile);
#ifdef DEBUG
    qDebug() << "config dir " << configMusicasDir;
#endif
    QDir *musicasDir = new QDir( configMusicasDir );

    QStringList musicas = musicasDir->entryList(QDir::Files);
#ifdef DEBUG
    qDebug() << "contagem " << musicasDir->count();
#endif
    QListWidgetItem *item[musicasDir->count()-2];

    for(int i=0; i<musicasDir->count()-2; i++ ){
        item[i] = new QListWidgetItem(ui->listWidget);
        QString str = musicas[i];

        item[i]->setText(str);
        ui->listWidget->addItem(item[i]);
    }

    for(int efx=0; efx<40; efx++ ){
        QString nome = QString("%1 " + Efeito::nomes[efx]).arg(efx+1);

        ui->perfEfeito->addItem(nome, QVariant::fromValue(0));
    }

    //Combo Perform, Parte Origem do efeito. Canal 10 não é válido.
    for(int i=0; i<=16; i++){
        QString texto = QString("%1").arg(i);
        if(i==0) texto = "PERFORM";
        if(i==10) continue;
        ui->perfOrigem->addItem(texto);
    }

    /*
     * PARTES
     */
    QTabWidget *tab = ui->tabWidget;
    for(int i=0 ; i<=15; i++){
        tabParts[i] = new PartTab(i+1, this->jack);
        QString name = QString::number(i+1) + "    ";

        tab->addTab(tabParts[i], name );
    }


    //    QShortcut *scProximaMusica = new QShortcut(QKeySequence("Ctrl+N"), parent );
    //    QObject::connect(scProximaMusica, SIGNAL(activated()), ui->listWidget, SLOT( proximaMusica() ) ) ;



}



TextFinder::~TextFinder()
{
    delete ui;
}



void TextFinder::loadTextFile(QString file)
{

    //    QFile inputFile(file);
    //    inputFile.open(QIODevice::ReadOnly);

    //    QTextStream in(&inputFile);
    //    QString line = in.readAll();
    //    inputFile.close();

    //    ui->textEdit->setPlainText(line);
    //    QTextCursor cursor = ui->textEdit->textCursor();
    //    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);




    ui->webView->load(QUrl(QString("file://" + file )));



}


void TextFinder::on_listWidget_itemSelectionChanged()
{
    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
#ifdef DEBUG
    qDebug() << "Item Selecionado " << selecionado->text();
#endif
    loadTextFile(configMusicasDir + "/" + selecionado->text());
}

void TextFinder::on_perfBtnEnviar_clicked()
{
    QList<int> *dados = new QList<int>();
    //Perfomance Commmon / Name 12 posições
    QString performanceName = ui->perfName->text();
    for(int i=performanceName.length() ; i<12; i++){
        performanceName.append(" ");
    }
    for(int i=0 ; i<12; i++){
        dados->append(performanceName.toLatin1().data()[i]);

    }

    dados->append(ui->perfOrigem->currentIndex());
    dados->append(ui->perfEfeito->currentIndex() );
    dados->append(ui->perfParam->value());
    dados->append(ui->perfParam_2->value());
    dados->append(ui->perfParam_3->value());
    dados->append(ui->perfParam_4->value());
    dados->append(ui->perfParam_5->value());
    dados->append(ui->perfParam_6->value());
    dados->append(ui->perfParam_7->value());
    dados->append(ui->perfParam_8->value());
    dados->append(ui->perfParam_9->value());
    dados->append(ui->perfParam_10->value());
    dados->append(ui->perfParam_11->value());
    dados->append(ui->perfParam_12->value());
    dados->append(ui->perfOA->currentIndex());
    dados->append(ui->perfMSL->value());
    dados->append(ui->perfCSL->value());
    dados->append(ui->perfperfRSL->value());
    dados->append(ui->perfCtrlSrc1->currentIndex());
    dados->append(ui->perfCtrlDepth1->value());
    dados->append(ui->perfCtrlSrc2->currentIndex());
    dados->append(ui->perfCtrlDepth2->value());
    dados->append(ui->perfChorusLevel->value());
    dados->append(ui->perfChorusRate->value());
    dados->append(ui->perfChorusDepth->value());
    dados->append(ui->perfChorusPreDelay->value());
    dados->append(ui->perfChorusFeedback->value());
    dados->append(ui->perfChorusOut->currentIndex());
    dados->append(ui->perfReverbType->currentIndex());
    dados->append(ui->perfReverbLevel->value());
    dados->append(ui->perfReverbTime->value());
    dados->append(ui->perfReverbHFDamp->currentIndex());
    dados->append(ui->perfReverbDelayFeedback->value());
    dados->append(ui->perfTempo->value());
    dados->append(ui->perfIntervaloNotas->isChecked() ? 1 : 0);
    //pula alguns endereços/itens de config
    dados->append(ui->perfMode->currentIndex());//item 48 da lista




    jack->setPerformanceCommon(dados);


}

void TextFinder::on_perfIntervaloNotas_clicked()
{
    if(ui->perfIntervaloNotas->isChecked())
        ui->perfIntervaloNotas->setText("COM REGIÕES");
    else
        ui->perfIntervaloNotas->setText("UNIÃO");
}
