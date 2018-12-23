#include "PatchTone.h"

PatchTone::PatchTone()
{

}

PatchTone::PatchTone(Function func)
{
    functionMap = QMap<Function,QString>();
    //gerado com Vim Macro
    functionMap.insert(Tone_Switch,"Tone_Switch");
    functionMap.insert(Wave_Group_Type,"Wave_Group_Type");
    functionMap.insert(Wave_Group_ID,"Wave_Group_ID");
    functionMap.insert(Wave_Number,"Wave_Number");
    functionMap.insert(Wave_Gain,"Wave_Gain");
    functionMap.insert(FXM_Switch,"FXM_Switch");
    functionMap.insert(FXM_Color,"FXM_Color");
    functionMap.insert(FXM_Depth,"FXM_Depth");
    functionMap.insert(Tone_Delay_Mode,"Tone_Delay_Mode");
    functionMap.insert(Tone_Delay_Time,"Tone_Delay_Time");
    functionMap.insert(Velocity_Cross_Fade,"Velocity_Cross_Fade");
    functionMap.insert(Velocity_Range_Lower,"Velocity_Range_Lower");
    functionMap.insert(Velocity_Range_Upper,"Velocity_Range_Upper");
    functionMap.insert(Keyboard_Range_Lower,"Keyboard_Range_Lower");
    functionMap.insert(Keyboard_Range_Upper,"Keyboard_Range_Upper");
    functionMap.insert(Redamper_Control_Switch,"Redamper_Control_Switch");
    functionMap.insert(Volume_Control_Switch,"Volume_Control_Switch");
    functionMap.insert(Hold1_Control_Switch,"Hold1_Control_Switch");
    functionMap.insert(Pitch_Bend_Control_Switch,"Pitch_Bend_Control_Switch");
    functionMap.insert(Pan_Control_Switch,"Pan_Control_Switch");
    functionMap.insert(Controller_1_Destination_1,"Controller_1_Destination_1");
    functionMap.insert(Controller_1_Depth_1,"Controller_1_Depth_1");
    functionMap.insert(Controller_1_Destination_2,"Controller_1_Destination_2");
    functionMap.insert(Controller_1_Depth_2,"Controller_1_Depth_2");
    functionMap.insert(Controller_1_Destination_3,"Controller_1_Destination_3");
    functionMap.insert(Controller_1_Depth_3,"Controller_1_Depth_3");
    functionMap.insert(Controller_1_Destination_4,"Controller_1_Destination_4");
    functionMap.insert(Controller_1_Depth_4,"Controller_1_Depth_4");
    functionMap.insert(Controller_2_Destination_1,"Controller_2_Destination_1");
    functionMap.insert(Controller_2_Depth_1,"Controller_2_Depth_1");
    functionMap.insert(Controller_2_Destination_2,"Controller_2_Destination_2");
    functionMap.insert(Controller_2_Depth_2,"Controller_2_Depth_2");
    functionMap.insert(Controller_2_Destination_3,"Controller_2_Destination_3");
    functionMap.insert(Controller_2_Depth_3,"Controller_2_Depth_3");
    functionMap.insert(Controller_2_Destination_4,"Controller_2_Destination_4");
    functionMap.insert(Controller_2_Depth_4,"Controller_2_Depth_4");
    functionMap.insert(Controller_3_Destination_1,"Controller_3_Destination_1");
    functionMap.insert(Controller_3_Depth_1,"Controller_3_Depth_1");
    functionMap.insert(Controller_3_Destination_2,"Controller_3_Destination_2");
    functionMap.insert(Controller_3_Depth_2,"Controller_3_Depth_2");
    functionMap.insert(Controller_3_Destination_3,"Controller_3_Destination_3");
    functionMap.insert(Controller_3_Depth_3,"Controller_3_Depth_3");
    functionMap.insert(Controller_3_Destination_4,"Controller_3_Destination_4");
    functionMap.insert(Controller_3_Depth_4,"Controller_3_Depth_4");
    functionMap.insert(LFO1_Waveform,"LFO1_Waveform");
    functionMap.insert(LFO1_Key_Sync,"LFO1_Key_Sync");
    functionMap.insert(LFO1_Rate,"LFO1_Rate");
    functionMap.insert(LFO1_Offset,"LFO1_Offset");
    functionMap.insert(LFO1_Delay_Time,"LFO1_Delay_Time");
    functionMap.insert(LFO1_Fade_Mode,"LFO1_Fade_Mode");
    functionMap.insert(LFO1_Fade_Time,"LFO1_Fade_Time");
    functionMap.insert(LFO1_External_Sync,"LFO1_External_Sync");
    functionMap.insert(LFO2_Waveform,"LFO2_Waveform");
    functionMap.insert(LFO2_Key_Sync,"LFO2_Key_Sync");
    functionMap.insert(LFO2_Rate,"LFO2_Rate");
    functionMap.insert(LFO2_Offset,"LFO2_Offset");
    functionMap.insert(LFO2_Delay_Time,"LFO2_Delay_Time");
    functionMap.insert(LFO2_Fade_Mode,"LFO2_Fade_Mode");
    functionMap.insert(LFO2_Fade_Time,"LFO2_Fade_Time");
    functionMap.insert(LFO2_External_Sync,"LFO2_External_Sync");
    functionMap.insert(Coarse_Tune,"Coarse_Tune");
    functionMap.insert(Fine_Tune,"Fine_Tune");
    functionMap.insert(Random_Pitch_Depth,"Random_Pitch_Depth");
    functionMap.insert(Pitch_Keyfollow,"Pitch_Keyfollow");
    functionMap.insert(Pitch_Envelope_Depth,"Pitch_Envelope_Depth");
    functionMap.insert(Pitch_Envelope_Velocity_Sens,"Pitch_Envelope_Velocity_Sens");
    functionMap.insert(Pitch_Envelope_Velocity_Time1,"Pitch_Envelope_Velocity_Time1");
    functionMap.insert(Pitch_Envelope_Velocity_Time4,"Pitch_Envelope_Velocity_Time4");
    functionMap.insert(Pitch_Envelope_Time_Keyfollow,"Pitch_Envelope_Time_Keyfollow");
    functionMap.insert(Pitch_Envelope_Time_1,"Pitch_Envelope_Time_1");
    functionMap.insert(Pitch_Envelope_Time_2,"Pitch_Envelope_Time_2");
    functionMap.insert(Pitch_Envelope_Time_3,"Pitch_Envelope_Time_3");
    functionMap.insert(Pitch_Envelope_Time_4,"Pitch_Envelope_Time_4");
    functionMap.insert(Pitch_Envelope_Level_1,"Pitch_Envelope_Level_1");
    functionMap.insert(Pitch_Envelope_Level_2,"Pitch_Envelope_Level_2");
    functionMap.insert(Pitch_Envelope_Level_3,"Pitch_Envelope_Level_3");
    functionMap.insert(Pitch_Envelope_Level_4,"Pitch_Envelope_Level_4");
    functionMap.insert(Pitch_LFO1_Depth,"Pitch_LFO1_Depth");
    functionMap.insert(Pitch_LFO2_Depth,"Pitch_LFO2_Depth");
    functionMap.insert(Filter_Type,"Filter_Type");
    functionMap.insert(Cutoff_Frequency,"Cutoff_Frequency");
    functionMap.insert(Cutoff_Keyfollow,"Cutoff_Keyfollow");
    functionMap.insert(Resonance,"Resonance");
    functionMap.insert(Resonance_Velocity_Sens,"Resonance_Velocity_Sens");
    functionMap.insert(Filter_Envelope_Depth,"Filter_Envelope_Depth");
    functionMap.insert(Filter_Envelope_Velocity_Curve,"Filter_Envelope_Velocity_Curve");
    functionMap.insert(Filter_Envelope_Velocity_Sens,"Filter_Envelope_Velocity_Sens");
    functionMap.insert(Filter_Envelope_Velocity_Time1,"Filter_Envelope_Velocity_Time1");
    functionMap.insert(Filter_Envelope_Velocity_Time4,"Filter_Envelope_Velocity_Time4");
    functionMap.insert(Filter_Envelope_Time_Keyfollow,"Filter_Envelope_Time_Keyfollow");
    functionMap.insert(Filter_Envelope_Time_1,"Filter_Envelope_Time_1");
    functionMap.insert(Filter_Envelope_Time_2,"Filter_Envelope_Time_2");
    functionMap.insert(Filter_Envelope_Time_3,"Filter_Envelope_Time_3");
    functionMap.insert(Filter_Envelope_Time_4,"Filter_Envelope_Time_4");
    functionMap.insert(Filter_Envelope_Level_1,"Filter_Envelope_Level_1");
    functionMap.insert(Filter_Envelope_Level_2,"Filter_Envelope_Level_2");
    functionMap.insert(Filter_Envelope_Level_3,"Filter_Envelope_Level_3");
    functionMap.insert(Filter_Envelope_Level_4,"Filter_Envelope_Level_4");
    functionMap.insert(Filter_LFO1_Depth,"Filter_LFO1_Depth");
    functionMap.insert(Filter_LFO2_Depth,"Filter_LFO2_Depth");
    functionMap.insert(Tone_Level,"Tone_Level");
    functionMap.insert(Bias_Direction,"Bias_Direction");
    functionMap.insert(Bias_Position,"Bias_Position");
    functionMap.insert(Bias_Level,"Bias_Level");
    functionMap.insert(Level_Envelope_Velocity_Curve,"Level_Envelope_Velocity_Curve");
    functionMap.insert(Level_Envelope_Velocity_Sens,"Level_Envelope_Velocity_Sens");
    functionMap.insert(Level_Envelope_Velocity_Time1,"Level_Envelope_Velocity_Time1");
    functionMap.insert(Level_Envelope_Velocity_Time4,"Level_Envelope_Velocity_Time4");
    functionMap.insert(Level_Envelope_Time_Keyfollow,"Level_Envelope_Time_Keyfollow");
    functionMap.insert(Level_Envelope_Time_1,"Level_Envelope_Time_1");
    functionMap.insert(Level_Envelope_Time_2,"Level_Envelope_Time_2");
    functionMap.insert(Level_Envelope_Time_3,"Level_Envelope_Time_3");
    functionMap.insert(Level_Envelope_Time_4,"Level_Envelope_Time_4");
    functionMap.insert(Level_Envelope_Level_1,"Level_Envelope_Level_1");
    functionMap.insert(Level_Envelope_Level_2,"Level_Envelope_Level_2");
    functionMap.insert(Level_Envelope_Level_3,"Level_Envelope_Level_3");
    functionMap.insert(Level_LFO1_Depth,"Level_LFO1_Depth");
    functionMap.insert(Level_LFO2_Depth,"Level_LFO2_Depth");
    functionMap.insert(Tone_Pan,"Tone_Pan");
    functionMap.insert(Pan_Keyfollow,"Pan_Keyfollow");
    functionMap.insert(Random_Pan_Depth,"Random_Pan_Depth");
    functionMap.insert(Alternate_Pan_Depth,"Alternate_Pan_Depth");
    functionMap.insert(Pan_LFO1_Depth,"Pan_LFO1_Depth");
    functionMap.insert(Pan_LFO2_Depth,"Pan_LFO2_Depth");
    functionMap.insert(Output_Assign,"Output_Assign");
    functionMap.insert(MixEFX_Send_Level,"MixEFX_Send_Level");
    functionMap.insert(Chorus_Send_Level,"Chorus_Send_Level");
    functionMap.insert(Reverb_Send_Level,"Reverb_Send_Level");

    functionName = functionMap.value(func);
    function = func;

    switch (func) {
    //gerado com Vim Macro
    case	Tone_Switch:
        address = 0x0000;
        break;
    case	Wave_Group_Type:
        address = 0x0001;
        break;
    case	Wave_Group_ID:
        address = 0x0002;
        break;
    case	Wave_Number:
        address = 0x0003;
        break;
    case	Wave_Gain:
        address = 0x0005;
        break;
    case	FXM_Switch:
        address = 0x0006;
        break;
    case	FXM_Color:
        address = 0x0007;
        break;
    case	FXM_Depth:
        address = 0x0008;
        break;
    case	Tone_Delay_Mode:
        address = 0x0009;
        break;
    case	Tone_Delay_Time:
        address = 0x000A;
        break;
    case	Velocity_Cross_Fade:
        address = 0x000B;
        break;
    case	Velocity_Range_Lower:
        address = 0x000C;
        break;
    case	Velocity_Range_Upper:
        address = 0x000D;
        break;
    case	Keyboard_Range_Lower:
        address = 0x000E;
        break;
    case	Keyboard_Range_Upper:
        address = 0x000F;
        break;
    case	Redamper_Control_Switch:
        address = 0x0010;
        break;
    case	Volume_Control_Switch:
        address = 0x0011;
        break;
    case	Hold1_Control_Switch:
        address = 0x0012;
        break;
    case	Pitch_Bend_Control_Switch:
        address = 0x0013;
        break;
    case	Pan_Control_Switch:
        address = 0x0014;
        break;
    case	Controller_1_Destination_1:
        address = 0x0015;
        break;
    case	Controller_1_Depth_1:
        address = 0x0016;
        break;
    case	Controller_1_Destination_2:
        address = 0x0017;
        break;
    case	Controller_1_Depth_2:
        address = 0x0018;
        break;
    case	Controller_1_Destination_3:
        address = 0x0019;
        break;
    case	Controller_1_Depth_3:
        address = 0x001A;
        break;
    case	Controller_1_Destination_4:
        address = 0x001B;
        break;
    case	Controller_1_Depth_4:
        address = 0x001C;
        break;
    case	Controller_2_Destination_1:
        address = 0x001D;
        break;
    case	Controller_2_Depth_1:
        address = 0x001E;
        break;
    case	Controller_2_Destination_2:
        address = 0x001F;
        break;
    case	Controller_2_Depth_2:
        address = 0x0020;
        break;
    case	Controller_2_Destination_3:
        address = 0x0021;
        break;
    case	Controller_2_Depth_3:
        address = 0x0022;
        break;
    case	Controller_2_Destination_4:
        address = 0x0023;
        break;
    case	Controller_2_Depth_4:
        address = 0x0024;
        break;
    case	Controller_3_Destination_1:
        address = 0x0025;
        break;
    case	Controller_3_Depth_1:
        address = 0x0026;
        break;
    case	Controller_3_Destination_2:
        address = 0x0027;
        break;
    case	Controller_3_Depth_2:
        address = 0x0028;
        break;
    case	Controller_3_Destination_3:
        address = 0x0029;
        break;
    case	Controller_3_Depth_3:
        address = 0x002A;
        break;
    case	Controller_3_Destination_4:
        address = 0x002B;
        break;
    case	Controller_3_Depth_4:
        address = 0x002C;
        break;
    case	LFO1_Waveform:
        address = 0x002D;
        break;
    case	LFO1_Key_Sync:
        address = 0x002E;
        break;
    case	LFO1_Rate:
        address = 0x002F;
        break;
    case	LFO1_Offset:
        address = 0x0030;
        break;
    case	LFO1_Delay_Time:
        address = 0x0031;
        break;
    case	LFO1_Fade_Mode:
        address = 0x0032;
        break;
    case	LFO1_Fade_Time:
        address = 0x0033;
        break;
    case	LFO1_External_Sync:
        address = 0x0034;
        break;
    case	LFO2_Waveform:
        address = 0x0035;
        break;
    case	LFO2_Key_Sync:
        address = 0x0036;
        break;
    case	LFO2_Rate:
        address = 0x0037;
        break;
    case	LFO2_Offset:
        address = 0x0038;
        break;
    case	LFO2_Delay_Time:
        address = 0x0039;
        break;
    case	LFO2_Fade_Mode:
        address = 0x003A;
        break;
    case	LFO2_Fade_Time:
        address = 0x003B;
        break;
    case	LFO2_External_Sync:
        address = 0x003C;
        break;
    case	Coarse_Tune:
        address = 0x003D;
        break;
    case	Fine_Tune:
        address = 0x003E;
        break;
    case	Random_Pitch_Depth:
        address = 0x003F;
        break;
    case	Pitch_Keyfollow:
        address = 0x0040;
        break;
    case	Pitch_Envelope_Depth:
        address = 0x0041;
        break;
    case	Pitch_Envelope_Velocity_Sens:
        address = 0x0042;
        break;
    case	Pitch_Envelope_Velocity_Time1:
        address = 0x0043;
        break;
    case	Pitch_Envelope_Velocity_Time4:
        address = 0x0044;
        break;
    case	Pitch_Envelope_Time_Keyfollow:
        address = 0x0045;
        break;
    case	Pitch_Envelope_Time_1:
        address = 0x0046;
        break;
    case	Pitch_Envelope_Time_2:
        address = 0x0047;
        break;
    case	Pitch_Envelope_Time_3:
        address = 0x0048;
        break;
    case	Pitch_Envelope_Time_4:
        address = 0x0049;
        break;
    case	Pitch_Envelope_Level_1:
        address = 0x004A;
        break;
    case	Pitch_Envelope_Level_2:
        address = 0x004B;
        break;
    case	Pitch_Envelope_Level_3:
        address = 0x004C;
        break;
    case	Pitch_Envelope_Level_4:
        address = 0x004D;
        break;
    case	Pitch_LFO1_Depth:
        address = 0x004E;
        break;
    case	Pitch_LFO2_Depth:
        address = 0x004F;
        break;
    case	Filter_Type:
        address = 0x0050;
        break;
    case	Cutoff_Frequency:
        address = 0x0051;
        break;
    case	Cutoff_Keyfollow:
        address = 0x0052;
        break;
    case	Resonance:
        address = 0x0053;
        break;
    case	Resonance_Velocity_Sens:
        address = 0x0054;
        break;
    case	Filter_Envelope_Depth:
        address = 0x0055;
        break;
    case	Filter_Envelope_Velocity_Curve:
        address = 0x0056;
        break;
    case	Filter_Envelope_Velocity_Sens:
        address = 0x0057;
        break;
    case	Filter_Envelope_Velocity_Time1:
        address = 0x0058;
        break;
    case	Filter_Envelope_Velocity_Time4:
        address = 0x0059;
        break;
    case	Filter_Envelope_Time_Keyfollow:
        address = 0x005A;
        break;
    case	Filter_Envelope_Time_1:
        address = 0x005B;
        break;
    case	Filter_Envelope_Time_2:
        address = 0x005C;
        break;
    case	Filter_Envelope_Time_3:
        address = 0x005D;
        break;
    case	Filter_Envelope_Time_4:
        address = 0x005E;
        break;
    case	Filter_Envelope_Level_1:
        address = 0x005F;
        break;
    case	Filter_Envelope_Level_2:
        address = 0x0060;
        break;
    case	Filter_Envelope_Level_3:
        address = 0x0061;
        break;
    case	Filter_Envelope_Level_4:
        address = 0x0062;
        break;
    case	Filter_LFO1_Depth:
        address = 0x0063;
        break;
    case	Filter_LFO2_Depth:
        address = 0x0064;
        break;
    case	Tone_Level:
        address = 0x0065;
        break;
    case	Bias_Direction:
        address = 0x0066;
        break;
    case	Bias_Position:
        address = 0x0067;
        break;
    case	Bias_Level:
        address = 0x0068;
        break;
    case	Level_Envelope_Velocity_Curve:
        address = 0x0069;
        break;
    case	Level_Envelope_Velocity_Sens:
        address = 0x006A;
        break;
    case	Level_Envelope_Velocity_Time1:
        address = 0x006B;
        break;
    case	Level_Envelope_Velocity_Time4:
        address = 0x006C;
        break;
    case	Level_Envelope_Time_Keyfollow:
        address = 0x006D;
        break;
    case	Level_Envelope_Time_1:
        address = 0x006E;
        break;
    case	Level_Envelope_Time_2:
        address = 0x006F;
        break;
    case	Level_Envelope_Time_3:
        address = 0x0070;
        break;
    case	Level_Envelope_Time_4:
        address = 0x0071;
        break;
    case	Level_Envelope_Level_1:
        address = 0x0072;
        break;
    case	Level_Envelope_Level_2:
        address = 0x0073;
        break;
    case	Level_Envelope_Level_3:
        address = 0x0074;
        break;
    case	Level_LFO1_Depth:
        address = 0x0075;
        break;
    case	Level_LFO2_Depth:
        address = 0x0076;
        break;
    case	Tone_Pan:
        address = 0x0077;
        break;
    case	Pan_Keyfollow:
        address = 0x0078;
        break;
    case	Random_Pan_Depth:
        address = 0x0079;
        break;
    case	Alternate_Pan_Depth:
        address = 0x007A;
        break;
    case	Pan_LFO1_Depth:
        address = 0x007B;
        break;
    case	Pan_LFO2_Depth:
        address = 0x007C;
        break;
    case	Output_Assign:
        address = 0x007D;
        break;
    case	MixEFX_Send_Level:
        address = 0x007E;
        break;
    case	Chorus_Send_Level:
        address = 0x007F;
        break;
    case	Reverb_Send_Level:
        address = 0x0100;
        break;
    }


}
