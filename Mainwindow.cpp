#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include "kqueue.h"
#include <QDir>
#include <QList>
#include "Efeito.h"
#include "parttab.h"
#include <QDebug>
#include "dialogjack.h"
#include <QSettings>
#include <QStringList>
#include <QFileDialog>
#include <unistd.h>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QFile>
#include <QTextStream>
#include <cstdlib>
#include <QFileSystemWatcher>
#include <sys/inotify.h>
#include <thread>
#include <future>
#include <QTextCodec>
#include <QUuid>
#include "dialogdocumenteditor.h"
#include <QMap>
#include "Controller.h"
#include <QAction>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    MainWindow::showMaximized();
    ui->setupUi(this);


    ui->tabWidget->setCurrentIndex(0);
    this->jack= new MidiControl();


    qstageDir = QString(getenv("XDG_CONFIG_HOME")) + "/QStage";

    /*
    *   *************************************
    *       configura UI
    * ***************************************
    */
    try {
        setStyleSheet(Controller::getUltimoTemaUsado());
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Erro ao obter o último tema usado", e.what());
    }

    /*
     * **************************************
     *  cria menu de temas de look and feel
     * **************************************
     */
    try {
        QMenu *menuVisual = menuBar()->addMenu("Visual");

        QMap<QString, QString> temas = Controller::queryTemasUI();
        for(int i=0; i < temas.count(); i++){

            QString nomeTema = temas.keys().at(i);
            QString css = temas.value(temas.keys().at(i));

            QAction *act = menuVisual->addAction( temas.keys().at(i));
            act->setStatusTip("Ativar o tema " + nomeTema);
            act->setData(QVariant::fromValue(css));

        }
        connect(menuVisual, SIGNAL(triggered(QAction*)), this, SLOT(setTema(QAction*)));
    } catch (std::exception &e) {
        QMessageBox::warning(this,"Erro ao Configurar Tema", e.what());
    }

    //caso contrário o programa quebra ao tentar transmitir sinais quando carregar configurações
    if(!tentarAutoConectar(getConfig("port")))
        on_actionConectar_triggered();

    //    QDir *musicasDir = new QDir( configMusicasDir );

    //    QStringList musicas = musicasDir->entryList(QDir::Files);

    //    QListWidgetItem *item[musicasDir->count()-2];

    //    for(int i=0; i<musicasDir->count()-2; i++ ){
    //        item[i] = new QListWidgetItem(ui->listWidget);
    //        QString str = musicas[i];

    //        item[i]->setText(str);
    //        ui->listWidget->addItem(item[i]);
    //    }

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

    ui->perfReverbHFDamp->setCurrentIndex(17);//seleciona "BYPASS"

    /*
     * PARTES
     */
    QTabWidget *tab = ui->tabWidget;
    for(int i=0 ; i<=15; i++){
        tabParts[i] = new PartTab(i+1, this->jack);
        QString name = QString::number(i+1) + "    ";

        tab->addTab(tabParts[i], name );
    }

    //carrega playlists e a primeira lista de músicas da playlist
    this->playlistRecarregar();

    dlgPlaylist = new dialog_playlist(this);
    connect(dlgPlaylist, SIGNAL(registrarPlaylist(QString)), this, SLOT(playlistIncluir(QString)));
    connect(dlgPlaylist, SIGNAL(alterarPlaylist(int,QString)), this, SLOT( playlistAlterar(int,QString)));

    dlgMusica = new DialogMusica(this);
    connect(dlgMusica, SIGNAL(incluirMusica(QString)), this, SLOT(musicaIncluir(QString)));
    connect(dlgMusica, SIGNAL(alterarMusica(QString)), this, SLOT(musicaAlterar(QString)));

    dlgMusicaExistente = new DialogMusicaExistente();
    connect(dlgMusicaExistente, SIGNAL(musicaExistenteSelecionada(int)),this, SLOT(musicaExistenteIncluirNaPlaylist(int)));

    dlgEditorHTML = new DialogDocumentEditor(this);
    connect(dlgEditorHTML, SIGNAL(edicaoHTMLTerminada(int,QString)), this, SLOT(onEdicaoHTMLEncerrada(int,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTema(QAction * action){
    QString css = action->data().toString();
    setStyleSheet(css);

    try {
        Controller::setUltimoTemaUsado(action->text());
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Erro ao Atualizar Último Tema Usado", e.what());
    }
}

void MainWindow::carregarHTML()
{

    //    QFile inputFile(file);
    //    inputFile.open(QIODevice::ReadOnly);

    //    QTextStream in(&inputFile);
    //    QString line = in.readAll();
    //    inputFile.close();

    //    ui->textEdit->setPlainText(line);
    //    QTextCursor cursor = ui->textEdit->textCursor();
    //    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
//    ui->webView->setHtml(QString("<strong>TESTE</strong><h1>OK</h1>"));// load(QUrl(QString("file://" + file )));

    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();

    ui->webView->setHtml(musica->html );

}

void MainWindow::on_listWidget_itemSelectionChanged()
{
    carregarHTML();
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
        dados->append(performanceName.toUtf8().data()[i]);

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

    /*
     * System Common que está na mesma UI
     */
    QMap<int, int> systemCommon;
    systemCommon.insert(0x002B, ui->sysBtnTranspose->isChecked() ? 1 : 0 );
    systemCommon.insert(0x002C, ui->sysTransposeVal->value()+5);

    jack->setSystemCommon(systemCommon);
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
    if(ui->listWidget->selectedItems().length() <= 0){
        QMessageBox msg;
        msg.setText("Selecione uma música da lista");
        msg.exec();
        return;
    }

    try
    {

        QString tmp("/tmp/QStage." + QUuid().createUuid().createUuid().toString() + ".ini");
        QFile arquivoTemporario(tmp);
//        QTemporaryFile arquivoTemporario;
//        arquivoTemporario.open();

        int playlistId = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();
        QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
        Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();
        int musicaId = musica->musicaId;

        if(musica->programa.length() > 0){
            QMessageBox::StandardButton resposta = QMessageBox::question(this, "Sobrescrever SYSX", "O arquivo SYSX para "+ musica->titulo +" já existe.\nSobrescrever?", QMessageBox::Yes | QMessageBox::No);
            if(resposta != QMessageBox::Yes)
                return;
        }

        //salva no arquivo temporário
        QSettings *conf = new QSettings(arquivoTemporario.fileName(), QSettings::IniFormat);
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
            conf->setValue("localOn", tab->isLocalOn() );

            conf->endGroup();
        }

        /*
         * System (Common)
         */
        conf->beginGroup("System");
        conf->setValue("transpose", ui->sysBtnTranspose->isChecked()? 1 : 0);
        conf->setValue("transposeVal", ui->sysTransposeVal->value());
        conf->endGroup();

        conf->sync();
        arquivoTemporario.open(QFile::ReadOnly);//usar para QFile
        QTextStream entrada(&arquivoTemporario);  entrada.seek(0);

        //persiste do arquivo no banco de dados
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "UPDATE musicas SET programa = ? WHERE musica_id = ?");
        query.bind(1, entrada.readAll().toUtf8().data() );
        query.bind(2, musicaId);
        query.exec();
        arquivoTemporario.close();
        this->playlistRecarregar();

    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Remover Música", e.what());
        qDebug() << "exception: " << e.what();
    }


}

void MainWindow::on_actionAbrir_SYSEX_triggered()
{

    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();

    //qDebug() << QString(configSysExDir).append("/" + ui->listWidget->selectedItems()[0]->text() + ".ini");

    //QSettings *conf = new QSettings("/tmp/teste.ini", QSettings::IniFormat);
//    QSettings *conf = new QSettings(QString(configSysExDir).append("/" + ui->listWidget->selectedItems()[0]->text() + ".ini"), QSettings::IniFormat);

    QTemporaryFile arquivoTemporario;
    if(!arquivoTemporario.open())
        qDebug() << "erro ao abrir arquivo temporário";

    QTextStream fluxo(&arquivoTemporario);
    fluxo << musica->programa << endl;
    fluxo.flush();
    QSettings *conf = new QSettings( arquivoTemporario.fileName() , QSettings::IniFormat);
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

    ui->perfChorusLevel->setValue(conf->value("chorusLevel").toInt());
    ui->perfChorusRate->setValue(conf->value("chorusRate").toInt());
    ui->perfChorusDepth->setValue(conf->value("chorusDepth").toInt());
    ui->perfChorusPreDelay->setValue(conf->value("chorusPreDelay").toInt());
    ui->perfChorusFeedback->setValue(conf->value("chorusFeedback").toInt());
    ui->perfChorusOut->setCurrentIndex(conf->value("chorusOut").toInt());
    ui->perfReverbType->setCurrentIndex(conf->value("reverbType").toInt());
    ui->perfReverbLevel->setValue(conf->value("reverbLevel").toInt());
    ui->perfReverbTime->setValue(conf->value("reverbTime").toInt());
    ui->perfReverbHFDamp->setCurrentIndex(conf->value("reverbHFDamp").toInt());
    ui->perfReverbDelayFeedback->setValue(conf->value("reverbDelayFeedback").toInt());
    ui->perfTempo->setValue(conf->value("tempo").toInt());
    ui->perfIntervaloNotas->setChecked(conf->value("intervaloNotas").toInt());
    ui->perfMode->setCurrentIndex(conf->value("modo").toInt());
    conf->endGroup();

    /*
     * System (Common)
     */
    conf->beginGroup("System");
    ui->sysBtnTranspose->setChecked(conf->value("transpose").toInt() == 1 ? true : false );
    ui->sysTransposeVal->setValue(conf->value("transposeVal").toInt());
    conf->endGroup();

    on_perfBtnEnviar_clicked();


    for(int i=0 ; i<=15; i++){
        PartTab *tab = tabParts[i];
        conf->beginGroup(QString("Parte%1").arg(i));


        //        usleep(50000);//evitar sobrecarregar o ringbuffer
        tab->setLocalOn(conf->value("localOn").toInt());

        QString nomePatch = conf->value("nome").toString();
        if(QString::compare(nomePatch,"") != 0){
            tab->carregarPatches();
            tab->setPatch(tab->getIndexFromPatches(nomePatch));
            //tab->enviar(); usleep(20000);
            //            usleep(50000);
        }

        tab->setAfinacaoBruta(conf->value("afinacaoBruta").toInt());//tab->enviar(); usleep(20000);
        tab->setAfinacaoFina(conf->value("afinacaoFina").toInt());//tab->enviar(); usleep(20000);
        tab->setCanalMidi(conf->value("canalMidi").toInt());//tab->enviar(); usleep(20000);
        tab->setChorusLevel(conf->value("chorusLevel").toInt());//tab->enviar(); usleep(20000);
        tab->setMixEfxLevel(conf->value("mixEfxSendLevel").toInt());//tab->enviar(); usleep(20000);
        tab->setOitava(conf->value("oitava").toInt());//tab->enviar(); usleep(20000);
        //envio fracionado e com intervalos de tempo. foi assim que funcionou. as combos estavam ficando perdidas nos envios de SYSEX sem isso.
        //        //tab->enviar();
        //        usleep(50000);
        tab->setPan(conf->value("pan").toInt());//tab->enviar(); usleep(20000);
        tab->setRegiaoMax(conf->value("regiaoMax").toInt());//tab->enviar(); usleep(20000);
        tab->setRegiaoMin(conf->value("regiaoMin").toInt());//tab->enviar(); usleep(20000);
        tab->setReverbLevel(conf->value("reverbLevel").toInt());//tab->enviar(); usleep(20000);
        tab->setSaida(conf->value("saida").toInt());//tab->enviar(); usleep(20000);
        tab->setVolume(conf->value("volume").toInt());//tab->enviar(); usleep(20000);
        tab->enviar();
        conf->endGroup();
    }
}

void MainWindow::on_perfEfeito_currentIndexChanged(int index)
{
    QStringList *labels = Efeito::getLabels(index);
    QList<int> *max = Efeito::getParametrosMaxVal(index);

    ui->perfParamLbl->setText(labels->at(0));
    ui->perfParamLbl_2->setText(labels->at(1));
    ui->perfParamLbl_3->setText(labels->at(2));
    ui->perfParamLbl_4->setText(labels->at(3));
    ui->perfParamLbl_5->setText(labels->at(4));
    ui->perfParamLbl_6->setText(labels->at(5));
    ui->perfParamLbl_7->setText(labels->at(6));
    ui->perfParamLbl_8->setText(labels->at(7));
    ui->perfParamLbl_9->setText(labels->at(8));
    ui->perfParamLbl_10->setText(labels->at(9));
    ui->perfParamLbl_11->setText(labels->at(10));
    ui->perfParamLbl_12->setText(labels->at(11));

    ui->perfParam->setMaximum(max->at(0));
    ui->perfParam_2->setMaximum(max->at(1));
    ui->perfParam_3->setMaximum(max->at(2));
    ui->perfParam_4->setMaximum(max->at(3));
    ui->perfParam_5->setMaximum(max->at(4));
    ui->perfParam_6->setMaximum(max->at(5));
    ui->perfParam_7->setMaximum(max->at(6));
    ui->perfParam_8->setMaximum(max->at(7));
    ui->perfParam_9->setMaximum(max->at(8));
    ui->perfParam_10->setMaximum(max->at(9));
    ui->perfParam_11->setMaximum(max->at(10));

}

void MainWindow::on_actionCarregar_Proxima_triggered()
{
    if(ui->listWidget->currentRow() < 0)
        ui->listWidget->setCurrentRow(0);//->setSelected(true);
    else if(ui->listWidget->currentRow() == ui->listWidget->count() - 1)
        return;
    else
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow()+1);//->setSelected(true);
}

