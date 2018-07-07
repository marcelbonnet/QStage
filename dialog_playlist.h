#ifndef DIALOG_PLAYLIST_H
#define DIALOG_PLAYLIST_H

#include <QDialog>
#include <QString>

namespace Ui {
class dialog_playlist;
}

class dialog_playlist : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_playlist(QWidget *parent = 0);
    ~dialog_playlist();

    int getId() const;
    void setId(int value);

    QString getNome() const;
    void setNome(const QString &value);

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

signals:
    void registrarPlaylist(QString);
    void alterarPlaylist(int, QString);

private:
    Ui::dialog_playlist *ui;
    int id = 0;
    QString nome;
};

#endif // DIALOG_PLAYLIST_H
