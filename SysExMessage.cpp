#include "SysExMessage.h"
#include <QDebug>

SysExMessage::SysExMessage(BaseAddress baseAddress, SystemCommon sys, int data)
{
    this->base = baseAddress;
    this->sys = sys;
    this->data = data;

    int addr = base.startAddress + sys.address;
    message.b8 = addr & 0xFF;
    message.b7 = addr >> 8 & 0xFF;
    message.b6 = addr >> 16 & 0xFF;
    message.b5 = addr >> 24 & 0xFF;

    if(!sys.is2ByteData){
        message.b9 = data;
    } else {
        int d1 = data & 0xF;
        int d2 = (data >> 4) & 0xF;
        message.b9 = d2;
        message.b10 = d1;
    }

    message.b11 = calcularChecksum(addr, data);

    qDebug() << QString("SYSTEM COMMON %1 -> %2 = %3    DATA: %4").arg(sys.functionName).arg(QString::number(sys.address,16)).arg(QString::number(data,16)).arg(message.getDataRepresentation());

}

SysExMessage::SysExMessage(BaseAddress baseAddress, PerformanceCommon perf, int data)
{
    this->base = baseAddress;
    this->perf = perf;
    this->data = data;

    int addr = base.startAddress + perf.address;
    message.b8 = addr & 0xFF;
    message.b7 = addr >> 8 & 0xFF;
    message.b6 = addr >> 16 & 0xFF;
    message.b5 = addr >> 24 & 0xFF;

    if(!perf.is2ByteData){
        message.b9 = data;
    } else {
        int d1 = data & 0xF;
        int d2 = (data >> 4) & 0xF;
        message.b9 = d2;
        message.b10 = d1;
    }

    message.b11 = calcularChecksum(addr, data);

    qDebug() << QString("Performance COMMON %1 -> %2 = %3    DATA: %4").arg(perf.functionName).arg(QString::number(perf.address,16)).arg(QString::number(data,16)).arg(message.getDataRepresentation());
}

SysExMessage::SysExMessage(BaseAddress baseAddress, PerformancePart part, int data)
{
    this->base = baseAddress;
    this->part = part;
    this->data = data;

    int addr = base.startAddress + part.address;
    message.b8 = addr & 0xFF;
    message.b7 = addr >> 8 & 0xFF;
    message.b6 = addr >> 16 & 0xFF;
    message.b5 = addr >> 24 & 0xFF;

    if(!part.is2ByteData){
        message.b9 = data;
    } else {
        int d1 = data & 0xF;
        int d2 = (data >> 4) & 0xF;
        message.b9 = d2;
        message.b10 = d1;
    }

    message.b11 = calcularChecksum(addr, data);

    qDebug() << QString("Performance Part %1 -> %2 = %3    DATA: %4").arg(sys.functionName).arg(QString::number(sys.address,16)).arg(QString::number(data,16)).arg(message.getDataRepresentation());
}

SysExMessage::SysExMessage(BaseAddress baseAddress, PatchTone *patchTone, int data)
{
    this->base = baseAddress;
    this->patchTone = patchTone;
    this->data = data;

    int addr = base.startAddress + patchTone->address;
    message.b8 = addr & 0xFF;
    message.b7 = addr >> 8 & 0xFF;
    message.b6 = addr >> 16 & 0xFF;
    message.b5 = addr >> 24 & 0xFF;

    if(!patchTone->is2ByteData){
        message.b9 = data;
    } else {
        int d1 = data & 0xF;
        int d2 = (data >> 4) & 0xF;
        message.b9 = d2;
        message.b10 = d1;
    }

    message.b11 = calcularChecksum(addr, data);

    qDebug() << QString("Patch Tone %4 %1 = %2    DATA: %3")
                .arg(QString::number(patchTone->address,16))
                .arg(QString::number(data,16))
                .arg(message.getDataRepresentation())
                .arg(patchTone->functionName);
}


int SysExMessage::calcularChecksum(int endereco, int dado)
{
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

