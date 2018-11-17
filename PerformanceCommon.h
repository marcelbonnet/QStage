#ifndef PERFORMANCECOMMON_H
#define PERFORMANCECOMMON_H

#include <QMap>
#include <QString>

class PerformanceCommon
{
public:
    enum Function {
        Name01,
        Name02,
        Name03,
        Name04,
        Name05,
        Name06,
        Name07,
        Name08,
        Name09,
        Name10,
        Name11,
        Name12,
        EFXSource,
		EFXType,
		EFXParam01,
		EFXParam02,
		EFXParam03,
		EFXParam04,
        EFXParam05,
		EFXParam06,
		EFXParam07,
		EFXParam08,
		EFXParam09,
		EFXParam10,
		EFXParam11,
		EFXParam12,
		EFXOutputAssign,
		EFXMixSendOutputLevel,
		EFXChorusSendLevel,
		EFXReverbSendLevel,
		EFXControlSource1,
		EFXControlDepth1,
		EFXControlSource2,
		EFXControlDepth2,
		ChorusLevel,
		ChorusRate,
		ChorusDepth,
		ChorusPreDelay,
		ChorusFeedback,
		ChorusOutput,
		ReverbType,
		ReverbLevel,
		ReverbTime,
		ReverbHFDamp,
		DelayFeedback,
		PerformanceTempo,
		KeyboardRangeSwitch,
		KeyboardMode,
		ClockSource,

    };

    int address = 0x0000;
    bool is2ByteData = false;

    QString functionName;
    PerformanceCommon();
    PerformanceCommon(Function func);

private:
    QMap<Function,QString> functionMap;
};

#endif // PERFORMANCECOMMON_H