void MainWindow::on_actionCarregar_Anterior_triggered()
{
    if(ui->listWidget->currentRow() < 0)
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    else if(ui->listWidget->currentRow() == 0)
        return;
    else
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow()-1);
}

void MainWindow::restaurarSelecao(int playlistIndex, int musicaIndex){
    if(this->ui->playlist->count() == 0)
        return;

    if(playlistIndex > -1){
        ui->playlist->setCurrentIndex(playlistIndex);
        this->on_playlist_currentIndexChanged(playlistIndex);
        if(musicaIndex > -1)
            ui->listWidget->setCurrentRow(musicaIndex);
    }
}

void MainWindow::playlistRecarregar(){
    //    auto f = [](int a, int b) -> int { return a + b; };
    //    auto n = f(1, 2);

    //memoriza estado atual das combos
    int playlistIndex = 0;
    int musicaRow = ui->listWidget->currentRow();
    if(ui->playlist->currentIndex() > -1){
        int playlistId = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();
        playlistIndex =  ui->playlist->findData(playlistId);
    }


    //limpa essa playlist
    disconnect(ui->playlist,SIGNAL(currentIndexChanged(int)), this
               , SLOT(on_playlist_currentIndexChanged(int)));
    ui->playlist->clear();
    //limpa a list de músicas da playlist no listWidget:
    disconnect(ui->listWidget,SIGNAL(itemSelectionChanged()), this
               , SLOT(on_listWidget_itemSelectionChanged()));
    ui->listWidget->clear();

    try
    {
        QMap<int, QString> rs = Controller::queryPlaylists();
        for(int i=0; i<rs.count(); i++ )
            ui->playlist->addItem( rs.value(rs.keys().at(i)) , QVariant::fromValue(rs.keys().at(i)));

    }
    catch (std::exception& e)
    {
        qDebug() << "exception: " << e.what();
        QMessageBox::warning(this, "Erro ao Recarregar Playlists", e.what());
    }

    connect(ui->playlist,SIGNAL(currentIndexChanged(int)), this
               , SLOT(on_playlist_currentIndexChanged(int)));
    connect(ui->listWidget,SIGNAL(itemSelectionChanged()), this
               , SLOT(on_listWidget_itemSelectionChanged()));

    this->restaurarSelecao(playlistIndex, musicaRow);
}

