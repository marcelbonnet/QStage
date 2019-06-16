#include "WorkerSysExRequest.h"
#include <unistd.h>
#include <QDebug>

WorkerSysExRequest::WorkerSysExRequest(MidiControl *jack, int addr, int data)
{
    this->jack = jack;
    this->addr = addr;
    this->data = data;
}

void WorkerSysExRequest::process() {
    //adiciona marcador para saber qual mensagem e a quantidade de itens que teremos nas posições seguintes da lista
//    jack->sysxin->append(addr);//qual página de memória do teclado (endereço)
//    jack->sysxin->append(data+1);//tamanho da mensagem (quantos itens)
    jack->txPacoteRequestData(addr, data);

//    usleep(1000000);
    emit finished();
}
