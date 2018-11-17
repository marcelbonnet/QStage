#ifndef DATASET1_H
#define DATASET1_H

#include <QString>

class DataSet1
{
public:
    DataSet1();

    //implicity static constants
    //const int DATA_SET1_MAX_SIZE_IN_BYTES = 13;
    //const int MAX_PACKET_SIZE_IN_BYTES = 128;
    const int MIN_PACKET_INTERVAL_IN_MICRO_SEC = 20000; //a cada 9 SysEx Messages

    const int b0  = 0xf0;// exclusive status
    const int b1  = 0x41;// ID number (Roland)
    const int b2  = 0x10;// device ID
    const int b3  = 0x6a;// XP-30 model ID
    const int b4  = 0x12;// data set 1

    int b5  = 0x00;// Address MSB
    int b6  = 0x00;// Address
    int b7  = 0x00;// Address
    int b8  = 0x00;// Address LSB

    int b9  = 0x00;//Data
    int b10 = 0x00;//Data
    int b11 = 0x00;//Check Sum

    const int b12 = 0xf7;//EOX

    const QString getDataRepresentation();

protected:
};

#endif // DATASET1_H
