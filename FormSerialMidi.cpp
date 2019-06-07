#include "FormSerialMidi.h"
#include "ui_FormSerialMidi.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFrame>
#include <QStringList>
#include <QLabel>
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>
#include <QException>
#include <QRandomGenerator>

#define CLIENTE "QStagePedal"
#define OUT_PORT_NAME "Pedal_Out"

FormSerialMidi::FormSerialMidi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSerialMidi)
{
    ui->setupUi(this);

    QString notas[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    QStringList prg;
    prg << "OFF" << "Nota"
        << "Maior" << "menor"
        << "2sus"
        << "4sus";

    QStringList intervalos;
    intervalos << "IIb" << "II"
               << "IIIb" << "III"
               << "IV" << "IV#" << "V"
               << "VI b" << "VI"
               << "VII b" << "VII" << "VIII"
               << "IX" << "XI";

    cmbProgramas = new QList<QComboBox*>();
    cmbIntervalos = new QList<QListWidget*>();
    cmbVelocityVals = new QList<QSpinBox*>();
    cmbHumanizeVals = new QList<QSpinBox*>();
    cmbOitava = new QList<QSpinBox*>();

    QVBoxLayout *vbox = new QVBoxLayout(parent);
    QScrollArea *scroll = new QScrollArea();
    QGridLayout *grid = new QGridLayout(scroll);
    QFrame *frame = new QFrame();

    btnIgnoreNoteOff = new QPushButton("Sustain");
    btnIgnoreNoteOff->setCheckable(true);
    //connect(btnIgnoreNoteOff, SIGNAL(clicked()), this, SLOT(onSustain()));

    int y = 0;
    grid->addWidget(btnIgnoreNoteOff, y, 0, 1, 2, Qt::AlignLeft);
    y++;

    for(int i=0; i<12; i++){
        QLabel *l = new QLabel(notas[i]);
        grid->addWidget(l, y, 0, 1, 1, Qt::AlignLeft);

        QComboBox *cmbPrograma = new QComboBox();
        for(int o=0; o<prg.size(); o++)
            cmbPrograma->addItem(prg.at(o));
        grid->addWidget(cmbPrograma,y,1,1,1,Qt::AlignTop);

        cmbProgramas->append(cmbPrograma);

        QListWidget *lstAdds = new QListWidget();
        lstAdds->setSelectionMode(QListWidget::MultiSelection);
        lstAdds->setMaximumHeight(100);
        for(int o=0; o<intervalos.size(); o++){
            QListWidgetItem * item = new QListWidgetItem();
            item->setText(intervalos.at(o));
            item->setData(Qt::UserRole, QVariant::fromValue(o));
            lstAdds->addItem(item);
        }
        grid->addWidget(lstAdds,y,2,1,1,Qt::AlignTop);

        cmbIntervalos->append(lstAdds);

        //Oitava
        QSpinBox *oitava = new QSpinBox();
        oitava->setMinimum(-1);
        oitava->setMaximum(9);
        oitava->setValue(4);

        grid->addWidget(new QLabel("Oitava"),y,3,1,1,Qt::AlignTop);
        grid->addWidget(oitava,y,4,1,1,Qt::AlignTop);
        cmbOitava->append(oitava);
        //Velocity
        QSpinBox *velocity = new QSpinBox();
        velocity->setMinimum(0);
        velocity->setMaximum(127);
        velocity->setValue(100);

        grid->addWidget(new QLabel("Velocity"),y,5,1,1,Qt::AlignTop);
        grid->addWidget(velocity,y,6,1,1,Qt::AlignTop);
        cmbVelocityVals->append(velocity);

        //Humanize
        QSpinBox *humanize = new QSpinBox();
        humanize->setMinimum(0);
        humanize->setMaximum(100);
        humanize->setValue(10);

        grid->addWidget(new QLabel("Humanize"),y,7,1,1,Qt::AlignTop);
        grid->addWidget(humanize,y,8,1,1,Qt::AlignTop);
        cmbHumanizeVals->append(humanize);

        y++;
    }

    frame->setLayout(grid);
    scroll->setWidget(frame);
    vbox->addWidget(scroll);
    setLayout(vbox);

}

bool FormSerialMidi::ignorarNoteOff(){
    return btnIgnoreNoteOff->isChecked();
}

QList<int> *FormSerialMidi::getNotas(int notaRaiz){
    QList<int> *notas = new QList<int>();

    int prog = cmbProgramas->at(notaRaiz)->currentIndex();


    QList<QListWidgetItem*> itens = cmbIntervalos->at(notaRaiz)->selectedItems();


    switch (prog) {
    case 0: //OFF
        return NULL;
    case 1://NOTA
        notas->append(0);
        break;
    case 2:// Maior
        notas->append(4);
        notas->append(7);
        break;
    case 3:// menor
        notas->append(3);
        notas->append(7);
        break;
    case 4:// 2sus
        notas->append(2);
        notas->append(7);
        break;
    case 5:// 4sus
        notas->append(5);
        notas->append(7);
        break;
    }

    for(QListWidgetItem *item : itens){
        int index = item->data(Qt::UserRole).value<int>();

        if(index <=11)
            notas->append(index+1);

        if(index == 12)
            notas->append(14); // intervalo de 9

        if(index == 13)
            notas->append(17); // intervalo de 11

    }

    return  notas;
}

void FormSerialMidi::setOitava(int notaRaiz, int val) noexcept(false){
    if(notaRaiz < 0 || notaRaiz > 11)
        throw new QStageException("Nota fora do intervalo de 0 a 11");
    if(val < -1 || val > 9)
        throw new QStageException("Oitava fora do intervalo de -1 a 9");

    cmbOitava->at(notaRaiz)->setValue(val);
}

int FormSerialMidi::getOitava(int notaRaiz) noexcept(false){
    if(notaRaiz < 0 || notaRaiz > 11)
        throw new QStageException("Nota fora do intervalo de 0 a 11");

    return cmbOitava->at(notaRaiz)->value();
}

void FormSerialMidi::setVelocity(int notaRaiz, int velocity) noexcept(false){
    if(notaRaiz < 0 || notaRaiz > 11)
        throw new QStageException("Nota fora do intervalo de 0 a 11");
    if(velocity < 0 || velocity > 127)
        throw new QStageException("Velocity fora do intervalo de 0 a 127");

    cmbVelocityVals->at(notaRaiz)->setValue(velocity);
}

int FormSerialMidi::getVelocity(int notaRaiz) noexcept(false){
    if(notaRaiz < 0 || notaRaiz > 11)
        throw new QStageException("Nota fora do intervalo de 0 a 11");

    QRandomGenerator qr;
    double r = qr.system()->generateDouble();
    int humanize = cmbHumanizeVals->at(notaRaiz)->value() / 2;
    qDebug() << "HUMANIZE: " << r << humanize << (r>0.5);
    int velocity = 127;
    if(r>=0.5)
        velocity = cmbVelocityVals->at(notaRaiz)->value() * ( 1 + (humanize/2 + r/2)/100 );
    else
        velocity = cmbVelocityVals->at(notaRaiz)->value() * ( 1 - (humanize/2 - r/2)/100 );

    return velocity > 127 ? 127 : velocity;
}

void FormSerialMidi::setVelocityHumanize(int notaRaiz, int humanize) noexcept(false){
    if(notaRaiz < 0 || notaRaiz > 11)
        throw new QStageException("Nota fora do intervalo de 0 a 11");
    if(humanize < 0 || humanize > 100)
        throw new QStageException("Humanize fora do intervalo de 0 a 100");

    cmbHumanizeVals->at(notaRaiz)->setValue(humanize);
}

//int FormSerialMidi::getVelocityHumanize(int notaRaiz) noexcept(false){
//    if(notaRaiz < 0 || notaRaiz > 11)
//        throw new QStageException("Nota fora do intervalo de 0 a 11");

//    return cmbHumanizeVals->at(notaRaiz)->value();
//}

void FormSerialMidi::setBtnIgnorarNoteOff(bool checked){
    btnIgnoreNoteOff->setChecked(checked);
}

void FormSerialMidi::setPrograma(int nota, int index){
    cmbProgramas->at(nota)->setCurrentIndex(index);
}

void FormSerialMidi::setIntervalos(int nota, QList<int> *notas){
    for(int n : *notas){
        qDebug() << n;

        if(n <=12)
            cmbIntervalos->at(nota)->setCurrentRow(n-1);

        if(n == 14)
            cmbIntervalos->at(nota)->setCurrentRow(12);

        if(n == 17)
            cmbIntervalos->at(nota)->setCurrentRow(13);

    }
}

FormSerialMidi::~FormSerialMidi()
{
    delete ui;
}

void FormSerialMidi::inicializarCliente() noexcept(false){
    int err;


    jack_client = jack_client_open(CLIENTE, JackNoStartServer, NULL);

    if (jack_client == NULL) {
        throw new QStageException("jack client nulo");
    }

    ringbuffer = jack_ringbuffer_create(RINGBUFFER_SIZE*2);

    if (ringbuffer == NULL) {
        throw new QStageException("ring buffer nulo");
    }

    jack_ringbuffer_mlock(ringbuffer);

    err = jack_set_process_callback(jack_client, FormSerialMidi::processCallback , this);

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

void FormSerialMidi::conectar(QString nomePortaDestino)noexcept(false){
    QString nomePortaOrigem =   QString("%1:%2").arg(CLIENTE).arg(OUT_PORT_NAME);
    int ret = jack_connect(jack_client, nomePortaOrigem.toLatin1().data() , nomePortaDestino.toLatin1().data());
    if(ret != 0)
        throw new QStageException("não conectou o cliente na porta");
}

double FormSerialMidi::nframes_to_ms(jack_nframes_t nframes){
    jack_nframes_t sr;

    sr = jack_get_sample_rate(jack_client);

    assert(sr > 0);

    return ((nframes * 1000.0) / (double)sr);
}

struct FormSerialMidi::MidiMessage * FormSerialMidi::midi_message_from_midi_event(jack_midi_event_t event)noexcept(false){
    FormSerialMidi::MidiMessage * ev = new FormSerialMidi::MidiMessage();

    if (ev == NULL) {
        throw new QStageException("MidiMessage está nula");
    }

    assert(event.size >= 1 && event.size <= 3);

    ev->len = event.size;
    ev->time = event.time;

    memcpy(ev->data, event.buffer, ev->len);

    return (ev);
}
void FormSerialMidi::process_midi_output(jack_nframes_t nframes) noexcept(false){
    int read, t, bytes_remaining;
    unsigned char *buffer;
    void *port_buffer;
    jack_nframes_t last_frame_time;
    struct FormSerialMidi::MidiMessage ev;

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

int FormSerialMidi::processCallback(jack_nframes_t nframes, void *arg)noexcept(false)
{
    FormSerialMidi * obj = (FormSerialMidi*) arg;
    /* Check for impossible condition that actually happened to me, caused by some problem between jackd and OSS4. */
    if (nframes <= 0) {
        printf("Process callback called with nframes = 0; bug in JACK?");
        return 0;
    }

    //process_midi_input(nframes);
    obj->process_midi_output(nframes);
    return (0);
}

void FormSerialMidi::queue_message(int b0, int b1, int b2) noexcept(false)
{
    struct FormSerialMidi::MidiMessage ev;

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
        throw new QStageException("Sem espaço no ring buffer");
    }

    written = jack_ringbuffer_write(ringbuffer, (char *)&ev, sizeof(ev));

    if (written != sizeof(ev))
        throw new QStageException("jack_ringbuffer_write failed, NOTE %i LOST." + ev.data[1] );
}



