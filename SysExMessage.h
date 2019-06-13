#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

#include "DataSet1.h"
#include "baseaddress.h"
#include "SystemCommon.h"
#include "PatchTone.h"
#include "PerformanceCommon.h"
#include "PerformancePart.h"
#include "patch.h"
#include "DataRequest.h"
#include "DataSysExType.h"
class SysExMessage
{
public:

    SysExMessage(BaseAddress baseAddress, SystemCommon sys, int data = 0);
    SysExMessage(BaseAddress baseAddress, PerformanceCommon perf, int data = 0);
    SysExMessage(BaseAddress baseAddress, PerformancePart part, int data = 0);
    SysExMessage(BaseAddress baseAddress, PatchTone *patchTone, int data = 0, int type = DataSysExType::DATASET);
    SysExMessage(BaseAddress baseAddress, Patch *patch, int data = 0);


    DataSysExType message = DataSysExType();

protected:
    BaseAddress base;
    SystemCommon sys;
    PerformanceCommon perf;
    PerformancePart part;
    PatchTone *patchTone;
    Patch *patch;
    int data;



private:
    int calcularChecksum(int endereco, int dado);
};

#endif // SYSEXMESSAGE_H