void MainWindow::on_actionAtualizar_Playlists_triggered()
{
    this->playlistRecarregar();
}

void MainWindow::on_playlist_currentIndexChanged(int index)
{

    /*
    QStringList musicas = musicasDir->entryList(QDir::Files);

    QListWidgetItem *item[musicasDir->count()-2];

    for(int i=0; i<musicasDir->count()-2; i++ ){
        item[i] = new QListWidgetItem(ui->listWidget);
        QString str = musicas[i];

        item[i]->setText(str);
        ui->listWidget->addItem(item[i]);
    }
    */

    /* sem desconectar o sinal antes de listWidget->clear
     * depois de selecionar um listWidget, troca-se um playlist, em seguida quebra
     */
    disconnect(ui->listWidget,SIGNAL(itemSelectionChanged()), this
               , SLOT(on_listWidget_itemSelectionChanged()));
    ui->listWidget->clear();
    int playlistId = ui->playlist->currentData().toInt();

    try
    {


        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data());
        SQLite::Statement   query(db, "SELECT m.musica_id, m.titulo, m.html, pm.ordem, m.programa, m.tag "
                                      "FROM musicas m JOIN playlist_musicas pm ON pm.fk_musica = m.musica_id  "
                                      "WHERE pm.fk_playlist = ? ORDER BY pm.ordem ASC;");
        query.bind(1, playlistId);
        while (query.executeStep())
        {
            int ordem = query.getColumn(3);
            int musicaId = query.getColumn(0);
            const char* tituloChar = query.getColumn(1);
            const char* htmlChar = query.getColumn(2);
            const char* programaChar = query.getColumn(4);
            QRgb tag = query.getColumn(5);

            QString titulo = QString("%1 %2").arg(QString::number(ordem), tituloChar);
            QString html = QString(htmlChar);
            QString programa = QString(programaChar);

            Musica *musica = new Musica();
            musica->musicaId = musicaId;
            musica->titulo = QString(tituloChar);
            musica->html = html;
            musica->programa = programa;

            QListWidgetItem *novoItem = new QListWidgetItem(ui->listWidget);
            novoItem->setText(titulo);
            novoItem->setBackgroundColor(QColor(tag));
            novoItem->setData(Qt::UserRole, QVariant::fromValue(musica));


            ui->listWidget->addItem(novoItem);
        }

        connect(ui->listWidget,SIGNAL(itemSelectionChanged()), this
                   , SLOT(on_listWidget_itemSelectionChanged()));

    }
    catch (std::exception& e)
    {
        qDebug() << "exception: " << e.what();
    }


}

