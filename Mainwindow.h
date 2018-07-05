#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include "MidiControl.h"
#include "parttab.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "musica.h"
#include <QString>

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

    void on_actionEditar_HTML_triggered();

    void on_htmlFileChanged();

    void on_actionEditar_HTML_GVim_triggered();

private:
    Ui::MainWindow *ui;
    QString qstageDir;
    QString configMusicasDir;
    QString configSysExDir;
    void loadTextFile(QString file);
    void atualizarListaDeMusicas();
    void editarHTML(QString binPath);

    PartTab *tabParts[16];    //Tab
                            //TODO Receive Switch
    QSpinBox *partMidiCh[16]; //MIDI CHANNEL


 protected:
    MidiControl *jack = NULL;

    QString path;
};

#endif // MAINWINDOW_H
