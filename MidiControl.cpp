#include "MidiControl.h"
#include <QString>

#include <QDebug>
#include <stdio.h>
#include <unistd.h>

#include <QThread>
#include <QObject>
#include "WorkerSerialMidi.h"

MidiControl::MidiControl()
{

    sysxin = new QList<int>();

}

double MidiControl::nframes_to_ms(jack_nframes_t nframes){
    jack_nframes_t sr;

    sr = jack_get_sample_rate(jack_client);

    assert(sr > 0);

    return ((nframes * 1000.0) / (double)sr);
}

struct MidiControl::MidiMessage * MidiControl::midi_message_from_midi_event(jack_midi_event_t event){
    MidiControl::MidiMessage * ev = new MidiControl::MidiMessage();

    if (ev == NULL) {
        perror("malloc");
        return (NULL);
    }

    if(event.size < 1 || event.size > 3)
        return NULL;

    ev->len = event.size;
    ev->time = event.time;

    memcpy(ev->data, event.buffer, ev->len);

    return (ev);
}

void MidiControl::process_midi_input(jack_nframes_t nframes){
    int read, events, i;
    void *port_buffer;
    jack_midi_event_t event; //input event we received from jack

    port_buffer = jack_port_get_buffer(input_port, nframes);
    if (port_buffer == NULL) {
        printf("jack_port_get_buffer failed, cannot receive anything.");
        return;
    }

    events = jack_midi_get_event_count(port_buffer);

    for (i = 0; i < events; i++) {
        struct MidiControl::MidiMessage *rev;

        read = jack_midi_event_get(&event, port_buffer, i);
        if (read) {
            printf("jack_midi_event_get failed, RECEIVED NOTE LOST.");
            continue;
        }

        if (event.size > 3) {
            printf("Ignoring MIDI message longer than three bytes, probably a SysEx.");
            continue;
        }

        rev = midi_message_from_midi_event(event);
        if (rev == NULL) {
            printf("midi_message_from_midi_event failed, RECEIVED NOTE LOST.");
            continue;
        }

        post_process_midi_input(rev);
    }
}

void MidiControl::post_process_midi_input(struct MidiControl::MidiMessage *ev){

    if(ev->len == 0)
        return;

    int a = ev->data[0];
    int b = ev->data[1];
    int c = ev->data[2];

    if(a == 0xFE) //active sensing enviado pelo teclado = 254
        return;

    //salvando tudo
    sysxin->append(a);
    sysxin->append(b);
    sysxin->append(c);
    return;

//    //salvando estado do tipo de mensagem que estamos recebendo
//    if(a == 0xF0){
//        midiInputDoTipoSysEx = true;//nesta iteração teremos: a=0xf0 b=0x41 c=0x10
//        midiInputHeader1 = true;
//        return;
//    }

//    //na segunda iteração, teremos a flag de header1 levantada e os seguintes bytes:
//    if(midiInputHeader1 && a == 0x6a && b == 0x12){
//        midiInputHeader1 = false;
//        sysxin->append(c); // c=[data] nesta iteração
//        return;
//    }

//    //se estamos recebendo uma SYSEX, guardar na lista:
//    if(midiInputDoTipoSysEx && !midiInputHeader1 ){
//        //determinar se a sysex terminou
//        if(c == 0xF7){
//            midiInputDoTipoSysEx = false;
//            sysxin->append(a);
//            //b =[checksum]
//            return;
//        }
//        if(b == 0xF7){
//            midiInputDoTipoSysEx = false;
//            //a =[checksum]
//            return;
//        }
//        if(a == 0xF7){
//            midiInputDoTipoSysEx = false;
//            sysxin->removeAt(sysxin->length()-1); //o último byte gravado era um checksum
//            return;
//        }

//        //nesta iteração tudo é data:
//        sysxin->append(a);
//        sysxin->append(b);
//        sysxin->append(c);

//        return;
//    }

//    qDebug() << (midiInputDoTipoSysEx? "SYSEX IN" : "IN") << QString::number(a,16) << QString::number(b,16) << QString::number(c,16);

    //determinar se a sysex terminou
//    if(a == 0xF7 || b == 0xF7 || c == 0xF7)
//        midiInputDoTipoSysEx = false;

}