void MainWindow::on_htmlFileChanged(){
    qDebug() << "arquivo foi alterado...";
    //QMessageBox::information(this, "OK","deu certo : " + file);
}

void MainWindow::abrirPrograma(){
    std::string str = std::string(this->path.toUtf8().data());
    system( str.c_str() );
}

bool MainWindow::watchFile(QString path){
//    if (Kqueue::watch(path))
//        qDebug() << "arquivo " << path << " foi modificado";
//    else
//        qDebug() << "arquivo " << path << " não sofreu alterações.";
    return Kqueue::watch(path);
}


void MainWindow::on_actionEditar_HTML_GVim_triggered()
{
    if(ui->listWidget->selectedItems().length() == 0){
        QMessageBox::warning(this, "Editar Música","Selecione uma música para editar.");
        return;
    }

    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();

    dlgEditorHTML->musicaId = musica->musicaId;
    dlgEditorHTML->setHtml(musica->html);
    dlgEditorHTML->show();
}

void MainWindow::onEdicaoHTMLEncerrada(int musicaId, QString html){

    try
    {
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data()
                            , SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "UPDATE musicas SET html = ? WHERE musica_id = ? ");

        query.bind(1, html.toUtf8().data() );
        query.bind(2, musicaId);
        query.exec();

        this->playlistRecarregar();
        carregarHTML();
    }
    catch (std::exception& e)
    {
        qDebug() << "exception: " << e.what();
        QMessageBox::warning(this,"Erro ao Salvar HTML da Música", QString(e.what()));
    }

}

