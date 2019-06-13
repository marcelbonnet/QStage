#ifndef DATAREQUEST_H
#define DATAREQUEST_H

#include <QString>
#include "DataSysExType.h"

class DataRequest : public DataSysExType
{
public:
    DataRequest();

    //implicity static constants
    //const int DATA_SET1_MAX_SIZE_IN_BYTES = 13;
    //const int MAX_PACKET_SIZE_IN_BYTES = 128;

//    const int MIN_PACKET_INTERVAL_IN_MICRO_SEC = 20000; //a cada 9 SysEx Messages

//    const int b0  = 0xf0;// exclusive status
//    const int b1  = 0x41;// ID number (Roland)
//    const int b2  = 0x10;// device ID
//    const int b3  = 0x6a;// XP-30 model ID
      int b4  = 0x11;// DATA REQUEST

//    int b5  = 0x00;// Address MSB
//    int b6  = 0x00;// Address
//    int b7  = 0x00;// Address
//    int b8  = 0x00;// Address LSB

//    int b9  = 0x00;//Size MSB
//    int b10 = 0x00;//Size
//    int b11 = 0x00;//Size
//    int b12 = 0x00;//Size LSB
//    int b13 = 0x00;//Check Sum

      int b14 = 0xf7;//EOX

//    const QString getDataRepresentation();
};

#endif // DATAREQUEST_H
