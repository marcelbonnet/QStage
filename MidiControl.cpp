#include "MidiControl.h"
#include <QString>

#include <QDebug>
#include <stdio.h>
#include <unistd.h>

MidiControl::MidiControl()
{
    //recupera dados
    /*queue_new_message( 0xF0, 0x41, 0x10);
    queue_new_message( 0x6A, 0x11, 0x10);
    queue_new_message( 0x02, 0x12, 0x00);
    queue_new_message( 0x00, 0x00, 0x00);
    queue_new_message( 0x19, 0x43, 0xF7);*/

    //performance common EFX : PERFORM REVERB : type=delay
    /*queue_new_message(0xF0, 0x41, 0x10);
    queue_new_message(0x6a, 0x12, 0x01);
    queue_new_message(0x00, 0x00, 0x28);
    queue_new_message(0x06, 0x51, 0xf7);*/


    //Em Modo GM: o teclado recebe a troca de patch em cada part:
    /*queue_new_message(0xc0, 0x3e, -1);//GM:063
    queue_new_message(0xc1, 0x2a, -1);
    queue_new_message(0xc2, 0x5f, -1);
    queue_new_message(MIDI_NOTE_ON, 0x49, VELOCITY_NORMAL);
    queue_new_message(0x91, 0x52, VELOCITY_NORMAL);
    queue_new_message(0x92, 0x56, VELOCITY_NORMAL);
    sleep(2);	//não recebeu o note off?
    queue_new_message(0x80, 0x49, -1);
    queue_new_message(0x81, 0x52, -1);
    queue_new_message(0x82, 0x56, -1);*/

    //pg 204 Example 2 : o cliente precisa continuar conectado no jackd para o XP-30 transmitir
    /*
    queue_new_message(0xf0, 0x41, 0x10);
    queue_new_message(0x6a, 0x11, 0x10); //0x11 pede para transmitir dados
    queue_new_message(0x02, 0x12, 0x00);
    queue_new_message(0x00, 0x00, 0x00);
    queue_new_message(0x19, 0x43, 0xf7);*/

    /*//troca o caracter posição 4 do perform common / perform name
    queue_new_message(0xf0, 0x41, 0x10);
    queue_new_message(0x6a, 0x12, 0x01);
    queue_new_message(0x00, 0x00, 0x04);
    int data = 0x23;
    int cksum = 128 - (0x01 + 0x04 + data)%128;
    queue_new_message(data, cksum, 0xf7);*/


    /*
    //patch no perform part 1, trocando para XPA
    //set perform patch group type (1-2-2) = 0x02
    queue_new_message(0xf0, 0x41, 0x10);
    queue_new_message(0x6a, 0x12, 0x01);
    queue_new_message(0x00, 0x10, 0x02);
    int cksum = 128 - (0x01 + 0x10 + 0x02 + 0x02)%128;
    queue_new_message(0x02, cksum, 0xf7);
    //set perform patch group id (1-2-2) = 0x09
    queue_new_message(0xf0, 0x41, 0x10);
    queue_new_message(0x6a, 0x12, 0x01);
    queue_new_message(0x00, 0x10, 0x03);
    cksum = 128 - (0x01 + 0x10 + 0x03 + 0x09)%128;
    queue_new_message(0x09, cksum, 0xf7);
    //patch number 112 = 0x6F => a transmitir em duas partes: 0x06 e 0x0F
    queue_new_message(0xf0, 0x41, 0x10);
    queue_new_message(0x6a, 0x12, 0x01);
    queue_new_message(0x00, 0x10, 0x04);
    cksum = 128 - (0x01 + 0x10 + 0x04 + 0x06 + 0x0f )%128;
    queue_new_message(0x06, 0x0f,cksum);
    queue_new_message(0xf7, -1, -1);
    */
    //jack_client_close(jack_client);


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

    assert(event.size >= 1 && event.size <= 3);

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

        //post_process_midi_input(rev);
    }
}

int MidiControl::post_process_midi_input(struct MidiControl::MidiMessage *ev){
    //não usado
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




    //process_midi_input(nframes);
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
        // parece que começa a dar checksum error: talvez o teclado dê timeout na mensagem sysex truncada?
//        qDebug() << "Not enough space in the ringbuffer, NOTE LOST.";
//        //usleep(300000);
//        qDebug() << "Tentando de novo.";
//        queue_message(ev);
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

    queue_message(&ev);


    /*
    if(ev.data[0] == 0xF7 || ev.data[1] == 0xF7 || ev.data[2] == 0xF7){
        //fazer um buffer das mensagens para saber o que re-enviar
        qtdeMensagensExclusivas++;
        qDebug() << QString("#%1").arg(qtdeMensagensExclusivas);
        usleep(100000);

    }
    */
}

