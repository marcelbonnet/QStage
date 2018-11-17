#ifndef SYSTEMCOMMON_H
#define SYSTEMCOMMON_H

#include <QMap>
#include <QString>

class SystemCommon
{
public:
    enum Function {
        SoundMode,
        PerformanceNumber,
        PatchGroupType,
        PatchGroupId,
        PatchNumber,
        MasterTune,
        ScaleTuneSwitch,
        EFXSwitch,
        ChorusSwitch,
        ReverbSwitch,
        PatchRemain,
        ClockSource,
        TAPControlSource,
        HoldControlSource,
        PeakControlSource,
        VolumeControlSource,
        AftertouchSource,
        SystemControlSource1,
        SystemControlSource2,
        ReceiveProgramChange,
        ReceiveBankSelect,
        ReceiveControlChange,
        ReceiveModulation,
        ReceiveVolume,
        ReceiveHold1,
        ReceivePitchBend,
        ReceiveAftertouch,
        ControlChannel,
        PatchReceiveChannel,
        RhythmEditSource,
        TransmitProgramChange,
        TransmitBankSelect,
        PatchTransmitChannel,
        TransposeSwitch,
        TransposeValue,
        OctaveShift,
        KeyboardVelocity,
        KeyboardSens,
        AftertouchSens,
        Pedal1Assign,
        Pedal1OutputMode,
        Pedal2Assign,
        Pedal2OutputMode,
        C1Assign,
        C1OutputMode,
        C2Assign,
        C2OutputMode,
        HoldPedalOutputMode,

        ArpeggioStyle,
        ArpeggioMotif,
        ArpeggioBeatPattern,
        ArpeggioAccentRate,
        ArpeggioShuffleRate,
        ArpeggioKeyboardVelocity,
        ArpeggioOctaveRange,
        ArpeggioPartNumber,

        SystemTempo,
        C3Assign,
        C3OutputMode,
        C4Assign,
        C4OutputMode
    };

    int address = 0x0000;
    bool is2ByteData = false;
    QString functionName;

    SystemCommon();
    SystemCommon(Function func);

private:
    QMap<Function, QString> functionMap;
};

#endif // SYSTEMCOMMON_H
