#ifndef CLIENTEPORTA_H
#define CLIENTEPORTA_H

extern "C" {
#include <jack/jack.h>
#include <jack/types.h>
#include <jack/ringbuffer.h>
#include <jack/midiport.h>
}

#include <QString>

#include "QStageException.h"

class ClientePorta
{
public:
    enum TipoPorta { AUDIO, MIDI };
    enum DirecaoPorta { INPUT, OUTPUT };

    ClientePorta(QString nomePorta, int ringBufferSize, TipoPorta tipoPorta, DirecaoPorta direcao) noexcept(false);

    jack_ringbuffer_t *getRingBuffer();
    void setRingBuffer(jack_ringbuffer_t *ringBuffer);
    TipoPorta getTipoPorta();
    DirecaoPorta getDirecaoPorta();
    jack_port_t *getPorta();
    void setPorta(jack_port_t *porta);
    QString getNomePorta();
    int getRingBufferSize();

private:
    QString nomePorta;
    TipoPorta tipoPorta;
    DirecaoPorta direcaoPorta;
    jack_port_t *porta;
    jack_ringbuffer_t *ringBuffer;
    int ringBufferSize;

};

#endif // CLIENTEPORTA_H
