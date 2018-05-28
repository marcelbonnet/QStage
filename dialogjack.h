#ifndef DIALOGJACK_H
#define DIALOGJACK_H

#include <QDialog>
#include "MidiControl.h"

namespace Ui {
class DialogJack;
}

class DialogJack : public QDialog
{
    Q_OBJECT

public:
    explicit DialogJack(MidiControl *jack, QWidget *parent = 0);
    ~DialogJack();

protected:
    MidiControl *jack;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogJack *ui;
};

#endif // DIALOGJACK_H
