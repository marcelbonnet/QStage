#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

#include "DataSet1.h"
#include "baseaddress.h"
#include "SystemCommon.h"
#include "PerformanceCommon.h"
#include "PerformancePart.h"

class SysExMessage
{
public:

    SysExMessage(BaseAddress baseAddress, SystemCommon sys, int data = 0);
    SysExMessage(BaseAddress baseAddress, PerformanceCommon perf, int data = 0);
    SysExMessage(BaseAddress baseAddress, PerformancePart part, int data = 0);

    DataSet1 message = DataSet1();
protected:
    BaseAddress base;
    SystemCommon sys;
    PerformanceCommon perf;
    PerformancePart part;
    int data;



private:
    int calcularChecksum(int endereco, int dado);
};

#endif // SYSEXMESSAGE_H