int MidiControl::calcularChecksum(int endereco, int dado){
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

void MidiControl::adicionarEndereco(int endereco){
    int a,b,c,d = 0;

    a = endereco & 0xFF;    //LSB
    b = (endereco >> 8) & 0xFF;
    c = (endereco >> 16) & 0xFF;
    d = (endereco >> 24) & 0xFF; //MSB

    mensagens->append(d);
    mensagens->append(c);
    mensagens->append(b);
    mensagens->append(a);
}

int MidiControl::adicionarDados(int dados, bool usarDuasMsg){
    int e, f = 0;

    if(usarDuasMsg) /*nos endereços que usam valores maiores que 127*/ {
        e = dados & 0xF;
        f = (dados >> 4) & 0xF;
        //qDebug() << dados << QString("%1 %2").arg(f,0,16).arg(e,0,16);
        mensagens->append(f);
        mensagens->append(e);

        return f+e;
    } else {
        mensagens->append(dados);
        return dados;
    }

}

void MidiControl::iniciarMensagem(){
    mensagens->append(0xF0);//sysex
    mensagens->append(0x41);//roland
    mensagens->append(0x10);//device id
    mensagens->append(0x6A);//xp-30
    mensagens->append(0x12);//DT1

}

void MidiControl::encerrarMensagem(){
    mensagens->append(0xF7);//EOX
}

void MidiControl::transmitir(){
    //int total = mensagens->length() / 3;
    //int resto = (total%3);
    //int max = total - resto;

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

        queue_new_message(b0, b1, b2);

    }

    //for(int i=0; i<max; i++){
/*
        if(b0 == -1){
            b0 = mensagens->at(i);
            continue;
        }
        if(b0 == 0xF7){
            queue_new_message(b0, b1, b2);
            b0=-1;
            b1=-1;
            b2=-1;

            continue;
        }

        if(b1 == -1){
            b1 = mensagens->at(i);
            continue;
        }
        if(b1 == 0xF7){
            queue_new_message(b0, b1, b2);
            b0=-1;
            b1=-1;
            b2=-1;

            continue;
        }

        if(b2 == -1)
            b2 = mensagens->at(i);

        queue_new_message(b0, b1, b2);
        b0=-1;
        b1=-1;
        b2=-1;
*/

        /*
        if(b0 == 0xF7){
            b1=-1;
            b2=-1;
            queue_new_message(b0, b1, b2);
            i=i-2;
        } else if(b1 == 0xF7){
            b2 = -1;
            queue_new_message(b0, b1, b2);
            i=i-1;
        } else {
            queue_new_message(b0, b1, b2);
        }
        */
        //queue_new_message(mensagens->at(i), mensagens->at(i+1), mensagens->at(i+2));


    //}


/*
    if(resto > 0){
        int b0 = mensagens->at(max);
        int b1 = -1;
        int b2 = -1;

        if(resto == 2){
            b1 = mensagens->at(max+1);
        }
        queue_new_message(b0, b1, b2);
    }
*/
    mensagens->clear();
}

void MidiControl::setSystemCommon(QMap<int, int> dados){
    QMapIterator<int, int> i(dados);
    int endereco= 0x0000; //System Common
    while(i.hasNext()){
        i.next();
        int offset  = i.key();
        int val     = i.value();

        iniciarMensagem();
        adicionarEndereco(endereco + offset);
        int valor = adicionarDados(val);
        mensagens->append(calcularChecksum(endereco+ offset, valor));
        encerrarMensagem();
    }

    transmitir();
}

void MidiControl::setPerformanceCommon(QList<int> *dados){
    int ultimo = 0;
    int endereco= 0x01000000; //endereço temporary performance
    int offset  = 0x00000000;
    int valor = 0;



    //Name
    for(int i=0; i<12 ; i++){
        iniciarMensagem();

        valor = dados->at(i);

        adicionarEndereco(endereco + offset);

        valor = adicionarDados(valor);
        mensagens->append(calcularChecksum(endereco+ offset, valor));

        encerrarMensagem();
        ultimo = i;
        offset++; //varia de 0 a 11 ; 0x00 a 0x0B
    }



    ultimo++;

    //demais parâmetros
    for(int i=ultimo; i < ultimo+35; i++){

//        if(offset == 0x1a || offset == 0x2d || offset == 0x2e){
//            offset++;
//            continue;
//        }
        bool usarDuasMsg = false;
        if (offset == 0x2D) usarDuasMsg=true;

        iniciarMensagem();
        valor = dados->at(i);
        adicionarEndereco(endereco + offset);
        valor = adicionarDados(valor, usarDuasMsg);
        mensagens->append(calcularChecksum(endereco+ offset, valor));
        encerrarMensagem();

        //QString strDebug = QString("Endereco %1 Valor %2\n").arg(endereco+offset,0,16).arg(valor);
        //qDebug() << strDebug;
        (offset == 0x2D) ? offset=0x2F : offset++;
    }

    //Voice Reserve 0030 a 003F
    //TODO


    //keyboard mode
    iniciarMensagem();
    offset = 0x00000040;
    valor = dados->at(47);
    adicionarEndereco(endereco + offset);
    valor = adicionarDados(valor);
    mensagens->append(calcularChecksum(endereco+ offset, valor));
    encerrarMensagem();


    //Clock Source: fixo em PERFORM
    iniciarMensagem();
    offset = 0x00000041;
    valor = 0;
    adicionarEndereco(endereco + offset);
    valor = adicionarDados(valor);
    mensagens->append(calcularChecksum(endereco+ offset, valor));
    encerrarMensagem();


    transmitir();

}

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

bool MidiControl::conectarNaPorta(QString nomePortaDestino){
        QString nomePortaOrigem =  QString("%1:%2").arg(PACKAGE_NAME).arg(OUTPUT_PORT_NAME);
        int ret = jack_connect(jack_client, nomePortaOrigem.toLatin1().data() , nomePortaDestino.toLatin1().data());
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