void MidiControl::process_midi_output(jack_nframes_t nframes){
    int read, t, bytes_remaining;
    unsigned char *buffer;
    void *port_buffer;
    jack_nframes_t last_frame_time;
    struct MidiControl::MidiMessage ev;

    last_frame_time = jack_last_frame_time(jack_client);

    port_buffer = jack_port_get_buffer(output_port, nframes);
    if (port_buffer == NULL) {
        printf("jack_port_get_buffer failed, cannot send anything.");
        return;
    }

    jack_midi_clear_buffer(port_buffer);//mandatory call at the begining, only for output callback

    /* We may push at most one byte per 0.32ms to stay below 31.25 Kbaud limit. */
    bytes_remaining = nframes_to_ms(nframes) * rate_limit;

    while (jack_ringbuffer_read_space(ringbuffer)) {
        //inspect data on ringbuffer
        read = jack_ringbuffer_peek(ringbuffer, (char *)&ev, sizeof(ev));

        if (read != sizeof(ev)) {
            printf("Short read from the ringbuffer, possible note loss.");
            jack_ringbuffer_read_advance(ringbuffer, read); //read and advance the buffer pointers
            continue;
        }

        bytes_remaining -= ev.len;

        if (rate_limit > 0.0 && bytes_remaining <= 0) {
            printf("Rate limiting in effect.");
            break;
        }

        //compute Message event time
        t = ev.time + nframes - last_frame_time;

        /* If computed time is too much into the future, we'll need
           to send it later. */
        if (t >= (int)nframes)
            break;

        /* If computed time is < 0, we missed a cycle because of xrun. */
        if (t < 0)
            t = 0;

        if (time_offsets_are_zero)
            t = 0;

        jack_ringbuffer_read_advance(ringbuffer, sizeof(ev));//read and advance the buffer pointers

        buffer = jack_midi_event_reserve(port_buffer, t, ev.len);//Clients are to write the actual event data to be written starting at the pointer returned by this function.

        if (buffer == NULL) {
            printf("jack_midi_event_reserve failed, NOTE LOST : %x %x %x\n", ev.data[0], ev.data[1], ev.data[2]);
            break;
        }

        memcpy(buffer, ev.data, ev.len);//write actual event into reserved pointer buffer
    }
}

int MidiControl::processCallback(jack_nframes_t nframes, void *arg)
{
    MidiControl * mc = (MidiControl*) arg;
    /* Check for impossible condition that actually happened to me, caused by some problem between jackd and OSS4. */
    if (nframes <= 0) {
        printf("Process callback called with nframes = 0; bug in JACK?");
        return 0;
    }




    mc->process_midi_input(nframes);
    mc->process_midi_output(nframes);


    return (0);
}

int MidiControl::connect(){
    int err;

    jack_client = jack_client_open(PACKAGE_NAME, JackNoStartServer, NULL);

    if (jack_client == NULL) {
        printf("Is jackd running? Could not connect to JACK.");
        return EX_UNAVAILABLE;
    }

    ringbuffer = jack_ringbuffer_create(RINGBUFFER_SIZE*2);

    if (ringbuffer == NULL) {
        printf("Cannot create JACK ringbuffer.");
        return EX_SOFTWARE;
    }

    jack_ringbuffer_mlock(ringbuffer);

    err = jack_set_process_callback(jack_client, MidiControl::processCallback , this);

    if (err) {
        printf("Could not register JACK process callback.");
        return EX_UNAVAILABLE;
    }

    output_port = jack_port_register(jack_client, OUTPUT_PORT_NAME, JACK_DEFAULT_MIDI_TYPE,
        JackPortIsOutput, 0);

    if (output_port == NULL) {
        printf("Could not register JACK output port.");
        return EX_UNAVAILABLE;
    }

    input_port = jack_port_register(jack_client, INPUT_PORT_NAME, JACK_DEFAULT_MIDI_TYPE,
        JackPortIsInput, 0);

    if (input_port == NULL) {
        printf("Could not register JACK input port.");
        return EX_UNAVAILABLE;
    }

    if (jack_activate(jack_client)) {
        printf("Cannot activate JACK client.");
        return EX_UNAVAILABLE;
    }

//    QString nomePorta =  QString("%1:%2").arg(PACKAGE_NAME).arg(OUTPUT_PORT_NAME);
// //    jack_connect(jack_client, nomePorta.toLatin1().data() , "FastTrack Pro:midi.RX");
//    jack_connect(jack_client, nomePorta.toLatin1().data() , nomePortaDestino.toLatin1().data());

    printf("Running as JACK client.\n");
    return 0;
}

void MidiControl::desconectar(QString nomePortaDestino){
    QString nomePorta =  QString("%1:%2").arg(PACKAGE_NAME).arg(OUTPUT_PORT_NAME);
    jack_disconnect(jack_client, nomePorta.toLatin1().data() , nomePortaDestino.toLatin1().data());
}


void MidiControl::queue_message(struct MidiControl::MidiMessage *ev)
{
    int written;

    if (jack_ringbuffer_write_space(ringbuffer) < sizeof(*ev)) {
        printf("Not enough space in the ringbuffer, NOTE LOST.\n");
        return;

    }

    written = jack_ringbuffer_write(ringbuffer, (char *)ev, sizeof(*ev));

    if (written != sizeof(*ev))
        printf("jack_ringbuffer_write failed, NOTE %i LOST.", ev->data[1] );
}