/**
 * @deprecated era usado para salvar com LibreOffice
 * @brief MainWindow::editarHTML
 * @param binPath
 */
void MainWindow::editarHTML(QString binPath)
{
    if(ui->listWidget->selectedItems().length() == 0){
        QMessageBox::warning(this, "Editar Música","Selecione uma música para editar.");
        return;
    }

    QTemporaryFile file;

    //QFileSystemWatcher watcher;

    if(!file.open()){
        QMessageBox::warning(this, "Editar HTML", "Erro ao criar arquivo temporário");
        return;
    }

    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();
    QTextStream fluxo(&file);
    fluxo << musica->html << endl;
    fluxo.flush();
    qDebug() << file.fileName();
    const char * tmp =  + file.fileName().toUtf8().data();


    std::string str = std::string(binPath.toUtf8().data()) + file.fileName().toUtf8().data();
    system( str.c_str() );

    //if(Kqueue::watch(file.fileName())){ //não vai perceber a modificação realizada antes
        //persistir HTML no banco de dados
        try
        {
            SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data()
                                , SQLite::OPEN_READWRITE);
            SQLite::Statement   query(db, "UPDATE musicas SET html = ? WHERE musica_id = ? ");

            QTextStream *arquivoHTMLTemporario = &fluxo;
//            QTextStream arquivoHTMLTemporario(&file);
            arquivoHTMLTemporario->setCodec(QTextCodec::codecForName("UTF-8") );
            arquivoHTMLTemporario->seek(0);
            musica->html = arquivoHTMLTemporario->readAll();

            query.bind(1, musica->html.toUtf8().data() );
            query.bind(2, musica->musicaId);

            query.exec();

            //o geany chega aqui depois de salvar
            // o libreoffice chega aqui direto se já houver uma instância dele na memória (outro doc aberto por outro programa ou pelo usuário)
            qDebug() << "Rodei UPDATE SQLite";
        }
        catch (std::exception& e)
        {
            qDebug() << "exception: " << e.what();
            QMessageBox::warning(this,"Erro ao Salvar Letra da Música", QString(e.what()));
        }
    //}



    /*
    std::thread threadKqueue(&MainWindow::watchFile, this, file.fileName());
    threadKqueue.join();

    int pid = fork();
    if(pid == 0){
        std::string str = std::string("/usr/local/bin/libreoffice --writer ") + file.fileName().toUtf8().data();
        //system( str.c_str() );
        //AbrirProgramaController ctrl(QString("/usr/local/bin/libreoffice --writer "  + file.fileName() ));
        this->path = QString("/usr/local/bin/libreoffice --writer " + file.fileName());
        std::thread threadAbrirPrograma(&MainWindow::abrirPrograma , this);
        threadAbrirPrograma.join();
    }
    */

    /*
    //threads roda uma depois da outra, mas o kqueue percebe que o arquivo foi modificado ao fechar o libreoffice
    //mas se o arquivo não for modificado, minha implementação do kqueue trava, pois espera uma modificação
    std::future<bool> threadKqueue = std::async(std::launch::async, &MainWindow::watchFile, this, file.fileName());
    std::string str = std::string("/usr/local/bin/libreoffice --writer ") + file.fileName().toUtf8().data();
    this->path = QString("/usr/local/bin/libreoffice --writer " + file.fileName());
    std::future<void> threadAbrirPrograma = std::async(std::launch::async, &MainWindow::abrirPrograma , this);
    threadAbrirPrograma.get();

    if(threadKqueue.get()){
        qDebug() << "modificou, pode enfiar o HTML no banco de dados";
        try
        {
            SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data());
            SQLite::Statement   query(db, "UPDATE musicas SET html = ? WHERE musica_id = ?");

            query.bind(0, musica->html.toUtf8().data() );
            query.bind(1, musica->musicaId);

            query.exec();

        }
        catch (std::exception& e)
        {
            qDebug() << "exception: " << e.what();
            QMessageBox::warning(this,"Erro ao Salvar Letra da Música", QString(e.what()));
        }
    }
    */


    /*
    watcher.addPath(file.fileName());

    watcher.connect(&watcher
            , SIGNAL(fileChanged(QString))
            , this
            , SLOT(on_htmlFileChanged()) );
    */

    //ver arquivo em /usr/home/marcelbonnet/devel/workspace-audio/m-pad/src
