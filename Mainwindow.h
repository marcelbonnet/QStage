#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include "MidiControl.h"
#include "parttab.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "musica.h"
#include <QString>
#include "dialog_playlist.h"
#include "dialogmusica.h"
#include "dialogmusicaexistente.h"
#include "dialogdocumenteditor.h"
#include <QAction>
#include <QList>
#include "SysExMessage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void abrirPrograma();
    bool watchFile(QString path);

    static QString getQStageDir();
    static QString getQStageDatabase();


    QString getConfig(QString key);

private slots:
    void on_listWidget_itemSelectionChanged();

    void on_perfBtnEnviar_clicked();

    void on_perfIntervaloNotas_clicked();

    void on_actionConectar_triggered();

    void on_actionSalvar_SYSEX_triggered();

    void on_actionAbrir_SYSEX_triggered();

    void on_perfEfeito_currentIndexChanged(int index);

    void on_actionCarregar_Proxima_triggered();

    void on_actionCarregar_Anterior_triggered();

    void on_actionAtualizar_Playlists_triggered();

    void on_playlist_currentIndexChanged(int index);

    void on_htmlFileChanged();

    void on_actionEditar_HTML_GVim_triggered();

    void onEdicaoHTMLEncerrada(int musicaId, QString html);

    void on_action_Playlist_Nova_triggered();

    void playlistIncluir(QString str);

    void playlistAlterar(int id, QString str);

    void playlistRemover(int id);

    void on_action_Playlist_Remover_triggered();

    void on_action_Playlist_Editar_triggered();

    void on_action_Playlist_Duplicar_triggered();

    void on_action_Musica_Nova_triggered();

    void on_action_Musica_Editar_triggered();

    void on_action_Musica_Remover_triggered();

    void musicaIncluir(QString str);

    void musicaAlterar(QString str);

    void on_action_Adicionar_Musica_na_Playlist_triggered();

    void musicaExistenteIncluirNaPlaylist(int musicaId);

    void on_actionMoverParaCima_triggered();

    void on_actionMoverParaBaixo_triggered();

    void setTema(QAction * action);

    void on_actionTagMusica_triggered();

    void on_btnFilterEnvelope_toggled(bool checked);

    void on_btnLevel_toggled(bool checked);

    void on_btnController_toggled(bool checked);

    void on_perfOrigem_currentIndexChanged(int index);

    void partUtilsCopiarPerformancePartParaPart(int parteOrigem, int parteDestino);

private:
    Ui::MainWindow *ui;
    dialog_playlist *dlgPlaylist;
    DialogMusica *dlgMusica;
    DialogMusicaExistente *dlgMusicaExistente;
    DialogDocumentEditor *dlgEditorHTML;
    QString qstageDir;
    QString configMusicasDir;
    QString configSysExDir;

    /**
     * @brief carregarHTML carrega o HTML no Widget
     */
    void carregarHTML();
    /**
     * @brief restaurarSelecao
     * Selecionar playlist e música selecionadas antes
     * de uma determinada ação.
     */
    void restaurarSelecao(int playlistIndex, int musicaIndex);
    //recarrega combo de playlist
    void playlistRecarregar();

    /**
      @deprecated do libre office
     * @brief editarHTML
     * @param binPath
     */
    void editarHTML(QString binPath);
    void reordenar(int posicao);
    bool tentarAutoConectar(QString porta);



    PartTab *tabParts[16];    //Tab
                            //TODO Receive Switch
    QSpinBox *partMidiCh[16]; //MIDI CHANNEL


 protected:
    MidiControl *jack = NULL;

    QString path;

    QList<SysExMessage*> *mensagens = new QList<SysExMessage*>();


};

#endif // MAINWINDOW_H
