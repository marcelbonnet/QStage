#ifndef BASEADDRESS_H
#define BASEADDRESS_H

#include <QMap>
#include <QString>

class BaseAddress
{
public:
    enum Edit {
        System,
        TempPerformance,
        PerformanceModeTempPatch,
        TempRhythmSetup,
        PatchModeTempPatch,
        UserPerformance,
        UserRhythmSetup,
        UserPatch
    };

    int startAddress = 0x00000000;
    QString functionName;
    BaseAddress();
    BaseAddress(Edit ed, int param = 0);

private:
    QMap<Edit,QString> functionMap;
};

#endif // BASEADDRESS_H
