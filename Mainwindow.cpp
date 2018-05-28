#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include <QDir>
#include <QList>
#include "Efeito.h"
#include "parttab.h"
#include <QDebug>
#include "dialogjack.h"
#include <QSettings>
#include <QStringList>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    MainWindow::showMaximized();
    ui->setupUi(this);

    this->jack= new MidiControl();

    //this->setCentralWidget(ui->tabWidget);

    configMusicasDir = QDir::homePath() + "/.config/setlist/songs";// QStandardPaths::locate(QStandardPaths::HomeLocation, "songs", QStandardPaths::LocateFile);

    QDir *musicasDir = new QDir( configMusicasDir );

    QStringList musicas = musicasDir->entryList(QDir::Files);

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadTextFile(QString file)
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


void MainWindow::on_listWidget_itemSelectionChanged()
{
    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];

    loadTextFile(configMusicasDir + "/" + selecionado->text());
}

void MainWindow::on_perfBtnEnviar_clicked()
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

void MainWindow::on_perfIntervaloNotas_clicked()
{
    if(ui->perfIntervaloNotas->isChecked())
        ui->perfIntervaloNotas->setText("COM REGIÕES");
    else
        ui->perfIntervaloNotas->setText("UNIÃO");
}

void MainWindow::on_actionConectar_triggered()
{
    DialogJack *dlg = new DialogJack(jack, this);
    dlg->show();
}



void MainWindow::on_actionSalvar_SYSEX_triggered()
{

    QString diretorio = QFileDialog::getExistingDirectory();
    QSettings *conf = new QSettings(diretorio.append("/teste.ini"), QSettings::IniFormat);
    conf->beginGroup("PerformanceCommon");
    conf->setValue("nome",ui->perfName->text());
    conf->setValue("origem",ui->perfOrigem->currentIndex());
    conf->setValue("efeito",ui->perfEfeito->currentIndex() );
    conf->setValue("param1",ui->perfParam->value());
    conf->setValue("param2",ui->perfParam_2->value());
    conf->setValue("param3",ui->perfParam_3->value());
    conf->setValue("param4",ui->perfParam_4->value());
    conf->setValue("param5",ui->perfParam_5->value());
    conf->setValue("param6",ui->perfParam_6->value());
    conf->setValue("param7",ui->perfParam_7->value());
    conf->setValue("param8",ui->perfParam_8->value());
    conf->setValue("param9",ui->perfParam_9->value());
    conf->setValue("param10",ui->perfParam_10->value());
    conf->setValue("param11",ui->perfParam_11->value());
    conf->setValue("param12",ui->perfParam_12->value());
    conf->setValue("oa",ui->perfOA->currentIndex());
    conf->setValue("msl",ui->perfMSL->value());
    conf->setValue("csl",ui->perfCSL->value());
    conf->setValue("rsl",ui->perfperfRSL->value());
    conf->setValue("ctrlSrc1",ui->perfCtrlSrc1->currentIndex());
    conf->setValue("ctrlDepth1",ui->perfCtrlDepth1->value());
    conf->setValue("ctrlSrc2",ui->perfCtrlSrc2->currentIndex());
    conf->setValue("ctrlDepth2",ui->perfCtrlDepth2->value());
    conf->setValue("chorusLevel",ui->perfChorusLevel->value());
    conf->setValue("chorusRate",ui->perfChorusRate->value());
    conf->setValue("chorusDepth",ui->perfChorusDepth->value());
    conf->setValue("chorusPreDelay",ui->perfChorusPreDelay->value());
    conf->setValue("chorusFeedback",ui->perfChorusFeedback->value());
    conf->setValue("chorusOut",ui->perfChorusOut->currentIndex());
    conf->setValue("reverbType",ui->perfReverbType->currentIndex());
    conf->setValue("reverbLevel",ui->perfReverbLevel->value());
    conf->setValue("reverbTime",ui->perfReverbTime->value());
    conf->setValue("reverbHFDamp",ui->perfReverbHFDamp->currentIndex());
    conf->setValue("reverbDelayFeedback",ui->perfReverbDelayFeedback->value());
    conf->setValue("tempo",ui->perfTempo->value());
    conf->setValue("intervaloNotas",ui->perfIntervaloNotas->isChecked() ? 1 : 0);
    conf->setValue("modo",ui->perfMode->currentIndex());
    conf->endGroup();

    for(int i=0 ; i<=15; i++){
        PartTab *tab = tabParts[i];
        conf->beginGroup(QString("Parte%1").arg(i));

        conf->setValue("nome", tab->getPatch()->getNome() );
        conf->setValue("categoria", tab->getPatch()->getCategoria() );
        conf->setValue("categoriaPai", tab->getPatch()->getCategoriaPai() );
        conf->setValue("groupId", tab->getPatch()->getGroupId() );
        conf->setValue("groupType", tab->getPatch()->getGroupType() );
        conf->setValue("number", tab->getPatch()->getNumber() );

        conf->setValue("afinacaoBruta", tab->getAfinacaoBruta() );
        conf->setValue("afinacaoFina", tab->getAfinacaoFina() );
        conf->setValue("canalMidi", tab->getCanalMidi() );
        conf->setValue("chorusLevel", tab->getChorusLevel() );
        conf->setValue("mixEfxSendLevel", tab->getMixEfxLevel() );
        conf->setValue("oitava", tab->getOitava() );
        conf->setValue("pan", tab->getPan() );
        conf->setValue("regiaoMax", tab->getRegiaoMax() );
        conf->setValue("regiaoMin", tab->getRegiaoMin() );
        conf->setValue("reverbLevel", tab->getReverbLevel() );
        conf->setValue("saida", tab->getSaida() );
        conf->setValue("volume", tab->getVolume() );

        conf->endGroup();
    }
}