void MidiControl::queue_new_message(int b0, int b1, int b2)
{
    struct MidiControl::MidiMessage ev;

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
    printf("%X %X %X\n", ev.data[0], ev.data[1], ev.data[2]);
    queue_message(&ev);

}

int MidiControl::calcularChecksum(int endereco, QList<int> *data)
{
    int a,b,c,d, e, f, soma = 0;
    a = b = c = d = e = f = soma = 0; //faltava inicializar as variáveis corretamente para o checksum ser correto
    //endereço
    a = endereco & 0xFF;
    b = (endereco >> 8) & 0xFF;
    c = (endereco >> 16) & 0xFF;
    d = (endereco >> 24) & 0xFF;
    //dados do DS ou size do RQ
    for (int dado :*data) {
        if(dado <= 0xFF){
            e+=dado;
        } else {
            e += dado & 0xFF;
            f += (dado >> 8) & 0xFF;
        }
    }


    soma = 128 - ( (a+b+c+d+e+f)%128 );

    return soma;
}

int MidiControl::calcularChecksum(int endereco, int dado)
{
    int a,b,c,d, e, f, soma = 0;
    a = b = c = d = e = f = soma = 0; //faltava inicializar as variáveis corretamente para o checksum ser correto
    //endereço
    a = endereco & 0xFF;
    b = (endereco >> 8) & 0xFF;
    c = (endereco >> 16) & 0xFF;
    d = (endereco >> 24) & 0xFF;
    //dados
    if(dado <= 0xFF){
        e=dado;
    } else {
        e = dado & 0xFF;
        f = (dado >> 8) & 0xFF;
    }

    soma = 128 - ( (a+b+c+d+e+f)%128 );

    return soma;
}

void MidiControl::txPacoteDataSet(int addr, QList<int> *data){
    int a4 = addr & 0xFF;
    int a3 = addr >> 8 & 0xFF;
    int a2 = addr >> 16 & 0xFF;
    int a1 = addr >> 24 & 0xFF;



    //início da mensagem DS
    queue_new_message(0xF0, 0x41, 0x10 );
    queue_new_message(0x6A, 0x12,  a1);
    queue_new_message(a2, a3, a4 );

    for (int i=0; i<data->length(); i+=3) {
        if(i+2 < data->length()){
            queue_new_message(data->at(i), data->at(i+1), data->at(i+2));
        } else {
            if(i+1 >= data->length()){
                queue_new_message(data->at(i), -1, -1);
            } else {
                queue_new_message(data->at(i), data->at(i+1), -1);
            }

        }
    }

    queue_new_message(calcularChecksum(addr, data), 0xf7, -1);
    data->clear();
}

void MidiControl::txPacoteRequestData(int addr, int data){
    int a4 = addr & 0xFF;
    int a3 = addr >> 8 & 0xFF;
    int a2 = addr >> 16 & 0xFF;
    int a1 = addr >> 24 & 0xFF;

    //início da mensagem DS
    queue_new_message(0xF0, 0x41, 0x10 );
    queue_new_message(0x6A, 0x11,  a1);
    queue_new_message(a2, a3, a4 );
    queue_new_message(0, 0,0 ); //size
    queue_new_message(data, calcularChecksum(addr, data), 0xf7 );

}

void MidiControl::tx(QList<SysExMessage *> *sxs){
    bool segundoEnvio = false;

    for(int i=0; i<sxs->length(); i++){
        if(contadorMensagensEnviadas%8 == 0) {
//            qDebug() << "===> INTERVALO ENTRE PACOTES " << QString::number(contadorMensagensEnviadas);
            contadorMensagensEnviadas = 0;
            usleep(25000);
        }

        SysExMessage *msg = sxs->at(i);

        queue_new_message(msg->message.b0, msg->message.b1, msg->message.b2 );
        queue_new_message(msg->message.b3, msg->message.b4, msg->message.b5 );
        queue_new_message(msg->message.b6, msg->message.b7, msg->message.b8 );
        queue_new_message(msg->message.b9, msg->message.b10, msg->message.b11 );


        queue_new_message(msg->message.b12
                          , msg->message.b13 > -1? msg->message.b13 : -1
                          ,  msg->message.b14 > -1? msg->message.b14 : -1);


        contadorMensagensEnviadas++;
    }
    sxs->clear();

}

