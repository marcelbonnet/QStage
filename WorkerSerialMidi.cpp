#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#include "WorkerSerialMidi.h"
#include "QStageException.h"
#include <QDebug>

#define NOTE_ON 0x90
#define NOTE_OFF 0x80

WorkerSerialMidi::WorkerSerialMidi(FormSerialMidi *smidi)
{
    this->smidi = smidi;
    notasLigadas = new QList<int>();
}


int WorkerSerialMidi::set_interface_attribs (int fd, int speed, int parity)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
        qDebug() << "error %d from tcgetattr" << errno;
        return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
//    tty.c_cflag &= ~ICANON; //não deu certo? era para o arduino não precisar enviar \n
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;


    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        qDebug() << "error %d from tcsetattr" << errno;
        return -1;
    }
    return 0;
}

void WorkerSerialMidi::set_blocking (int fd, int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
        qDebug() << "error %d from tggetattr" << errno;
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
        qDebug() << "error %d setting term attributes" << errno;
}


void WorkerSerialMidi::process(){
    char *portname = QString("/dev/cuaU0").toUtf8().data();

    //    int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
    int fd = open (portname, O_RDONLY | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        qDebug() << "error %d opening %s: %s" << errno << portname << strerror (errno);
        emit finished();
    }

    set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    //se fizer sem block acioanará o setup do arduino a cada loop
    set_blocking (fd, 1);                // set no blocking

    //write (fd, "hello!\n", 7);           // send 7 character greeting

    //usleep ((7 + 25) * 100);             // sleep enough to transmit the 7 plus
    // receive 25:  approx 100 uS per char transmit

    char buf [12];
//    char buf_cmd [4];
//    char buf_dt1 [4];
//    char buf_dt2 [5];
    //para ler binário precisaria abrir com fopen, depois ler com fread. Mas o termios usa fd e não FILE* nas funções
    for (;;) {
//        int n = read (fd, buf, sizeof buf);  // read up to 100 characters if ready to read
//        printf(buf);



        int n = read (fd, buf, sizeof (buf));
        QString buffer = buf;
        //qDebug() << "A PORRA DO BUFFER: " << buf;

//        for(int i=0; i<=2; i++)
//            buf_cmd[i] = buf[i];

//        for(int i=3; i<=4; i++)
//            buf_dt1[i-3] = buf[i];

//        for(int i=5; i<=7; i++)
//            buf_dt2[i-5] = buf[i];

//        memcpy(buf_cmd, &buf[0], 3);
//        buf_cmd[3] = '\0';
//        memcpy(buf_dt1, &buf[0], 3);
//        buf_dt1[3] = '\0';
//        memcpy(buf_cmd, &buf[0], 3);
//        buf_cmd[3] = '\0';

        int cmd =   (QStringRef(&buffer,0,3)).toInt();
        int data1 = (QStringRef(&buffer,3,2)).toInt();
        int data2 = (QStringRef(&buffer,5,3)).toInt();

        qDebug() << "CMD " << cmd;
//        n = pread (fd, buf_dt1, 2, 3);
        qDebug() << "NOTA "  << data1;
//        n = pread (fd, buf_dt2, 3, 2);
//        qDebug() << "VEL "  << data2 ;//<< "TESTE: " << buf_dt2[0] << buf_dt2[1] << buf_dt2[2] ;


        int qtdeNotasParaEnviar = 0;
        try{
            if( (cmd == NOTE_ON || cmd == NOTE_OFF) && (data1 >=48 && data1 <=59)){ //ignorar outros até corrigir o debounce que gera mensagens indesejadas/erradas
                //arduino me envia notas de 48 até 59. Mas no QStage uso de 0 a 11.

                QList<int> *notasParaTocar = smidi->getNotas(data1 - 48); //recebe NULL se a nota estiver marcada como OFF

                if(smidi->ignorarNoteOff() && cmd == NOTE_ON){
                    qDebug() << "EXECUTOU NOTE_OFF DAS NOTAS LIGADAS";
                    //desliga as notas anteriores
                    for(int d : *notasLigadas)
                        smidi->queue_message(NOTE_OFF, d, 0);
                    notasLigadas->clear();

                    //interpretar a repetição da mesma nota como NOTE OFF. Já desligou, vamos emborar para que não ligue no próximo statement
                    if(data1 == ultimaNotaTocadaComSustain){
                        ultimaNotaTocadaComSustain = -1;
                        goto final;
                    }
                }


                if(notasParaTocar != NULL){
                    qtdeNotasParaEnviar = notasParaTocar->count();
                    data2 = smidi->getVelocity(data1-48);
                    qDebug() << "#NOTAS PARA TOCAR: " << qtdeNotasParaEnviar << "VELOCITY " << data2;

                    for(int nota : *notasParaTocar){
                        nota = nota + data1-48 + smidi->getOitava(data1-48)*12;
                        if(smidi->ignorarNoteOff() && cmd == NOTE_ON){
                            notasLigadas->append(nota); //guarda as notas que deverão ficar em sustain até receber outras notas
                            smidi->queue_message(NOTE_ON, nota, data2);
                            ultimaNotaTocadaComSustain = data1;
                        }

                        if(!smidi->ignorarNoteOff() )
                            smidi->queue_message(cmd, nota, data2);
                    }
                }


            }
        } catch(QStageException *e){
            qDebug() << e->getMessage();
        }

//        usleep (( /*recebe char[12] do arduino*/ 12 + /*o que envia*/ qtdeNotasParaEnviar*3  ) );
//        usleep(10000);
        final:
        usleep(1000);

    }

    emit finished();
}