//    int fd = inotify_init();
//    for(;;){
//        int retval = inotify_add_watch(fd, tmp, IN_MODIFY );
//        if(retval > 0){
//            qDebug() << "arquivo " << tmp << " foi modificado";
//            break;
//        }
//    }



}




void MainWindow::on_action_Playlist_Nova_triggered()
{
    this->dlgPlaylist->show();
}

void MainWindow::playlistIncluir(QString str){
   try
    {
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "INSERT INTO playlist (titulo) VALUES(?) ");
        query.bind(1, str.toUtf8().data());
        query.exec();
        this->playlistRecarregar();
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Incluir Playlist", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void MainWindow::playlistAlterar(int id, QString str){
    try
    {
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "UPDATE playlist SET titulo = ? WHERE id = ? ");
        query.bind(1, str.toUtf8().data());
        query.bind(2, id);
        query.exec();
        this->playlistRecarregar();
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Alterar Playlist", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void MainWindow::playlistRemover(int id){
    try
    {
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "DELETE FROM playlist_musicas WHERE fk_playlist = ?");
        query.bind(1, id);
        query.exec();

        SQLite::Statement   query2(db, "DELETE FROM playlist WHERE id = ?");
        query2.bind(1, id);
        query2.exec();
        this->playlistRecarregar();
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Remover Playlist", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void MainWindow::on_action_Playlist_Remover_triggered()
{
    QMessageBox::StandardButton question = QMessageBox::question(this,
                                                                 "Remover Playlist",
                                                                 "Remover a playlist: " + ui->playlist->currentText(),
                                                                 QMessageBox::Yes | QMessageBox::No);

    if(question != QMessageBox::Yes )
        return;

    int id = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();
    this->playlistRemover(id);
}

void MainWindow::on_action_Playlist_Editar_triggered()
{
    int id = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();
    QString str = ui->playlist->currentText();
    dlgPlaylist->setId(id);
    dlgPlaylist->setNome(str);
    dlgPlaylist->show();
}

void MainWindow::on_action_Playlist_Duplicar_triggered()
{
    int id = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();
    QString str = ui->playlist->currentText() + " (Cópia)";

    try
    {
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "INSERT INTO playlist (titulo) SELECT titulo || ' (cópia)' FROM playlist WHERE id= ?");
        query.bind(1, id);
        query.exec();

        SQLite::Statement   query2(db, "INSERT INTO playlist_musicas (fk_musica, fk_playlist, ordem) SELECT fk_musica, (SELECT last_insert_rowid()) , ordem FROM playlist_musicas WHERE fk_playlist = ?");
        query2.bind(1, id);
        query2.exec();
        this->playlistRecarregar();
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Duplicar Playlist", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void MainWindow::musicaIncluir(QString str){
    int id = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();

    try
    {
        int modeloId = dlgMusica->getModeloId();

        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "INSERT INTO musicas(titulo, html) "
                                      "VALUES (?, (SELECT modelo FROM modelos WHERE id=? ))");
        query.bind(1, str.toUtf8().data() );
        query.bind(2, modeloId);
        query.exec();

        SQLite::Statement   query2(db, "INSERT INTO playlist_musicas"
                                       "(fk_musica, fk_playlist, ordem) "
                                       "VALUES( (SELECT last_insert_rowid()) , ?, (SELECT MAX(ordem) "
                                            "FROM playlist_musicas WHERE fk_playlist= ?) )");

        query2.bind(1, id);
        query2.bind(2, id);
        query2.exec();

        this->playlistRecarregar();
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Incluir Música", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void MainWindow::musicaAlterar(QString str){
    int playlistId = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();
    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();
    int id = musica->musicaId;
//    QString str = dlgMusica->getTitulo();

    try
    {
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "UPDATE musicas SET titulo = ? WHERE musica_id = ?");
        query.bind(1, str.toUtf8().data() );
        query.bind(2, id);
        query.exec();

        this->playlistRecarregar();
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Alterar Música", e.what());
        qDebug() << "exception: " << e.what();
    }
}

