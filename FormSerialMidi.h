#ifndef FORMSERIALMIDI_H
#define FORMSERIALMIDI_H

#include <QWidget>
#include <QList>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>

extern "C" {
#include <jack/jack.h>
#include <jack/types.h>
#include <jack/ringbuffer.h>
#include <jack/midiport.h>
}

#include "QStageException.h"

#define RINGBUFFER_SIZE		1024*sizeof(struct FormSerialMidi::MidiMessage)

namespace Ui {
class FormSerialMidi;
}

class FormSerialMidi : public QWidget
{
    Q_OBJECT

public:
    explicit FormSerialMidi(QWidget *parent = nullptr);
    ~FormSerialMidi();

    struct MidiMessage {
        jack_nframes_t	time;
        int		len;	/* Length of MIDI message, in bytes. */
        unsigned char	data[3];
    };

    /**
      usará sustain ou não
     */
    bool ignorarNoteOff();
    /**
      todas as notas a serem ativadas para uma nota de dó (0) a si (11)
      Retorna uma lista de inteiros: cada posição é um valor a ser incrementado
      na nota MIDI:
      se foi ativado MIDI NOTA = 48 (dó C4)
      notaRaiz = 0
      se retornar {4, 7} > acionar NOTE ON {48, 48+4, 48+7} (acorde C)
     */
    QList<int>* getNotas(int notaRaiz);

    /**
     * seta se o botão estrá checado ou não
     */
    void setBtnIgnorarNoteOff(bool checked);
    /**
     * set o index da combo de programa pré determinado de notas
     * nota: de 0 a 11
     */
    void setPrograma(int nota, int index);
    /**
      set intervalos da nota
      nota: de 0 a 11
      **/
    void setIntervalos(int nota, QList<int> *notas);

    void inicializarCliente() noexcept(false);
    void conectar(QString nomePortaDestino)noexcept(false);
    double nframes_to_ms(jack_nframes_t nframes);
    static int processCallback(jack_nframes_t nframes, void *arg)noexcept(false);
    void desconectar(QString nomePortaDestino);
    struct FormSerialMidi::MidiMessage * midi_message_from_midi_event(jack_midi_event_t event)noexcept(false);
    int post_process_midi_input(struct FormSerialMidi::MidiMessage *ev)noexcept(false);
    void process_midi_output(jack_nframes_t nframes);
    void queue_message(int b0, int b1, int b2) noexcept(false);


private:
    Ui::FormSerialMidi *ui;
    QPushButton *btnIgnoreNoteOff ;
    QList<QComboBox*> *cmbProgramas;
    QList<QListWidget*> *cmbIntervalos;

    jack_port_t	*output_port;
    jack_port_t	*input_port;
    jack_client_t	*jack_client = NULL;
    jack_ringbuffer_t *ringbuffer;
};

#endif // FORMSERIALMIDI_H
