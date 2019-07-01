#ifndef FORMQSTUDIO_H
#define FORMQSTUDIO_H

#include <QDialog>
#include <QFile>
#include <QString>

extern "C" {
#include <jack/jack.h>
#include <jack/types.h>
#include <jack/ringbuffer.h>
#include <jack/midiport.h>
#include <smf.h>
}

namespace Ui {
class FormQStudio;
}

class FormQStudio : public QDialog
{
    Q_OBJECT

public:
    explicit FormQStudio(QWidget *parent = nullptr);
    ~FormQStudio();

    void playMIDIFile(QString file);

private slots:
    void on_pushButton_clicked();

private:
    Ui::FormQStudio *ui;
};

#endif // FORMQSTUDIO_H
