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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    MainWindow::showMaximized();
    ui->setupUi(this);

    this->jack= new MidiControl();


    //caso contrário o programa quebra ao tentar transmitir sinais quando carregar configurações
    on_actionConectar_triggered();

    qstageDir = QDir::homePath() + "/.config/QStage";
    configMusicasDir = QDir::homePath() + "/.config/QStage/songs"; // QStandardPaths::locate(QStandardPaths::HomeLocation, "songs", QStandardPaths::LocateFile);
    configSysExDir = QDir::homePath() + "/.config/QStage/XP-30";

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
    this->on_actionAtualizar_Playlists_triggered();
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




//    ui->webView->setHtml(QString("<strong>TESTE</strong><h1>OK</h1>"));// load(QUrl(QString("file://" + file )));



}

void MainWindow::on_listWidget_itemSelectionChanged()
{
    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();

//    loadTextFile("");
    ui->webView->setHtml(musica->html );
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
    if(ui->listWidget->selectedItems().length() <= 0){
        //esse workflow está ruim. nem deveria mexer nas partes sem uma música selecionada
        //ao selecionar vai apagar as mudanças
        QMessageBox msg;
        msg.setText("Selecione uma música da lista");
        msg.exec();
        return;
    }

    //QString diretorio = QFileDialog::getExistingDirectory();
    //QSettings *conf = new QSettings(diretorio.append("/teste.ini"), QSettings::IniFormat);
    QString arquivoSysx = QString(configSysExDir).append("/" + ui->listWidget->selectedItems()[0]->text() + ".ini");

    if(QFile(arquivoSysx).exists()){
        QMessageBox::StandardButton resposta = QMessageBox::question(this, "Sobrescrever SYSX", "O arquivo SYSX já existe.\nSobrescrever?", QMessageBox::Yes | QMessageBox::No);
        if(resposta != QMessageBox::Yes)
            return;
    }

    QSettings *conf = new QSettings(arquivoSysx, QSettings::IniFormat);
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
}

void MainWindow::on_actionAbrir_SYSEX_triggered()
{

    QListWidgetItem * selecionado = ui->listWidget->selectedItems()[0];
    Musica * musica = selecionado->data(Qt::UserRole).value<Musica*>();

    //qDebug() << QString(configSysExDir).append("/" + ui->listWidget->selectedItems()[0]->text() + ".ini");

    //QSettings *conf = new QSettings("/tmp/teste.ini", QSettings::IniFormat);
//    QSettings *conf = new QSettings(QString(configSysExDir).append("/" + ui->listWidget->selectedItems()[0]->text() + ".ini"), QSettings::IniFormat);
    QSettings *conf = new QSettings( musica->programa , QSettings::IniFormat);
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

void MainWindow::atualizarListaDeMusicas(){
    if(this->ui->playlist->count() > 0)
        this->on_playlist_currentIndexChanged(0);
}

void MainWindow::on_actionAtualizar_Playlists_triggered()
{
    //    auto f = [](int a, int b) -> int { return a + b; };
    //    auto n = f(1, 2);

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
        SQLite::Database db(QString(qstageDir + "/qstage.db").toLatin1().data());
        SQLite::Statement   query(db, "SELECT id, titulo FROM playlist ORDER BY id ASC");

        while (query.executeStep())
        {
            int         id      = query.getColumn(0);
            const char* titulo   = query.getColumn(1);

            ui->playlist->addItem(QString(titulo), QVariant::fromValue(id));
        }

        this->atualizarListaDeMusicas();

    }
    catch (std::exception& e)
    {
        qDebug() << "exception: " << e.what();
        QMessageBox::warning(this, "Erro ao Atualizar Playlists", e.what());
    }

    connect(ui->playlist,SIGNAL(currentIndexChanged(int)), this
               , SLOT(on_playlist_currentIndexChanged(int)));
    connect(ui->listWidget,SIGNAL(itemSelectionChanged()), this
               , SLOT(on_listWidget_itemSelectionChanged()));

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


        SQLite::Database db(QString(qstageDir + "/qstage.db").toLatin1().data());
        SQLite::Statement   query(db, "SELECT m.musica_id, m.titulo, m.html, pm.ordem, m.programa "
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
    std::string str = std::string(this->path.toLatin1().data());
    system( str.c_str() );
}

bool MainWindow::watchFile(QString path){
//    if (Kqueue::watch(path))
//        qDebug() << "arquivo " << path << " foi modificado";
//    else
//        qDebug() << "arquivo " << path << " não sofreu alterações.";
    return Kqueue::watch(path);
}

void MainWindow::on_actionEditar_HTML_triggered(){
    this->editarHTML(QString("/usr/local/bin/libreoffice --nologo --writer "));
}

void MainWindow::on_actionEditar_HTML_GVim_triggered()
{
     this->editarHTML(QString("/usr/local/bin/geany "));
}


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
    const char * tmp =  + file.fileName().toLatin1().data();


    std::string str = std::string(binPath.toUtf8().data()) + file.fileName().toUtf8().data();
    system( str.c_str() );

    //if(Kqueue::watch(file.fileName())){ //não vai perceber a modificação realizada antes
        //persistir HTML no banco de dados
        try
        {
            SQLite::Database db(QString(qstageDir + "/qstage.db").toLatin1().data()
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
        std::string str = std::string("/usr/local/bin/libreoffice --writer ") + file.fileName().toLatin1().data();
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
    std::string str = std::string("/usr/local/bin/libreoffice --writer ") + file.fileName().toLatin1().data();
    this->path = QString("/usr/local/bin/libreoffice --writer " + file.fileName());
    std::future<void> threadAbrirPrograma = std::async(std::launch::async, &MainWindow::abrirPrograma , this);
    threadAbrirPrograma.get();

    if(threadKqueue.get()){
        qDebug() << "modificou, pode enfiar o HTML no banco de dados";
        try
        {
            SQLite::Database db(QString(qstageDir + "/qstage.db").toLatin1().data());
            SQLite::Statement   query(db, "UPDATE musicas SET html = ? WHERE musica_id = ?");

            query.bind(0, musica->html.toLatin1().data() );
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