QString MainWindow::getQStageDir()
{
    return QDir::homePath() + "/.config/QStage";
}

QString MainWindow::getQStageDatabase()
{
    return getQStageDir()  + "/qstage.db";
}

void MainWindow::on_action_Musica_Nova_triggered()
{
    dlgMusica->show();
}

void MainWindow::on_action_Musica_Editar_triggered()
{
    if(ui->listWidget->selectedItems().count() == 0){
        QMessageBox::warning(this, "Alterar Música", "Selecione uma música para alterar.");
        return;
    }

    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();
    dlgMusica->setTitulo(musica->titulo);
    dlgMusica->setModoInclusao(false);
    dlgMusica->show();
}

void MainWindow::on_action_Musica_Remover_triggered()
{
    if(ui->listWidget->selectedItems().count() == 0){
        QMessageBox::warning(this, "Remover Música da Playlist", "Selecione uma música para remover da playlist.");
        return;
    }

    int playlistId = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();
    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();
    int id = musica->musicaId;
    QMessageBox::StandardButton question = QMessageBox::question(this,
                                                                 "Remover Música da Playlist",
                                                                 "Remover a música: " + musica->titulo,
                                                                 QMessageBox::Yes | QMessageBox::No);

    if(question != QMessageBox::Yes)
        return;

    try
    {
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "DELETE FROM playlist_musicas WHERE fk_musica = ? AND fk_playlist = ?");
        query.bind(1, id);
        query.bind(2, playlistId);
        query.exec();

        this->playlistRecarregar();
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Remover Música", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void MainWindow::on_action_Adicionar_Musica_na_Playlist_triggered()
{
    dlgMusicaExistente->show();
}

void MainWindow::musicaExistenteIncluirNaPlaylist(int musicaId){
    int playlistId = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();

    try
    {
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
        SQLite::Statement   query(db, "INSERT INTO playlist_musicas (fk_musica, fk_playlist) VALUES(?, ?)");
        query.bind(1, musicaId);
        query.bind(2, playlistId);
        query.exec();

        this->playlistRecarregar();
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Remover Música", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void MainWindow::reordenar(int posicao){
    if(ui->listWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, "Reordenar", "Selecione uma música para ordenar na playlist.");
        return;
    }

//    int linha = ui->listWidget->currentRow();

//    if(linha+posicao > ui->listWidget->count() || linha-posicao < 0)
//        return;

    try
    {
        QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
        int currentRow = ui->listWidget->row(selecionado) ;
        if( currentRow + posicao >=0 && currentRow + posicao < ui->listWidget->count() ){
            QListWidgetItem * currentItem = ui->listWidget->takeItem(currentRow);
            ui->listWidget->insertItem(currentRow + posicao, currentItem);
//            ui->listWidget->setCurrentRow(currentRow + posicao);
        } else
            return;

        int playlistId = ui->playlist->itemData(ui->playlist->currentIndex()).value<int>();
        for(int i=0; i<ui->listWidget->count(); i++){

            QListWidgetItem * item = ui->listWidget->item(i);
            Musica * musica = item->data(Qt::UserRole).value<Musica*>();
            int musicaId = musica->musicaId;
            int ordem = i+1;

            SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READWRITE);
            SQLite::Statement query(db, "UPDATE playlist_musicas SET ordem = ? WHERE fk_musica = ? AND fk_playlist = ?");
            query.bind(1, ordem );
            query.bind(2, musicaId);
            query.bind(3, playlistId);
            query.exec();
        }

        this->playlistRecarregar();
//        ui->listWidget->findItems(musica->titulo, Qt::MatchEndsWith)[0]->setSelected(true);
        ui->listWidget->setCurrentRow(currentRow + posicao);
    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao Reordenar Música", e.what());
        qDebug() << "exception: " << e.what();
    }
}

