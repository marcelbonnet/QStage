#include "SystemCommon.h"

SystemCommon::SystemCommon()
{

}

SystemCommon::SystemCommon(Function func)
{

    QMap<Function, QString> functionMap = QMap<Function,QString>();

    functionMap.insert(SoundMode,"SoundMode");
    functionMap.insert(PerformanceNumber,"PerformanceNumber");
    functionMap.insert(PatchGroupType,"PatchGroupType");
    functionMap.insert(PatchGroupId,"PatchGroupId");
    functionMap.insert(PatchNumber,"PatchNumber");
    functionMap.insert(MasterTune,"MasterTune");
    functionMap.insert(ScaleTuneSwitch,"ScaleTuneSwitch");
    functionMap.insert(EFXSwitch,"EFXSwitch");
    functionMap.insert(ChorusSwitch,"ChorusSwitch");
    functionMap.insert(ReverbSwitch,"ReverbSwitch");
    functionMap.insert(PatchRemain,"PatchRemain");
    functionMap.insert(ClockSource,"ClockSource");
    functionMap.insert(TAPControlSource,"TAPControlSource");
    functionMap.insert(HoldControlSource,"HoldControlSource");
    functionMap.insert(PeakControlSource,"PeakControlSource");
    functionMap.insert(VolumeControlSource,"VolumeControlSource");
    functionMap.insert(AftertouchSource,"AftertouchSource");
    functionMap.insert(SystemControlSource1,"SystemControlSource1");
    functionMap.insert(SystemControlSource2,"SystemControlSource2");
    functionMap.insert(ReceiveProgramChange,"ReceiveProgramChange");
    functionMap.insert(ReceiveBankSelect,"ReceiveBankSelect");
    functionMap.insert(ReceiveControlChange,"ReceiveControlChange");
    functionMap.insert(ReceiveModulation,"ReceiveModulation");
    functionMap.insert(ReceiveVolume,"ReceiveVolume");
    functionMap.insert(ReceiveHold1,"ReceiveHold1");
    functionMap.insert(ReceivePitchBend,"ReceivePitchBend");
    functionMap.insert(ReceiveAftertouch,"ReceiveAftertouch");
    functionMap.insert(ControlChannel,"ControlChannel");
    functionMap.insert(PatchReceiveChannel,"PatchReceiveChannel");
    functionMap.insert(RhythmEditSource,"RhythmEditSource");
    functionMap.insert(TransmitProgramChange,"TransmitProgramChange");
    functionMap.insert(TransmitBankSelect,"TransmitBankSelect");
    functionMap.insert(PatchTransmitChannel,"PatchTransmitChannel");
    functionMap.insert(TransposeSwitch,"TransposeSwitch");
    functionMap.insert(TransposeValue,"TransposeValue");
    functionMap.insert(OctaveShift,"OctaveShift");
    functionMap.insert(KeyboardVelocity,"KeyboardVelocity");
    functionMap.insert(KeyboardSens,"KeyboardSens");
    functionMap.insert(AftertouchSens,"AftertouchSens");
    functionMap.insert(Pedal1Assign,"Pedal1Assign");
    functionMap.insert(Pedal1OutputMode,"Pedal1OutputMode");
    functionMap.insert(Pedal2Assign,"Pedal2Assign");
    functionMap.insert(Pedal2OutputMode,"Pedal2OutputMode");
    functionMap.insert(C1Assign,"C1Assign");
    functionMap.insert(C1OutputMode,"C1OutputMode");
    functionMap.insert(C2Assign,"C2Assign");
    functionMap.insert(C2OutputMode,"C2OutputMode");
    functionMap.insert(HoldPedalOutputMode,"HoldPedalOutputMode");
    functionMap.insert(ArpeggioStyle,"ArpeggioStyle");
    functionMap.insert(ArpeggioMotif,"ArpeggioMotif");
    functionMap.insert(ArpeggioBeatPattern,"ArpeggioBeatPattern");
    functionMap.insert(ArpeggioAccentRate,"ArpeggioAccentRate");
    functionMap.insert(ArpeggioShuffleRate,"ArpeggioShuffleRate");
    functionMap.insert(ArpeggioKeyboardVelocity,"ArpeggioKeyboardVelocity");
    functionMap.insert(ArpeggioOctaveRange,"ArpeggioOctaveRange");
    functionMap.insert(ArpeggioPartNumber,"ArpeggioPartNumber");
    functionMap.insert(SystemTempo,"SystemTempo");
    functionMap.insert(C3Assign,"C3Assign");
    functionMap.insert(C3OutputMode,"C3OutputMode");
    functionMap.insert(C4Assign,"C4Assign");
    functionMap.insert(C4OutputMode, "C4OutputMode");

    functionName = functionMap.value(func);

    switch(func) {
    case 		SoundMode:
        address = 0x0000;
        break;
    case         PerformanceNumber:
        address = 0x0001;
        break;
    case         PatchGroupType:
        address = 0x0002;
        break;
    case         PatchGroupId:
        address = 0x0003;
        break;
    case         PatchNumber:
        address = 0x0004;
        is2ByteData = true;
        break;
    case         MasterTune:
        address = 0x0006;
        break;
    case         ScaleTuneSwitch:
        address = 0x0007;
        break;
    case         EFXSwitch:
        address = 0x0008;
        break;
    case         ChorusSwitch:
        address = 0x0009;
        break;
    case         ReverbSwitch:
        address = 0x000A;
        break;
    case         PatchRemain:
        address = 0x000B;
        break;
    case         ClockSource:
        address = 0x000C;
        break;
    case         TAPControlSource:
        address = 0x000D;
        break;
    case         HoldControlSource:
        address = 0x000E;
        break;
    case         PeakControlSource:
        address = 0x000F;
        break;
    case         VolumeControlSource:
        address = 0x0010;
        break;
    case         AftertouchSource:
        address = 0x0011;
        break;
    case         SystemControlSource1:
        address = 0x0012;
        break;
    case         SystemControlSource2:
        address = 0x0013;
        break;
    case         ReceiveProgramChange:
        address = 0x0014;
        break;
    case         ReceiveBankSelect:
        address = 0x0015;
        break;
    case         ReceiveControlChange:
        address = 0x0016;
        break;
    case         ReceiveModulation:
        address = 0x0017;
        break;
    case         ReceiveVolume:
        address = 0x0018;
        break;
    case         ReceiveHold1:
        address = 0x0019;
        break;
    case         ReceivePitchBend:
        address = 0x001A;
        break;
    case         ReceiveAftertouch:
        address = 0x001B;
        break;
    case         ControlChannel:
        address = 0x001C;
        break;
    case         PatchReceiveChannel:
        address = 0x001D;
        break;
    case         RhythmEditSource:
        address = 0x001E;
        break;
    case         TransmitProgramChange:
        address = 0x0028;
        break;
    case         TransmitBankSelect:
        address = 0x0029;
        break;
    case         PatchTransmitChannel:
        address = 0x002A;
        break;
    case         TransposeSwitch:
        address = 0x002B;
        break;
    case         TransposeValue:
        address = 0x002C;
        break;
    case         OctaveShift:
        address = 0x002D;
        break;
    case         KeyboardVelocity:
        address = 0x002E;
        break;
    case         KeyboardSens:
        address = 0x002F;
        break;
    case         AftertouchSens:
        address = 0x0030;
        break;
    case         Pedal1Assign:
        address = 0x0031;
        break;
    case         Pedal1OutputMode:
        address = 0x0032;
        break;
    case         Pedal2Assign:
        address = 0x0034;
        break;
    case         Pedal2OutputMode:
        address = 0x0035;
        break;
    case         C1Assign:
        address = 0x0037;
        break;
    case         C1OutputMode:
        address = 0x0038;
        break;
    case         C2Assign:
        address = 0x0039;
        break;
    case         C2OutputMode:
        address = 0x003A;
        break;
    case         HoldPedalOutputMode:
        address = 0x003B;
        break;
    case         ArpeggioStyle:
        address = 0x0058;
        break;
    case         ArpeggioMotif:
        address = 0x0059;
        break;
    case         ArpeggioBeatPattern:
        address = 0x005A;
        break;
    case         ArpeggioAccentRate:
        address = 0x005B;
        break;
    case         ArpeggioShuffleRate:
        address = 0x005C;
        break;
    case         ArpeggioKeyboardVelocity:
        address = 0x005D;
        break;
    case         ArpeggioOctaveRange:
        address = 0x005E;
        break;
    case         ArpeggioPartNumber:
        address = 0x005F;
        break;
    case         SystemTempo:
        address = 0x0060;
        is2ByteData = true;
        break;
    case         C3Assign:
        address = 0x0062;
        break;
    case         C3OutputMode:
        address = 0x0063;
        break;
    case         C4Assign:
        address = 0x0064;
        break;
    case         C4OutputMode:
        address = 0x0065;
        break;
    }


}
