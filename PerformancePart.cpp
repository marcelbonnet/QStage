#include "PerformancePart.h"
#include <QDebug>

PerformancePart::PerformancePart()
{

}

PerformancePart::PerformancePart(Function func, int part)
{
    functionMap = QMap<Function,QString>();

    functionMap.insert(ReceiveSwitch,"ReceiveSwitch");
    functionMap.insert(MIDIChannel,"MIDIChannel");
    functionMap.insert(PatchGroupType,"PatchGroupType");
    functionMap.insert(PatchGroupID,"PatchGroupID");
    functionMap.insert(PatchNumber,"PatchNumber");
    functionMap.insert(PartLevel,"PartLevel");
    functionMap.insert(PartPan,"PartPan");
    functionMap.insert(PartCoarseTune,"PartCoarseTune");
    functionMap.insert(PartFineTune,"PartFineTune");
    functionMap.insert(OutputAssign,"OutputAssign");
    functionMap.insert(MixEFXSendLevel,"MixEFXSendLevel");
    functionMap.insert(ChorusSendLevel,"ChorusSendLevel");
    functionMap.insert(ReverbSendLevel,"ReverbSendLevel");
    functionMap.insert(ReceiveProgramChangeSwitch,"ReceiveProgramChangeSwitch");
    functionMap.insert(ReceiveVolumeSwitch,"ReceiveVolumeSwitch");
    functionMap.insert(ReceiveHold1Switch,"ReceiveHold1Switch");
    functionMap.insert(KeyboardRangeLower,"KeyboardRangeLower");
    functionMap.insert(KeyboardRangeUpper,"KeyboardRangeUpper");
    functionMap.insert(OctaveShift,"OctaveShift");
    functionMap.insert(LocalSwitch,"LocalSwitch");
    functionMap.insert(TransmitSwitch,"TransmitSwitch");
    functionMap.insert(TransmitBankSelectGroup,"TransmitBankSelectGroup");
    functionMap.insert(TransmitVolume,"TransmitVolume");


    functionName = functionMap.value(func);


    switch(func){
    case    ReceiveSwitch:
        address = 0x00;
        break;
    case    MIDIChannel:
        address = 0x01;
        break;
    case    PatchGroupType:
        address = 0x02;
        break;
    case    PatchGroupID:
        address = 0x03;
        break;
    case    PatchNumber:
        address = 0x04;
        is2ByteData = true;
        break;
    case    PartLevel:
        address = 0x06;
        break;
    case    PartPan:
        address = 0x07;
        break;
    case    PartCoarseTune:
        address = 0x08;
        break;
    case    PartFineTune:
        address = 0x09;
        break;
    case    OutputAssign:
        address = 0x0A;
        break;
    case    MixEFXSendLevel:
        address = 0x0B;
        break;
    case    ChorusSendLevel:
        address = 0x0C;
        break;
    case    ReverbSendLevel:
        address = 0x0D;
        break;
    case    ReceiveProgramChangeSwitch:
        address = 0x0E;
        break;
    case    ReceiveVolumeSwitch:
        address = 0x0F;
        break;
    case    ReceiveHold1Switch:
        address = 0x10;
        break;
    case    KeyboardRangeLower:
        address = 0x11;
        break;
    case    KeyboardRangeUpper:
        address = 0x12;
        break;
    case    OctaveShift:
        address = 0x13;
        break;
    case    LocalSwitch:
        address = 0x14;
        break;
    case    TransmitSwitch:
        address = 0x15;
        break;
    case    TransmitBankSelectGroup:
        address = 0x16;
        break;
    case    TransmitVolume:
        address = 0x17;
        is2ByteData = true;
        break;
    }

    address = address + ((part-1 << 8) | 0x1000) ; //endereço temporary performance + endereço da parte
//    qDebug() << QString("Performance Part %1 -> %2 %3").arg(part).arg(functionName).arg(QString::number(address,16)) ;

}
