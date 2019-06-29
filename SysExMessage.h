#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

#include "DataSysExType.h"
#include "baseaddress.h"
#include "SystemCommon.h"


class SysExMessage
{
public:

    SysExMessage(BaseAddress baseAddress, SystemCommon sys, int data = 0);

    DataSysExType message = DataSysExType();

protected:
    BaseAddress base;
    SystemCommon sys;

    int data;



private:
    int calcularChecksum(int endereco, int dado);
};

#endif // SYSEXMESSAGE_H
