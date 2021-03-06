#ifndef WORKERSERIALMIDI_H
#define WORKERSERIALMIDI_H
#include <QObject>
#include <QList>
#include "FormSerialMidi.h"

class WorkerSerialMidi : public QObject
{
    Q_OBJECT
public:
    WorkerSerialMidi(FormSerialMidi *smidi);

public slots:
    void process();

protected:
    /*
     * Serial USB reader.
     * with code from https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c#6947758
     */
    int set_interface_attribs (int fd, int speed, int parity);
    void set_blocking (int fd, int should_block);
    FormSerialMidi *smidi;
    QList<int> *notasLigadas;
    int ultimaNotaTocadaComSustain = -1;

signals:
    void finished();
    void error(QString err);
};

#endif // WORKERSERIALMIDI_H
