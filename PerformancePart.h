#ifndef PERFORMANCEPART_H
#define PERFORMANCEPART_H

#include <QMap>

class PerformancePart
{
public:

    enum Function {
        ReceiveSwitch,
        MIDIChannel,
        PatchGroupType,
        PatchGroupID,
        PatchNumber,
        PartLevel,
        PartPan,
        PartCoarseTune,
        PartFineTune,
        OutputAssign,
        MixEFXSendLevel,
        ChorusSendLevel,
        ReverbSendLevel,
        ReceiveProgramChangeSwitch,
        ReceiveVolumeSwitch,
        ReceiveHold1Switch,
        KeyboardRangeLower,
        KeyboardRangeUpper,
        OctaveShift,
        LocalSwitch,
        TransmitSwitch,
        TransmitBankSelectGroup,
        TransmitVolume,
    };

    PerformancePart();
    PerformancePart(Function func, int part);

    int address = 0x0000;

    bool is2ByteData = false;

    QString functionName;


private:
    QMap<Function,QString> functionMap;
};

#endif // PERFORMANCEPART_H
