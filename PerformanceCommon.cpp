#include "PerformanceCommon.h"
#include <QDebug>

PerformanceCommon::PerformanceCommon()
{

}
PerformanceCommon::PerformanceCommon(Function func)
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
    functionMap.insert(EFXSource,"EFXSource");
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
    functionMap.insert(PerformanceTempo,"PerformanceTempo");
    functionMap.insert(KeyboardRangeSwitch,"KeyboardRangeSwitch");
    functionMap.insert(KeyboardMode,"KeyboardMode");
    functionMap.insert(ClockSource,"ClockSource");

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
    case         EFXSource:
        address = 0x000C;
        break;
    case         EFXType:
        address = 0x000D;
        break;
    case         EFXParam01:
        address = 0x000E;
        break;
    case         EFXParam02:
        address = 0x000F;
        break;
    case         EFXParam03:
        address = 0x0010;
        break;
    case         EFXParam04:
        address = 0x0011;
        break;
    case         EFXParam05:
        address = 0x0012;
        break;
    case         EFXParam06:
        address = 0x0013;
        break;
    case         EFXParam07:
        address = 0x0014;
        break;
    case         EFXParam08:
        address = 0x0015;
        break;
    case         EFXParam09:
        address = 0x0016;
        break;
    case         EFXParam10:
        address = 0x0017;
        break;
    case         EFXParam11:
        address = 0x0018;
        break;
    case         EFXParam12:
        address = 0x0019;
        break;
    case         EFXOutputAssign:
        address = 0x001A;
        break;
    case         EFXMixSendOutputLevel:
        address = 0x001B;
        break;
    case         EFXChorusSendLevel:
        address = 0x001C;
        break;
    case         EFXReverbSendLevel:
        address = 0x001D;
        break;
    case         EFXControlSource1:
        address = 0x001E;
        break;
    case         EFXControlDepth1:
        address = 0x001F;
        break;
    case         EFXControlSource2:
        address = 0x0020;
        break;
    case         EFXControlDepth2:
        address = 0x0021;
        break;
    case         ChorusLevel:
        address = 0x0022;
        break;
    case         ChorusRate:
        address = 0x0023;
        break;
    case         ChorusDepth:
        address = 0x0024;
        break;
    case         ChorusPreDelay:
        address = 0x0025;
        break;
    case         ChorusFeedback:
        address = 0x0026;
        break;
    case         ChorusOutput:
        address = 0x0027;
        break;
    case         ReverbType:
        address = 0x0028;
        break;
    case         ReverbLevel:
        address = 0x0029;
        break;
    case         ReverbTime:
        address = 0x002A;
        break;
    case         ReverbHFDamp:
        address = 0x002B;
        break;
    case         DelayFeedback:
        address = 0x002C;
        break;
    case         PerformanceTempo:
        address = 0x002D;
        is2ByteData = true;
        break;
    case         KeyboardRangeSwitch:
        address = 0x002F;
        break;
    case         KeyboardMode:
        address = 0x0040;
        break;
    case         ClockSource:
        address = 0x0041;
        break;
    }
}
