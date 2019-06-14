#ifndef WORKERSYSEXREQUEST_H
#define WORKERSYSEXREQUEST_H
#include <QObject>
#include "MidiControl.h"

class WorkerSysExRequest : public QObject
{
    Q_OBJECT
public:
    WorkerSysExRequest(MidiControl *jack, int addr, int data);

public slots:
    void process();

signals:
    void finished();
//    void error(QString err);

protected:
    MidiControl *jack;
    int addr;
    int data;
};

#endif // WORKERSYSEXREQUEST_H