void MidiControl::transmitir(){
    //int total = mensagens->length() / 3;
    //int resto = (total%3);
    //int max = total - resto;

    bool segundoEnvio = false;
    for(int i=0; i<mensagens->length(); i=i+3){
        int b0=-1;
        int b1=-1;
        int b2=-1;

        b0 = mensagens->at(i);
        b1 = mensagens->at(i+1);
        b2 = mensagens->at(i+2);


        if(b0 == 0xF7){
            b1 = -1;
            b2 = -1;
            i=i-2;
        }else if(b1 == 0xF7){
            b2 = -1;
            i=i-1;
        }
        printf("==============> foi chamado =====> %x %x %x", b0, b1, b2 );
        queue_new_message(b0, b1, b2);

        /*
         * tentando repetir a última mensagem SysEx (de F0 até F7)
         */
/*
        if(ultimaMensagemIndex<=9){
            ultimaMensagem[ultimaMensagemIndex++] = b0;
            ultimaMensagem[ultimaMensagemIndex++] = b1;
            ultimaMensagem[ultimaMensagemIndex++] = b2;
        } else {
            if(ultimaMensagem[11] != 0xF7)
                ultimaMensagem[12] = 0xF7;//valor esperado de b0

            QList<int> *msg = new QList<int>();
            for(int k=0; k<=12; k++) {
                msg->append(ultimaMensagem[k]);
                ultimaMensagem[k] = -1;
            }
            mensagensTransmitidas->append(*msg);
            ultimaMensagemIndex=0;
        }
*/

        /*
        if(i=mensagens->length()-1 && segundoEnvio == false){
            usleep(22000);
            //i=0;
            segundoEnvio = true;
            qDebug() << "Segundo Envio";
        }
        */

    }




    mensagens->clear();

    //WORKAROUND: agora tenta re-enviar tudo de novo
/*
    for(int i=0; i<mensagensTransmitidas->length(); i++){
        qDebug() << "re-enviando " << i << "/" << mensagensTransmitidas->length();
        QList<int> msg = mensagensTransmitidas->at(i);
        queue_new_message(msg.at(0), msg.at(1), msg.at(2));
        queue_new_message(msg.at(3), msg.at(4), msg.at(5));
        queue_new_message(msg.at(6), msg.at(7), msg.at(8));
        queue_new_message(msg.at(9), msg.at(10), msg.at(11));
        queue_new_message(msg.at(12), -1, -1 );
    }
    mensagensTransmitidas->clear();
*/
}


/*
void MidiControl::setPerformancePart(int parte, QList<int> *dados){
    int ultimo = 0;
    int endereco= 0x01000000 + ((parte-1 << 8) | 0x1000) ; //endereço temporary performance + endereço da parte
    int offset  = 0x00000000;
    int valor = 0;

    //00 00 Receive Switch  = 0 ou 1
    offset++;

    for(int i=0; i<dados->length(); i++){
        bool usarDuasMsg = false;

        if (offset == 0x04) usarDuasMsg=true;

        valor = dados->at(i);
        iniciarMensagem();
        adicionarEndereco(endereco+offset);
        valor = adicionarDados(valor, usarDuasMsg);
        mensagens->append(calcularChecksum(endereco+offset, valor));
        //qDebug() << QString("%1 %2 %3").arg(endereco, 0,16).arg(valor,0,16).arg(calcularChecksum(endereco+offset, valor),0,16);

        encerrarMensagem();
        (offset == 0x04) ? offset=0x06 : offset++;
        //(offset == 0x0E) ? offset=0x11 : offset++;//pula 3 endereços de receive midi change
    }

    transmitir();
}
*/

bool MidiControl::conectarNaPorta(QString nomePortaDestino){
        QString nomePortaOrigem =  QString("%1:%2").arg(PACKAGE_NAME).arg(OUTPUT_PORT_NAME);
        int ret = jack_connect(jack_client, nomePortaOrigem.toLatin1().data() , nomePortaDestino.toLatin1().data());
        return ret == 0 ? true : false;
}

bool MidiControl::conectarQStageMidiInEm(QString origem){
    QString qstageMidiInput =  QString("%1:%2").arg(PACKAGE_NAME).arg(INPUT_PORT_NAME);
    int ret = jack_connect(jack_client, origem.toLatin1().data(), qstageMidiInput.toLatin1().data() );
    return ret == 0 ? true : false;
}

QList<QString> *MidiControl::listarPortas(){

    if(jack_client == NULL){
        if( connect() > 0)
            return NULL;
    }

    QList<QString> * portas = new QList<QString>();
    const char ** ports = jack_get_ports(jack_client, NULL, NULL, 0 );
    for (unsigned int i = 0; ports && ports[i]; ++i) {
        portas->append((ports[i]));
    }
    return portas;
}

void MidiControl::startSerialMidi(FormSerialMidi *smidi){
    qDebug() << "Start Serial MIDI";

    QThread* thread = new QThread;
    WorkerSerialMidi* worker = new WorkerSerialMidi(smidi);
    worker->moveToThread(thread);
    // "this" não é um QObject para implementar errorString(QString)
    //QObject::connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(process()));
    QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
