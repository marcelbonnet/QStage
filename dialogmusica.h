#ifndef DIALOGMUSICA_H
#define DIALOGMUSICA_H

#include <QDialog>

namespace Ui {
class DialogMusica;
}

class DialogMusica : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMusica(QWidget *parent = 0);
    ~DialogMusica();

    QString getTitulo() const;
    void setTitulo(const QString &value);

    bool getModoInclusao() const;
    void setModoInclusao(bool value);

    int getModeloId() ;
    void setModeloId(int value);

protected:
    void showEvent(QShowEvent *);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogMusica *ui;
    QString titulo;
    int modeloId = 0;
    bool modoInclusao = true;

signals:
    void incluirMusica(QString);
    void alterarMusica(QString);
};

#endif // DIALOGMUSICA_H
