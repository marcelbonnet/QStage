#ifndef PATCH_H
#define PATCH_H

#include <QString>
#include <QVariant>
#include <QMetaType>

class Patch : public QVariant
{
public:
    //a implementação usa a categorias de defaults.h
//    const QString categorias[39] = {
//        "---",
//        "PNO","EP","KEY","BELL","MLT","ORG","ACD","HRM",
//        "AGT","EGT","DGT","BS","SBS","STR",
//        "ORC", "HIT", "WND","FLT","BRS","SBR","SAX","HLD","SLD",
//        "TEK","PLS","SFX","SYN","BPD","SPD","VOX", "PLK",
//        "ETH","FRT","PRC","FX","BTS","DRM","CMB"
//    };

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
        PatchTempo,

        PatchLevel,
        PatchPan,
        AnalogFeel,
        BendRangeUp,
        BendRangeDown,
        KeyAssignMode,
        SoloLegato,
        PortamentoSwitch,
        PortamentoMode,
        PortamentoType,
        PortamentoStart,
        PortamentoTime,
        PatchControlSource2,
        PatchControlSource3,
        EFXControlHoldPeak,
        Control_1_HoldPeak,
        Control_2_HoldPeak,
        Control_3_HoldPeak,
        VelocityRangeSwitch,
        OctaveShift,
        StretchTuneDepth,
        VoicePriority,
        StructyreType12,
        Booster12,
        StructyreType34,
        Booster34,
        ClockSource,
        PatchCategory
    };

    Patch(); //default
    Patch(QString nome, QString categoria, QString categoriaPai, int groupType, int groupId, int number);
    Patch(Function func);

    int address = 0x0000;
    int whichTone = 0;

    bool is2ByteData = false;

    QString functionName;
    Function function;



    QString getNome(){
        return nome;
    }

    QString getCategoria(){
        return categoria;
    }

    QString getCategoriaPai(){
        return categoriaPai;
    }

    int getGroupType(){
        return groupType;
    }

    int getGroupId(){
        return groupId;
    }

    int getNumber(){
        return number;
    }





    //private:
    QString nome;
    QString categoria;
    QString categoriaPai;
    int groupType;
    int groupId;
    int number;

private:
    QMap<Function,QString> functionMap;

};

Q_DECLARE_METATYPE(Patch*)

#endif // PATCH_H
