#include "Cliente.h"
#include <QDebug>

Cliente::Cliente(QString clientName)
{
    this->clientName = clientName;
}


void Cliente::inicializarCliente(JackProcessCallback callback) noexcept(false){
    jackClient = jack_client_open(clientName.toUtf8().data(), JackNoStartServer, NULL);

    if (jackClient == NULL) {
        throw new QStageException("jack client nulo");
    }

    for(ClientePorta *cp : portas ){
        qDebug() << "Iniciando" << cp->getNomePorta() << "Ring Buffer" << cp->getRingBuffer();

        cp->setRingBuffer(jack_ringbuffer_create(cp->getRingBufferSize()));
        jack_ringbuffer_mlock(cp->getRingBuffer());
        qDebug() << "Ring Buffer" << cp->getRingBuffer();

        int err = jack_set_process_callback(jackClient, callback , this);

        if (err) {
            throw new QStageException("erro ao processar callback");
        }

        jack_port_t *p = jack_port_register(jackClient
                                         , cp->getNomePorta().toUtf8().data()
                                         , cp->getTipoPorta() == ClientePorta::AUDIO? JACK_DEFAULT_AUDIO_TYPE : JACK_DEFAULT_MIDI_TYPE
                                         , cp->getDirecaoPorta() == ClientePorta::INPUT ? JackPortIsInput : JackPortIsOutput
                                         , 0);
        ClientePorta *aPorta = portas.value(cp->getNomePorta());
        aPorta->setPorta(p);
        portas.value(cp->getNomePorta(), aPorta);

        if (p == NULL) {
            throw new QStageException("não registrou a porta");
        }


        if (jack_activate(jackClient)) {
            throw new QStageException("não conseguiu ativar o cliente");
        }
    }

}

void Cliente::conectar(QString portaOrigem, QString clienteDestino, QString portaDestino)noexcept(false){
    ClientePorta * cp = getClientePorta(portaOrigem);
    QString nomePortaOrigem =   QString("%1:%2").arg(clientName).arg(portaOrigem);
    QString nomePortaDestino = QString("%1:%2").arg(clienteDestino).arg(portaDestino);
    int ret = jack_connect(jackClient, nomePortaOrigem.toLatin1().data() , portaDestino.toLatin1().data());
    if(ret != 0)
        throw new QStageException("Não conectou o cliente na porta");
}

void Cliente::addClientePorta(ClientePorta *cp){
    portas.insert(cp->getNomePorta(), cp);
}

ClientePorta* Cliente::getClientePorta(QString nomePorta){
    return portas.value(nomePorta);
}

jack_client_t *Cliente::getClient(){
    return this->jackClient;
}

void Cliente::sendMIDI(QString outputPortName, jack_nframes_t nframes){
    ClientePorta *cp = getClientePorta(outputPortName);
    int read;
    unsigned char *buffer;
    void *port_buffer;
    jack_nframes_t last_frame_time;
    struct Cliente::MidiMessage ev;

//    last_frame_time = jack_last_frame_time(getClient());

    qDebug() <<"TESTE" << cp->getNomePorta()  ;

    port_buffer = jack_port_get_buffer(cp->getPorta(), nframes);
    if (port_buffer == NULL) {
        throw new QStageException("port_buffer nulo");
    }

    jack_midi_clear_buffer(port_buffer);//mandatory call at the begining, only for output callback

    while (jack_ringbuffer_read_space(cp->getRingBuffer())) {
        //inspect data on ringbuffer
        read = jack_ringbuffer_peek(cp->getRingBuffer(), (char *)&ev, sizeof(ev));

        if (read != sizeof(ev)) {
            printf("Short read from the ringbuffer, possible note loss.");
            jack_ringbuffer_read_advance(cp->getRingBuffer(), read); //read and advance the buffer pointers
            continue;
        }

        jack_ringbuffer_read_advance(cp->getRingBuffer(), sizeof(ev));//read and advance the buffer pointers

        buffer = jack_midi_event_reserve(port_buffer, 0, ev.len);//Clients are to write the actual event data to be written starting at the pointer returned by this function.

        if (buffer == NULL) {
            throw new QStageException("Buffer nulo");
        }

        memcpy(buffer, ev.data, ev.len);//write actual event into reserved pointer buffer
    }
}
