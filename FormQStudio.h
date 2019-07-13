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

#include "Cliente.h"
#include "ClientePorta.h"
#include "QStageException.h"

namespace Ui {
class FormQStudio;
}

class FormQStudio : public QDialog
{
    Q_OBJECT

public:
    explicit FormQStudio(QWidget *parent = nullptr);
    ~FormQStudio();

    struct MidiMessage {
        jack_nframes_t	time;
        int		len;	/* Length of MIDI message, in bytes. */
        unsigned char	data[3];
    };

    void playMIDIFile(QString file) noexcept(false);

    static int processJackCallback(jack_nframes_t nframes, void *arg) noexcept(false);


    void provaConceitoInicializarCliente() noexcept(false);
    void provaConceitoConectar(QString nomePortaDestino)noexcept(false);
//    double provaConceitoNframes_to_ms(jack_nframes_t nframes);
    static int provaConceitoProcessCallback(jack_nframes_t nframes, void *arg)noexcept(false);
    void provaConceitoDesconectar(QString nomePortaDestino);
    struct FormQStudio::MidiMessage * provaConceito_midi_message_from_midi_event(jack_midi_event_t event)noexcept(false);
//    int provaConceito_post_process_midi_input(struct FormQStudio::MidiMessage *ev)noexcept(false);
    void provaConceito_process_midi_output(jack_nframes_t nframes);
    void provaConceito_queue_message(int b0, int b1, int b2) noexcept(false);


private slots:
    void on_pushButton_clicked();

private:
    Ui::FormQStudio *ui;

    //atributos para a prova de conceito do SMF
    jack_port_t	*output_port;
    jack_port_t	*input_port;
    jack_client_t	*jack_client = NULL;
    jack_ringbuffer_t *ringbuffer;

};


class WorkerReadSMF : public QObject
{
    Q_OBJECT
public:
    WorkerReadSMF(FormQStudio *smidi);

public slots:
    void process();

protected:
    FormQStudio *qst;


signals:
    void finished();
    void error(QString err);
};

#endif // FORMQSTUDIO_H
