#ifndef DIALOGTEMAEDITOR_H
#define DIALOGTEMAEDITOR_H

#include "Mainwindow.h"
#include <QDialog>

namespace Ui {
class DialogTemaEditor;
}

class DialogTemaEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTemaEditor(MainWindow *w, QWidget *parent = nullptr);

    ~DialogTemaEditor();

private slots:
    void on_pushButton_3_clicked();

    void on_cmbTemas_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_btnRemover_clicked();

private:
    Ui::DialogTemaEditor *ui;
    MainWindow *w;

    void carregarTemas();
};

#endif // DIALOGTEMAEDITOR_H
