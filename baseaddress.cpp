#include "baseaddress.h"
#include <QDebug>

BaseAddress::BaseAddress()
{

}

BaseAddress::BaseAddress(Edit ed, int param)
{
    functionMap = QMap<Edit,QString>();
    functionMap.insert(    System,"System");
    functionMap.insert(    TempPerformance,"TempPerformance");
    functionMap.insert(    PerformanceModeTempPatch,"PerformanceModeTempPatch");
    functionMap.insert(    TempRhythmSetup,"TempRhythmSetup");
    functionMap.insert(    PatchModeTempPatch,"PatchModeTempPatch");
    functionMap.insert(    UserPerformance,"UserPerformance");
    functionMap.insert(    UserRhythmSetup,"UserRhythmSetup");
    functionMap.insert(    UserPatch,"UserPatch");

    functionName = functionMap.value(ed);

    switch (ed) {
    case System:
        break; //usar o valor padrão
    case TempPerformance:
        startAddress = 0x01000000;
        //qDebug() << "Selecionado TempPerformance startAddress=" << QString::number(startAddress, 16);
        break;
    case PerformanceModeTempPatch:
        //se 10 (0xA) usar temporary rhythm setup
        int offset = (param - 1) << 24;
        startAddress = 0x02000000 + offset;
        /*
        if(param >=1 && param <= 9){
            startAddress = 0x02000000 + offset;
        } else{
            int offset = (param - 1) << 24;
            startAddress = 0x02000000 + offset;
        }
        */
        break;
    }
}
