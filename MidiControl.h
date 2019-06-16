#ifndef MIDICONTROL_H
#define MIDICONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sysexits.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/inotify.h>

extern "C" {
#include <jack/jack.h>
#include <jack/types.h>
#include <jack/ringbuffer.h>
#include <jack/midiport.h>
}

#include <QString>
#include <QList>
#include <QDateTime>
#include <QMap>

#include "SysExMessage.h"

#include "FormSerialMidi.h"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )			/*inotify event*/
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )		/*inotify events buffer size*/

#define VELOCITY_MAX		127
#define VELOCITY_NORMAL		64	//0x40
#define VELOCITY_MIN		1

#define OUTPUT_PORT_NAME	"midi_out"
#define INPUT_PORT_NAME		"midi_in"
#define PACKAGE_NAME		"QStage"
#define PACKAGE_VERSION		"2.0.0"



#define NNOTES			128

#define MIDI_NOTE_ON		0x90
#define MIDI_NOTE_OFF		0x80
#define MIDI_PROGRAM_CHANGE	0xC0
#define MIDI_CONTROLLER		0xB0
#define MIDI_RESET		0xFF
#define MIDI_HOLD_PEDAL		64
#define MIDI_ALL_SOUND_OFF	120
#define MIDI_ALL_MIDI_CONTROLLERS_OFF	121
#define MIDI_ALL_NOTES_OFF	123	//7B
#define MIDI_BANK_SELECT_MSB	0
#define MIDI_BANK_SELECT_LSB	32

#define BANK_MIN		0
#define BANK_MAX		127
#define PROGRAM_MIN		0
#define PROGRAM_MAX		127
#define CHANNEL_MIN		1
#define CHANNEL_MAX		16

#define RINGBUFFER_SIZE		1024*sizeof(struct MidiControl::MidiMessage)

/* Will emit a warning if time between jack callbacks is longer than this. */
#define MAX_TIME_BETWEEN_CALLBACKS	0.1

/* Will emit a warning if execution of jack callback takes longer than this. */
#define MAX_PROCESSING_TIME	0.01



class MidiControl
{
public:
    MidiControl();

    struct MidiMessage {
        jack_nframes_t	time;
        int		len;	/* Length of MIDI message, in bytes. */
        unsigned char	data[3];
    };

    int contadorMensagensEnviadas = 0; //para saber quando fazer um intervalo entre pacotes de mensagens
    double	rate_limit = 0.0;
    int		time_offsets_are_zero = 0; //Send all MIDI messages with zero time offset, making them play as soon as they reach the synth.

    jack_port_t	*output_port;
    jack_port_t	*input_port;
    jack_client_t	*jack_client = NULL;
    jack_ringbuffer_t *ringbuffer;
    /**
     * @brief mensagens exclusivas recebidas pelo QStage para serem lidas e apagadas quando necessário
     */
    QList<int> *sysxin;

    double nframes_to_ms(jack_nframes_t nframes);

    static int processCallback(jack_nframes_t nframes, void *arg);
    int (MidiControl::*func)(jack_nframes_t nframes, void *notused);// <- declare by saying what class it is a pointer to
    int connect();
    void desconectar(QString nomePortaDestino);
    bool conectarNaPorta(QString nomePortaDestino);
    bool conectarQStageMidiInEm(QString origem);
    struct MidiControl::MidiMessage * midi_message_from_midi_event(jack_midi_event_t event);
    void post_process_midi_input(struct MidiControl::MidiMessage *ev);
    void process_midi_output(jack_nframes_t nframes);
    void process_midi_input(jack_nframes_t nframes);
    void queue_message(struct MidiControl::MidiMessage *ev);
    void queue_new_message(int b0, int b1, int b2);



    void transmitir();//envia as mensagens da QList para queue_new_message. Esvazia a QList no final

    void tx(QList<SysExMessage*> *sxs);
    int calcularChecksum(int endereco, QList<int> *data);
    int calcularChecksum(int endereco, int dado);
    /**
     * @brief Faz o envio do pacote e clean do ponteiro que contém os dados
     * @param addr endereço inicial da página de dados a ser modificada
     * @param data lista de dados em sequência
     */
    void txPacoteDataSet(int addr, QList<int> *data);
    void txPacoteRequestData(int addr, int data);

    QList<QString> *listarPortas();

    void startSerialMidi(FormSerialMidi *smidi);

    enum Performance {
        //0x00 0x00
        Common,
        //0x10 0x00 ... 0x1F 0x00
        Part1, Part2, Part3, Part4, Part5, Part6, Part7, Part8, Part9, Part10, Part11, Part12, Part13, Part14, Part15, Part16
    };

    enum PerformanceCommon {
        //0x00 0x00 ... 0x00 0x0B ( 0 a 11 decimal)
        Name,
        //efeitos


    };



protected:
    QList<SysExMessage> *sxm = new QList<SysExMessage>();
    QList<int> *mensagens = new QList<int>();
    QDateTime tempoUltimoEnvio;
    int qtdeMensagensExclusivas = 0;

    int ultimaMensagem[13] = { -1, -1, -1,-1, -1, -1,-1, -1, -1,-1, -1, -1, -1 };
    int ultimaMensagemIndex = 0;
    QList<QList<int>> *mensagensTransmitidas = new QList<QList<int>>();




private:
    //usada pelo post_process_midi_input para saber quando guardar cópia das mensagens recebidas
    bool midiInputDoTipoSysEx = false;
    //marcador para saber se estamos no primeiro trio de bytes de header
    bool midiInputHeader1 = false;
    //marcador para saber se ainda estamos recebendo o segundo trio de header de sysex (0x6a 0x12 [data])
    bool midiInputHeader2 = false;


};

#endif // MIDICONTROL_H
