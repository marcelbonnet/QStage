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
    jack->txPacoteRequestData(addr, data);
    usleep(1000000);
    emit finished();
}
