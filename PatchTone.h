#ifndef PATCHTONE_H
#define PATCHTONE_H

#include <QString>
#include <QMap>
#include<QVariant>
#include<QMetaType>

/**
  Endereços de Patch Tone
 * @brief The PatchTone class
 */
class PatchTone : public QVariant
{
public:
    enum Function {
        //gerado com Vim Macro
        Tone_Switch,
        Wave_Group_Type,
        Wave_Group_ID,
        Wave_Number,
        Wave_Gain,
        FXM_Switch,
        FXM_Color,
        FXM_Depth,
        Tone_Delay_Mode,
        Tone_Delay_Time,
        Velocity_Cross_Fade,
        Velocity_Range_Lower,
        Velocity_Range_Upper,
        Keyboard_Range_Lower,
        Keyboard_Range_Upper,
        Redamper_Control_Switch,
        Volume_Control_Switch,
        Hold1_Control_Switch,
        Pitch_Bend_Control_Switch,
        Pan_Control_Switch,
        Controller_1_Destination_1,
        Controller_1_Depth_1,
        Controller_1_Destination_2,
        Controller_1_Depth_2,
        Controller_1_Destination_3,
        Controller_1_Depth_3,
        Controller_1_Destination_4,
        Controller_1_Depth_4,
        Controller_2_Destination_1,
        Controller_2_Depth_1,
        Controller_2_Destination_2,
        Controller_2_Depth_2,
        Controller_2_Destination_3,
        Controller_2_Depth_3,
        Controller_2_Destination_4,
        Controller_2_Depth_4,
        Controller_3_Destination_1,
        Controller_3_Depth_1,
        Controller_3_Destination_2,
        Controller_3_Depth_2,
        Controller_3_Destination_3,
        Controller_3_Depth_3,
        Controller_3_Destination_4,
        Controller_3_Depth_4,
        LFO1_Waveform,
        LFO1_Key_Sync,
        LFO1_Rate,
        LFO1_Offset,
        LFO1_Delay_Time,
        LFO1_Fade_Mode,
        LFO1_Fade_Time,
        LFO1_External_Sync,
        LFO2_Waveform,
        LFO2_Key_Sync,
        LFO2_Rate,
        LFO2_Offset,
        LFO2_Delay_Time,
        LFO2_Fade_Mode,
        LFO2_Fade_Time,
        LFO2_External_Sync,
        Coarse_Tune,
        Fine_Tune,
        Random_Pitch_Depth,
        Pitch_Keyfollow,
        Pitch_Envelope_Depth,
        Pitch_Envelope_Velocity_Sens,
        Pitch_Envelope_Velocity_Time1,
        Pitch_Envelope_Velocity_Time4,
        Pitch_Envelope_Time_Keyfollow,
        Pitch_Envelope_Time_1,
        Pitch_Envelope_Time_2,
        Pitch_Envelope_Time_3,
        Pitch_Envelope_Time_4,
        Pitch_Envelope_Level_1,
        Pitch_Envelope_Level_2,
        Pitch_Envelope_Level_3,
        Pitch_Envelope_Level_4,
        Pitch_LFO1_Depth,
        Pitch_LFO2_Depth,
        Filter_Type,
        Cutoff_Frequency,
        Cutoff_Keyfollow,
        Resonance,
        Resonance_Velocity_Sens,
        Filter_Envelope_Depth,
        Filter_Envelope_Velocity_Curve,
        Filter_Envelope_Velocity_Sens,
        Filter_Envelope_Velocity_Time1,
        Filter_Envelope_Velocity_Time4,
        Filter_Envelope_Time_Keyfollow,
        Filter_Envelope_Time_1,
        Filter_Envelope_Time_2,
        Filter_Envelope_Time_3,
        Filter_Envelope_Time_4,
        Filter_Envelope_Level_1,
        Filter_Envelope_Level_2,
        Filter_Envelope_Level_3,
        Filter_Envelope_Level_4,
        Filter_LFO1_Depth,
        Filter_LFO2_Depth,
        Tone_Level,
        Bias_Direction,
        Bias_Position,
        Bias_Level,
        Level_Envelope_Velocity_Curve,
        Level_Envelope_Velocity_Sens,
        Level_Envelope_Velocity_Time1,
        Level_Envelope_Velocity_Time4,
        Level_Envelope_Time_Keyfollow,
        Level_Envelope_Time_1,
        Level_Envelope_Time_2,
        Level_Envelope_Time_3,
        Level_Envelope_Time_4,
        Level_Envelope_Level_1,
        Level_Envelope_Level_2,
        Level_Envelope_Level_3,
        Level_LFO1_Depth,
        Level_LFO2_Depth,
        Tone_Pan,
        Pan_Keyfollow,
        Random_Pan_Depth,
        Alternate_Pan_Depth,
        Pan_LFO1_Depth,
        Pan_LFO2_Depth,
        Output_Assign,
        MixEFX_Send_Level,
        Chorus_Send_Level,
        Reverb_Send_Level
    };

    PatchTone();
    PatchTone(Function func, int whichTone);

    int address = 0x0000;
    int whichTone = 0;//0=Patch Common e Patch Tones de 1 a 4

    bool is2ByteData = false;

    QString functionName;
    Function function;


private:
    QMap<Function,QString> functionMap;
};

Q_DECLARE_METATYPE(PatchTone*);

#endif // PATCHTONE_H