void MainWindow::on_actionMoverParaCima_triggered()
{
    reordenar(-1);

}

void MainWindow::on_actionMoverParaBaixo_triggered()
{
    reordenar(1);
}

QString MainWindow::getConfig(QString key){
    try
    {
        SQLite::Database db(QString(qstageDir + "/qstage.db").toUtf8().data(), SQLite::OPEN_READONLY);
        SQLite::Statement   query(db, "SELECT val FROM config WHERE key = ? ");
        query.bind(1, key.toUtf8().data());
        while(query.executeStep()){
        const char *val = query.getColumn(0);
            qDebug() << val << QString(val);
            return QString(val);
        }


    }
    catch (std::exception& e)
    {
        QMessageBox::warning(this,"Erro ao obter configuração", e.what());
        qDebug() << "exception: " << e.what();
    }
}

bool MainWindow::tentarAutoConectar(QString porta){
    if(jack->connect() != 0)
        return false;

    QList<QString> *lista = jack->listarPortas();
    qDebug() <<  "Tentando auto conexao na porta preferencial: " << porta ;
    for(int i=0; i< lista->length(); i++) {
        if(lista->at(i).compare(porta) == 0){
            if(jack->conectarNaPorta(porta))
                return true;
            else
                return false;
        }
    }

    return false;

}

void MainWindow::on_actionTagMusica_triggered()
{
    if(ui->listWidget->selectedItems().length() == 0){
        QMessageBox::warning(this, "Etiquetar Música","Selecione uma música para etiquetar.");
        return;
    }

    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();

    QColor cor = QColorDialog::getColor(QColor(selecionado->backgroundColor().rgb()), this);
    if (!cor.isValid())
        return;

    try {
        Controller::updateMusicaTag(musica->musicaId, cor.rgb());
    } catch (std::exception &e) {
        QMessageBox::warning(this, "Tag", e.what());
    }
    selecionado->setBackgroundColor(cor);

}
