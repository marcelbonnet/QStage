#ifndef CLIENTE_H
#define CLIENTE_H

extern "C" {
#include <jack/jack.h>
#include <jack/types.h>
#include <jack/ringbuffer.h>
#include <jack/midiport.h>
}

#include <QString>
#include <QMap>

#include "ClientePorta.h"
#include "QStageException.h"

class Cliente
{
public:
    Cliente(QString clientName);

    struct MidiMessage {
        jack_nframes_t	time;
        int		len;	/* Length of MIDI message, in bytes. */
        unsigned char	data[3];
    };

//    jack_port_t *getOutputPort();
//    jack_port_t *getInputPort();
    jack_client_t *getClient();
//    jack_ringbuffer_t *getRingBuffer();

    /**
     * @brief conectar "cliente:portaOrigem" em "clienteDestino:portaDestino"
     */
    void conectar(QString portaOrigem, QString clienteDestino, QString portaDestino) noexcept(false);
    /**
     * @brief inicializar um cliente
     */
//    void inicializarCliente(JackProcessCallback callback) noexcept(false);


    void inicializarCliente(JackProcessCallback callback) noexcept(false);

    /**
     * @brief Adiciona uma instância uma porta ao cliente. ClientePorta especifica a porta em detalhes.
     */
    void addClientePorta(ClientePorta * cp);
    /**
     * @brief Recupera a instância de uma porta do cliente. ClientePorta especifica a porta em detalhes.
     */
    ClientePorta* getClientePorta(QString nomePorta);

    /**
     * @brief Envia MIDI para a saída
     */
    void sendMIDI(QString outputPortName, jack_nframes_t nframes ) noexcept(false);

private:
    QString clientName;
    QString inputPortName;
    QString outputPortName;
    QMap<QString,ClientePorta*> portas;
    jack_client_t	*jackClient = NULL;

};

#endif // CLIENTE_H
