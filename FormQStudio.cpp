#include "FormQStudio.h"
#include "ui_FormQStudio.h"
#include <QDateTime>
#include <QDebug>
#include <QtMath>
#include <QThread>

#include <unistd.h>


#define CLIENTE "QStudio"
#define OUT_PORT_NAME "out"

FormQStudio::FormQStudio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormQStudio)
{
    ui->setupUi(this);
}

FormQStudio::~FormQStudio()
{
    delete ui;
}

int FormQStudio::processJackCallback(unsigned int nframes, void *arg){
    Cliente *cli = (Cliente*) arg;

    cli->sendMIDI("", nframes);
    return 1;
}



void FormQStudio::playMIDIFile(QString file) noexcept(false){
    //    int (*funcCallback)(jack_nframes_t, void*);

    //    funcCallback = &processJackCallback;

    smf_t *smf = smf_load(file.toUtf8());
    smf_event_t *event;

    ClientePorta *cp = new ClientePorta("out1",2048,ClientePorta::MIDI, ClientePorta::OUTPUT);
    Cliente *c = new Cliente("QStudio");

    c->addClientePorta(cp);

    c->inicializarCliente(FormQStudio::processJackCallback);
    //lambda:
    //    c->inicializarCliente([&](jack_nframes_t nframes, void *arg){

    //        Cliente *cli = (Cliente*) arg;
    //        cli->sendMIDI(cp->getNomePorta(), nframes);
    //    });


    //    if (smf == NULL) {
    //        throw QStageException("Erro ao abrir o arquivo MIDI");
    //    }

    //    while ((event = smf_get_next_event(smf)) != NULL) {
    //        if (smf_event_is_metadata(event))
    //            continue;

    //        //wait until event->time_seconds.
    //        //feed_to_midi_output(event->midi_buffer, event->midi_buffer_length);
    //        struct Cliente::MidiMessage ev;

    //        ev.time = jack_frame_time(c->getClient());

    //        int written;

    //        if (jack_ringbuffer_write_space(cp->getRingBuffer()) < sizeof(ev)) {
    //            throw new QStageException("Sem espaço no ring buffer");
    //        }

    //        written = jack_ringbuffer_write(cp->getRingBuffer(), (char *)&ev, sizeof(ev));

    //        if (written != sizeof(ev))
    //            throw new QStageException("jack_ringbuffer_write failed, NOTE %i LOST." + ev.data[1] );
    //    }

}

void FormQStudio::on_pushButton_clicked()
{
    //    playMIDIFile("/studio/OLD/Estudo/Paul\ McCartney\ MIDIs/LondonTown.mid");
    //GM usado de teste, partes de 1 a 16:
    // 006 034 001 057 001 030 001 001 ...
    /*
     * prova de conceito
     * */
    provaConceitoInicializarCliente();
    provaConceitoConectar("FastTrack Pro:midi.RX");
    provaConceitoConectar("MIDI monitor:midi_in");

    QThread* thread = new QThread;
    WorkerReadSMF* worker = new WorkerReadSMF(this);
    worker->moveToThread(thread);
    // "this" não é um QObject para implementar errorString(QString)
    //QObject::connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(process()));
    QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();



}


/* ************************************************************
 * ************************************************************
 * ************************************************************
 * ************************************************************
 * ************************************************************
 * ************************************************************/

void FormQStudio::provaConceitoInicializarCliente() noexcept(false){
    int err;


    jack_client = jack_client_open(CLIENTE, JackNoStartServer, NULL);

    if (jack_client == NULL) {
        throw new QStageException("jack client nulo");
    }

    //    ringbuffer = jack_ringbuffer_create(RINGBUFFER_SIZE*2);
    ringbuffer = jack_ringbuffer_create(1024*sizeof(struct FormQStudio::MidiMessage) *2);

    if (ringbuffer == NULL) {
        throw new QStageException("ring buffer nulo");
    }

    jack_ringbuffer_mlock(ringbuffer);

    err = jack_set_process_callback(jack_client, FormQStudio::provaConceitoProcessCallback , this);

    if (err) {
        throw new QStageException("erro ao processar callback");
    }

    output_port = jack_port_register(jack_client, OUT_PORT_NAME, JACK_DEFAULT_MIDI_TYPE,
                                     JackPortIsOutput, 0);

    if (output_port == NULL) {
        throw new QStageException("não registrou a porta de saída");
    }

    if (jack_activate(jack_client)) {
        throw new QStageException("não conseguiu ativar o cliente");
    }

}

void FormQStudio::provaConceitoConectar(QString nomePortaDestino)noexcept(false){
    QString nomePortaOrigem =   QString("%1:%2").arg(CLIENTE).arg(OUT_PORT_NAME);
    int ret = jack_connect(jack_client, nomePortaOrigem.toLatin1().data() , nomePortaDestino.toLatin1().data());
    if(ret != 0)
        throw new QStageException("não conectou o cliente na porta");
}

//double FormQStudio::provaConceitoNframes_to_ms(jack_nframes_t nframes){
//    jack_nframes_t sr;

//    sr = jack_get_sample_rate(jack_client);

//    assert(sr > 0);

//    return ((nframes * 1000.0) / (double)sr);
//}

//struct FormQStudio::MidiMessage * FormQStudio::provaConceito_midi_message_from_midi_event(jack_midi_event_t event)noexcept(false){
//    FormQStudio::MidiMessage * ev = new FormQStudio::MidiMessage();

//    if (ev == NULL) {
//        throw new QStageException("MidiMessage está nula");
//    }

//    assert(event.size >= 1 && event.size <= 3);