void MainWindow::on_actionAbrir_SYSEX_triggered()
{
//    settings = new QSettings("/tmp/teste.ini",QSettings::IniFormat);
//    settings->beginGroup("user");
//    QStringList player_number = settings->childGroups(); // returns 0, 1, 2 - OK !
//    const QStringList childKeys = settings->childKeys(); // should return name, wins, ... right ?
//    foreach(const QString &childKey, childKeys)
//        {
//            ui->choosePlayerBox->addItem(settings->value(childKey).toString());  // should add lukasz, 3, 3, pawel...., but it doesn`t work

//        }
//    settings->endGroup();

    QSettings *conf = new QSettings("/tmp/teste.ini", QSettings::IniFormat);
    conf->beginGroup("PerformanceCommon");

    ui->perfName->setText(conf->value("nome").toString());
    ui->perfOrigem->setCurrentIndex(conf->value("origem").toInt());
    ui->perfEfeito->setCurrentIndex(conf->value("efeito").toInt());
    ui->perfParam->setValue(conf->value("param1").toInt());
    ui->perfParam_2->setValue(conf->value("param2").toInt());
    ui->perfParam_3->setValue(conf->value("param3").toInt());
    ui->perfParam_4->setValue(conf->value("param4").toInt());
    ui->perfParam_5->setValue(conf->value("param5").toInt());
    ui->perfParam_6->setValue(conf->value("param6").toInt());
    ui->perfParam_7->setValue(conf->value("param7").toInt());
    ui->perfParam_8->setValue(conf->value("param8").toInt());
    ui->perfParam_9->setValue(conf->value("param9").toInt());
    ui->perfParam_10->setValue(conf->value("param10").toInt());
    ui->perfParam_11->setValue(conf->value("param11").toInt());
    ui->perfParam_12->setValue(conf->value("param12").toInt());

    ui->perfOA->setCurrentIndex(conf->value("oa").toInt());
    ui->perfMSL->setValue(conf->value("msl").toInt());
    ui->perfCSL->setValue(conf->value("csl").toInt());
    ui->perfperfRSL->setValue(conf->value("rsl").toInt());
    ui->perfCtrlSrc1->setCurrentIndex(conf->value("ctrlSrc1").toInt());
    ui->perfCtrlDepth1->setValue(conf->value("ctrlDepth1").toInt());
    ui->perfCtrlSrc2->setCurrentIndex(conf->value("ctrlSrc2").toInt());
    ui->perfCtrlDepth2->setValue(conf->value("ctrlDepth2").toInt());


//    conf->setValue("chorusLevel",ui->perfChorusLevel->value());
//    conf->setValue("chorusRate",ui->perfChorusRate->value());
//    conf->setValue("chorusDepth",ui->perfChorusDepth->value());
//    conf->setValue("chorusPreDelay",ui->perfChorusPreDelay->value());
//    conf->setValue("chorusFeedback",ui->perfChorusFeedback->value());
//    conf->setValue("chorusOut",ui->perfChorusOut->currentIndex());
//    conf->setValue("reverbType",ui->perfReverbType->currentIndex());
//    conf->setValue("reverbLevel",ui->perfReverbLevel->value());
//    conf->setValue("reverbTime",ui->perfReverbTime->value());
//    conf->setValue("reverbHFDamp",ui->perfReverbHFDamp->currentIndex());
//    conf->setValue("reverbDelayFeedback",ui->perfReverbDelayFeedback->value());
//    conf->setValue("tempo",ui->perfTempo->value());
//    conf->setValue("intervaloNotas",ui->perfIntervaloNotas->isChecked() ? 1 : 0);
//    conf->setValue("modo",ui->perfMode->currentIndex());
    conf->endGroup();
//fazer setters:
//    for(int i=0 ; i<=15; i++){
//        PartTab *tab = tabParts[i];
//        conf->beginGroup(QString("Parte%1").arg(i));

//        conf->setValue("nome", tab->getPatch()->getNome() );
//        conf->setValue("categoria", tab->getPatch()->getCategoria() );
//        conf->setValue("categoriaPai", tab->getPatch()->getCategoriaPai() );
//        conf->setValue("groupId", tab->getPatch()->getGroupId() );
//        conf->setValue("groupType", tab->getPatch()->getGroupType() );
//        conf->setValue("number", tab->getPatch()->getNumber() );

//        conf->setValue("afinacaoBruta", tab->getAfinacaoBruta() );
//        conf->setValue("afinacaoFina", tab->getAfinacaoFina() );
//        conf->setValue("canalMidi", tab->getCanalMidi() );
//        conf->setValue("chorusLevel", tab->getChorusLevel() );
//        conf->setValue("mixEfxSendLevel", tab->getMixEfxLevel() );
//        conf->setValue("oitava", tab->getOitava() );
//        conf->setValue("pan", tab->getPan() );
//        conf->setValue("regiaoMax", tab->getRegiaoMax() );
//        conf->setValue("regiaoMin", tab->getRegiaoMin() );
//        conf->setValue("reverbLevel", tab->getReverbLevel() );
//        conf->setValue("saida", tab->getSaida() );
//        conf->setValue("volume", tab->getVolume() );

//        conf->endGroup();
//    }
}
