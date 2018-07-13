#ifndef DIALOGMUSICAEXISTENTE_H
#define DIALOGMUSICAEXISTENTE_H

#include <QDialog>

namespace Ui {
class DialogMusicaExistente;
}

class DialogMusicaExistente : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMusicaExistente(QWidget *parent = 0);
    ~DialogMusicaExistente();

protected:
    void showEvent(QShowEvent *);

private:
    Ui::DialogMusicaExistente *ui;

signals:
    void musicaExistenteSelecionada(int);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // DIALOGMUSICAEXISTENTE_H
