#include "patch.h"
#include <QDebug>

Patch::Patch(){

}

Patch::Patch(Function func)
{

    functionMap = QMap<Function,QString>();

    functionMap.insert(Name01,"Name01");
    functionMap.insert(Name02,"Name02");
    functionMap.insert(Name03,"Name03");
    functionMap.insert(Name04,"Name04");
    functionMap.insert(Name05,"Name05");
    functionMap.insert(Name06,"Name06");
    functionMap.insert(Name07,"Name07");
    functionMap.insert(Name08,"Name08");
    functionMap.insert(Name09,"Name09");
    functionMap.insert(Name10,"Name10");
    functionMap.insert(Name11,"Name11");
    functionMap.insert(Name12,"Name12");
    functionMap.insert(EFXType,"EFXType");
    functionMap.insert(EFXParam01,"EFXParam01");
    functionMap.insert(EFXParam02,"EFXParam02");
    functionMap.insert(EFXParam03,"EFXParam03");
    functionMap.insert(EFXParam04,"EFXParam04");
    functionMap.insert(EFXParam05,"EFXParam05");
    functionMap.insert(EFXParam06,"EFXParam06");
    functionMap.insert(EFXParam07,"EFXParam07");
    functionMap.insert(EFXParam08,"EFXParam08");
    functionMap.insert(EFXParam09,"EFXParam09");
    functionMap.insert(EFXParam10,"EFXParam10");
    functionMap.insert(EFXParam11,"EFXParam11");
    functionMap.insert(EFXParam12,"EFXParam12");
    functionMap.insert(EFXOutputAssign,"EFXOutputAssign");
    functionMap.insert(EFXMixSendOutputLevel,"EFXMixSendOutputLevel");
    functionMap.insert(EFXChorusSendLevel,"EFXChorusSendLevel");
    functionMap.insert(EFXReverbSendLevel,"EFXReverbSendLevel");
    functionMap.insert(EFXControlSource1,"EFXControlSource1");
    functionMap.insert(EFXControlDepth1,"EFXControlDepth1");
    functionMap.insert(EFXControlSource2,"EFXControlSource2");
    functionMap.insert(EFXControlDepth2,"EFXControlDepth2");
    functionMap.insert(ChorusLevel,"ChorusLevel");
    functionMap.insert(ChorusRate,"ChorusRate");
    functionMap.insert(ChorusDepth,"ChorusDepth");
    functionMap.insert(ChorusPreDelay,"ChorusPreDelay");
    functionMap.insert(ChorusFeedback,"ChorusFeedback");
    functionMap.insert(ChorusOutput,"ChorusOutput");
    functionMap.insert(ReverbType,"ReverbType");
    functionMap.insert(ReverbLevel,"ReverbLevel");
    functionMap.insert(ReverbTime,"ReverbTime");
    functionMap.insert(ReverbHFDamp,"ReverbHFDamp");
    functionMap.insert(DelayFeedback,"DelayFeedback");
    functionMap.insert(PatchTempo,"PatchTempo");

    functionMap.insert(PatchLevel,"PatchLevel");
    functionMap.insert(PatchPan,"PatchPan");
    functionMap.insert(AnalogFeel,"AnalogFeel");
    functionMap.insert(BendRangeUp,"BendRangeUp");
    functionMap.insert(BendRangeDown,"BendRangeDown");
    functionMap.insert(KeyAssignMode,"KeyAssignMode");
    functionMap.insert(SoloLegato,"SoloLegato");
    functionMap.insert(PortamentoSwitch,"PortamentoSwitch");
    functionMap.insert(PortamentoMode,"PortamentoMode");
    functionMap.insert(PortamentoType,"PortamentoType");
    functionMap.insert(PortamentoStart,"PortamentoStart");
    functionMap.insert(PortamentoTime,"PortamentoTime");
    functionMap.insert(PatchControlSource2,"PatchControlSource2");
    functionMap.insert(PatchControlSource3,"PatchControlSource3");
    functionMap.insert(EFXControlHoldPeak,"EFXControlHoldPeak");
    functionMap.insert(Control_1_HoldPeak,"Control_1_HoldPeak");
    functionMap.insert(Control_2_HoldPeak,"Control_2_HoldPeak");
    functionMap.insert(Control_3_HoldPeak,"Control_3_HoldPeak");
    functionMap.insert(VelocityRangeSwitch,"VelocityRangeSwitch");
    functionMap.insert(OctaveShift,"OctaveShift");
    functionMap.insert(StretchTuneDepth,"StretchTuneDepth");
    functionMap.insert(VoicePriority,"VoicePriority");
    functionMap.insert(StructyreType12,"StructyreType12");
    functionMap.insert(Booster12,"Booster12");
    functionMap.insert(StructyreType34,"StructyreType34");
    functionMap.insert(Booster34,"Booster34");
    functionMap.insert(ClockSource,"ClockSource");
    functionMap.insert(PatchCategory,"PatchCategory");

    functionName = functionMap.value(func);

    switch(func){
    case 		Name01:
        address = 0x0000;
        break;
    case 		Name02:
        address = 0x0001;
        break;
    case 		Name03:
        address = 0x0002;
        break;
    case 		Name04:
        address = 0x0003;
        break;
    case 		Name05:
        address = 0x0004;
        break;
    case 		Name06:
        address = 0x0005;
        break;
    case 		Name07:
        address = 0x0006;
        break;
    case 		Name08:
        address = 0x0007;
        break;
    case 		Name09:
        address = 0x0008;
        break;
    case 		Name10:
        address = 0x0009;
        break;
    case 		Name11:
        address = 0x000A;
        break;
    case 		Name12:
        address = 0x000B;
        break;
    case         EFXType:
        address = 0x000C;
        break;
    case         EFXParam01:
        address = 0x000D;
        break;
    case         EFXParam02:
        address = 0x000E;
        break;
    case         EFXParam03:
        address = 0x000F;
        break;
    case         EFXParam04:
        address = 0x0010;
        break;
    case         EFXParam05:
        address = 0x0011;
        break;
    case         EFXParam06:
        address = 0x0012;
        break;
    case         EFXParam07:
        address = 0x0013;
        break;
    case         EFXParam08:
        address = 0x0014;
        break;
    case         EFXParam09:
        address = 0x0015;
        break;
    case         EFXParam10:
        address = 0x0016;
        break;
    case         EFXParam11:
        address = 0x0017;
        break;
    case         EFXParam12:
        address = 0x0018;
        break;
    case         EFXOutputAssign:
        address = 0x0019;
        break;
    case         EFXMixSendOutputLevel:
        address = 0x001A;
        break;
    case         EFXChorusSendLevel:
        address = 0x001B;
        break;
    case         EFXReverbSendLevel:
        address = 0x001C;
        break;
    case         EFXControlSource1:
        address = 0x001D;
        break;
    case         EFXControlDepth1:
        address = 0x001E;
        break;
    case         EFXControlSource2:
        address = 0x001F;
        break;
    case         EFXControlDepth2:
        address = 0x0020;
        break;
    case         ChorusLevel:
        address = 0x0021;
        break;
    case         ChorusRate:
        address = 0x0022;
        break;
    case         ChorusDepth:
        address = 0x0023;
        break;
    case         ChorusPreDelay:
        address = 0x0024;
        break;
    case         ChorusFeedback:
        address = 0x0025;
        break;
    case         ChorusOutput:
        address = 0x0026;
        break;
    case         ReverbType:
        address = 0x0027;
        break;
    case         ReverbLevel:
        address = 0x0028;
        break;
    case         ReverbTime:
        address = 0x0029;
        break;
    case         ReverbHFDamp:
        address = 0x002A;
        break;
    case         DelayFeedback:
        address = 0x002B;
        break;
    case         PatchTempo:
        address = 0x002C;
        is2ByteData = true;
        break;


    case PatchLevel:
        address = 0x002E;
        break;
    case PatchPan:
        address = 0x002F;
        break;
    case AnalogFeel:
        address = 0x0030;
        break;
    case BendRangeUp:
        address = 0x0031;
        break;
    case BendRangeDown:
        address = 0x0032;
        break;
    case KeyAssignMode:
        address = 0x0033;
        break;
    case SoloLegato:
        address = 0x0034;
        break;
    case PortamentoSwitch:
        address = 0x0035;
        break;
    case PortamentoMode:
        address = 0x0036;
        break;
    case PortamentoType:
        address = 0x0037;
        break;
    case PortamentoStart:
        address = 0x0038;
        break;
    case PortamentoTime:
        address = 0x0039;
        break;
    case PatchControlSource2:
        address = 0x003A;
        break;
    case PatchControlSource3:
        address = 0x003B;
        break;
    case EFXControlHoldPeak:
        address = 0x003C;
        break;
    case Control_1_HoldPeak:
        address = 0x003D;
        break;
    case Control_2_HoldPeak:
        address = 0x003E;
        break;
    case Control_3_HoldPeak:
        address = 0x003F;
        break;
    case VelocityRangeSwitch:
        address = 0x0040;
        break;
    case OctaveShift:
        address = 0x0041;
        break;
    case StretchTuneDepth:
        address = 0x0042;
        break;
    case VoicePriority:
        address = 0x0043;
        break;
    case StructyreType12:
        address = 0x0044;
        break;
    case Booster12:
        address = 0x0045;
        break;
    case StructyreType34:
        address = 0x0046;
        break;
    case Booster34:
        address = 0x0047;
        break;
    case ClockSource:
        address = 0x0048;
        break;
    case PatchCategory:
        address = 0x0049;
        break;
    }

//    qDebug() << QString("PATCH COMMON %1 -> %2").arg(functionName).arg(QString::number(address,16));
}

Patch::Patch(QString nome,QString categoria, QString categoriaPai, int groupType, int groupId, int number)
{
    this->nome = nome;
    this->categoria = categoria;
    this->categoriaPai = categoriaPai;
    this->groupType = groupType;
    this->groupId = groupId;
    this->number = number;
}
