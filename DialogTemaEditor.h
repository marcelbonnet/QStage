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

private:
    Ui::DialogTemaEditor *ui;
    MainWindow *w;
};

#endif // DIALOGTEMAEDITOR_H
