#include "ClientePorta.h"


ClientePorta::ClientePorta(QString nomePorta, int ringBufferSize
                           , ClientePorta::TipoPorta tipoPorta, DirecaoPorta direcao) noexcept(false)
{
    this->nomePorta = nomePorta;
    this->ringBufferSize = ringBufferSize;
    this->tipoPorta = tipoPorta;
    this->direcaoPorta = direcao;

}

jack_ringbuffer_t *ClientePorta::getRingBuffer(){
    return ringBuffer;
}

void ClientePorta::setRingBuffer(jack_ringbuffer_t *ringBuffer){
    this->ringBuffer = ringBuffer;
}


ClientePorta::TipoPorta ClientePorta::getTipoPorta(){
    return tipoPorta;
}

jack_port_t *ClientePorta::getPorta(){
    return this->porta;
}

void ClientePorta::setPorta(jack_port_t *porta){
    this->porta = porta;
}

QString ClientePorta::getNomePorta(){
    return  this->nomePorta;
}


ClientePorta::DirecaoPorta ClientePorta::getDirecaoPorta(){
    return direcaoPorta;
}

int ClientePorta::getRingBufferSize(){
    return  this->ringBufferSize;
}
