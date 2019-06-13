#ifndef DATASYSEXTYPE_H
#define DATASYSEXTYPE_H

#include <QString>

class DataSysExType
{
public:
    const static int DATASET = 1;
    const static int DATAREQUEST = 2;
    int TYPE = DATASET;

    DataSysExType();

    const int MIN_PACKET_INTERVAL_IN_MICRO_SEC = 20000; //a cada 9 SysEx Messages

    const int b0  = 0xf0;// exclusive status
    const int b1  = 0x41;// ID number (Roland)
    const int b2  = 0x10;// device ID
    const int b3  = 0x6a;// XP-30 model ID
    int b4  = 0x12;// DS=0x12 ; DATA REQUEST=0x11

    int b5  = 0x00;// Address MSB
    int b6  = 0x00;// Address
    int b7  = 0x00;// Address
    int b8  = 0x00;// Address LSB

    int b9  = 0x00;//Size MSB  ou data em DS
    int b10 = 0x00;//Size   ou data em DS
    int b11 = 0x00;//Size  ou check sum em DS
    int b12 = 0x00;//Size LSB ou EOX em DS
    int b13 = 0x00;//Check Sum  ou não existe DS

    const int b14 = 0xf7;//EOX ou não existe em DS



    const QString getDataRepresentation();
};

#endif // DATASYSEXTYPE_H