//    ev->len = event.size;
//    ev->time = event.time;

//    memcpy(ev->data, event.buffer, ev->len);

//    return (ev);
//}
void FormQStudio::provaConceito_process_midi_output(jack_nframes_t nframes) noexcept(false){
    int read, t, bytes_remaining;
    unsigned char *buffer;
    void *port_buffer;
    jack_nframes_t last_frame_time;
    struct FormQStudio::MidiMessage ev;

    last_frame_time = jack_last_frame_time(jack_client);

    port_buffer = jack_port_get_buffer(output_port, nframes);
    if (port_buffer == NULL) {
        throw new QStageException("port_buffer nulo");
    }

    jack_midi_clear_buffer(port_buffer);//mandatory call at the begining, only for output callback

    /* We may push at most one byte per 0.32ms to stay below 31.25 Kbaud limit. */
    //    bytes_remaining = nframes_to_ms(nframes) * rate_limit;

    while (jack_ringbuffer_read_space(ringbuffer)) {
        //inspect data on ringbuffer
        read = jack_ringbuffer_peek(ringbuffer, (char *)&ev, sizeof(ev));

        if (read != sizeof(ev)) {
            printf("Short read from the ringbuffer, possible note loss.");
            jack_ringbuffer_read_advance(ringbuffer, read); //read and advance the buffer pointers
            continue;
        }

        bytes_remaining -= ev.len;

        //        if (rate_limit > 0.0 && bytes_remaining <= 0) {
        //            printf("Rate limiting in effect.");
        //            break;
        //        }

        //compute Message event time
        t = ev.time + nframes - last_frame_time;

        /* If computed time is too much into the future, we'll need
           to send it later. */
        if (t >= (int)nframes)
            break;

        /* If computed time is < 0, we missed a cycle because of xrun. */
        if (t < 0)
            t = 0;

        //        if (time_offsets_are_zero)
        t = 0;

        jack_ringbuffer_read_advance(ringbuffer, sizeof(ev));//read and advance the buffer pointers

        buffer = jack_midi_event_reserve(port_buffer, t, ev.len);//Clients are to write the actual event data to be written starting at the pointer returned by this function.

        if (buffer == NULL) {
            throw new QStageException("Buffer nulo");
        }

        memcpy(buffer, ev.data, ev.len);//write actual event into reserved pointer buffer
    }
}

int FormQStudio::provaConceitoProcessCallback(jack_nframes_t nframes, void *arg)noexcept(false){
    FormQStudio * obj = (FormQStudio*) arg;
    /* Check for impossible condition that actually happened to me, caused by some problem between jackd and OSS4. */
    if (nframes <= 0) {
        printf("Process callback called with nframes = 0; bug in JACK?");
        return 0;
    }

    //process_midi_input(nframes);
    obj->provaConceito_process_midi_output(nframes);
    return (0);
}

void FormQStudio::provaConceito_queue_message(int b0, int b1, int b2) noexcept(false)
{
    struct FormQStudio::MidiMessage ev;

    /* For MIDI messages that specify a channel number, filter the original
       channel number out and add our own. */
    /*
     *
    if (b0 >= 0x80 && b0 <= 0xEF) {
        b0 &= 0xF0;
        b0 += channel;
    }
     */

    if (b1 == -1) {
        ev.len = 1;
        ev.data[0] = b0;

    } else if (b2 == -1) {
        ev.len = 2;
        ev.data[0] = b0;
        ev.data[1] = b1;

    } else {
        ev.len = 3;
        ev.data[0] = b0;
        ev.data[1] = b1;
        ev.data[2] = b2;
    }

    ev.time = jack_frame_time(jack_client);

    int written;


    if (jack_ringbuffer_write_space(ringbuffer) < sizeof(ev)) {
        qDebug() << "o ring buffer encheu";
    }

    written = jack_ringbuffer_write(ringbuffer, (char *)&ev, sizeof(ev));

    if (written != sizeof(ev))
        throw new QStageException("jack_ringbuffer_write failed, NOTE %i LOST." + ev.data[1] );
}

WorkerReadSMF::WorkerReadSMF(FormQStudio *qst)
{
    this->qst = qst;
}

void WorkerReadSMF::process(){

    QString file("/studio/OLD/Estudo/Paul\ McCartney\ MIDIs/LondonTown.mid");
    smf_t *smf = smf_load(file.toUtf8());
    smf_event_t *event;

    int tempoUtltimoEvento = 0;
//    qint64 tempoInicio = QDateTime::currentDateTime().toMSecsSinceEpoch();
    while ((event = smf_get_next_event(smf)) != NULL) {
        if (smf_event_is_metadata(event) || smf_event_is_sysex(event) )
            continue;

//tentarDeNovo:
//        qint64 agora = QDateTime::currentDateTime().toMSecsSinceEpoch() ;
//qDebug() << tempoInicio << agora << (tempoInicio + floor(event->time_seconds * 1000)) << floor(event->time_seconds * 1000);

//        if(tempoInicio + floor(event->time_seconds * 1000) < agora )
//            goto tentarDeNovo;

//        usleep( ( (agora - tempoInicio) + floor(event->time_seconds * 1000)) );
        int agora = floor(event->time_seconds * 1000);
        usleep( (agora - tempoUtltimoEvento)*1000 );
        qst->provaConceito_queue_message(
                    event->midi_buffer[0]
                ,event->midi_buffer[1]
                , event->midi_buffer[2] );


//        tempoUtltimoEvento = agora;
         tempoUtltimoEvento = floor(event->time_seconds * 1000);

    }

    emit finished();
}
