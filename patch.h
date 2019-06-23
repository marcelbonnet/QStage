#ifndef PATCH_H
#define PATCH_H

#include <QString>
#include <QVariant>
#include <QMetaType>

class Patch : public QVariant
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
    Patch(int id, QString nome, int groupType, int groupId, int number, bool roland, int categId);
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

    QString getPresetGroup(){
        if(groupType == 0 && groupId == 1)
            return "USER";
        if(groupType == 0 && groupId == 3)
            return "PR-A";
        if(groupType == 0 && groupId == 4)
            return "PR-B";
        if(groupType == 0 && groupId == 5)
            return "PR-C";
        if(groupType == 0 && groupId == 6)
            return "GM";
        if(groupType == 0 && groupId == 7)
            return "PR-E";
        if(groupType == 2 && groupId == 9)
            return "XP-A";
        if(groupType == 2 && groupId == 2)
            return "XP-B";
        if(groupType == 2 && groupId == 11)
            return "XP-C";

        return NULL;
    }

    QString getFullName(){
        QString nome;
        if(getPresetGroup() != NULL){
            nome+=getPresetGroup()+":";
            nome+=QString("%1 %2 ").arg(getNumber()+1, 3, 10, QChar('0')).arg(_categ[categId]);

        } else {
            nome+=QString("%1 %2 ").arg(this->id, 4, 10, QChar('0')).arg(_categ[categId]);
        }
        nome+=getNome();
        return nome;
    }


    bool isRoland(){ return roland; }


    //private:
    QString nome;//nome do patch
    QString categoria;//categoria por extenso
    QString categoriaPai;//categoria pai que existe apenas nos botões de 1 a 0 do Roland: categ pai "PIANO" contém EL e AC PNO , e assim por diante
    int groupType;//waveform grouptype
    int groupId; //waveform group id
    int number; //waveform number
    bool roland;//true se o patch era de fábrica do Roland ou do banco de USER no momento do backup para SQLite
    int categId;//0 - 38
    int id;// id no SQLite

private:
    QMap<Function,QString> functionMap;

    //tem algum parte do programa QStage que inclui defaults.h, que contém esse arrei com outro nome.
    const QString _categ[39] = {
        "---",
        "PNO","EP","KEY","BELL","MLT","ORG","ACD","HRM",
        "AGT","EGT","DGT","BS","SBS","STR",
        "ORC", "HIT", "WND","FLT","BRS","SBR","SAX","HLD","SLD",
        "TEK","PLS","SFX","SYN","BPD","SPD","VOX", "PLK",
        "ETH","FRT","PRC","FX","BTS","DRM","CMB"
    };



};

Q_DECLARE_METATYPE(Patch*)

#endif // PATCH_H
