#ifndef PATCHUI_H
#define PATCHUI_H

#include <QWidget>
#include <QScrollArea>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include <QList>
#include <QTabWidget>

namespace Ui {
class PatchUI;
}

class PatchUI : public QWidget
{
    Q_OBJECT

public:
    explicit PatchUI(QWidget *parent = 0);
    ~PatchUI();

private:
    Ui::PatchUI *ui;

    QGridLayout *grid;
    QScrollArea *scrollArea;
    QList<QWidget*> *lista = new QList<QWidget*>();

};

#endif // PATCHUI_H
