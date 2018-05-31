#include "Efeito.h"

Efeito::Efeito()
{


}

const QString Efeito::nomes[40] = {
    "StereoEQ",
    "Overdrive",
    "Distortion",
    "Phaser",
    "Spectrum",
    "Enhancer",
    "AutoWah",
    "Rotary",
    "Compressor",
    "Limiter",
    "HexaChorus",
    "TremoloChorus",
    "SpaceD",
    "StereoChorus",
    "StereoFlanger",
    "StepFlanger",
    "StereoDelay",
    "ModulationDelay",
    "TripleTapDelay",
    "QuadrupleTapDelay",
    "TimeControlDelay",
    "TwoVoicePitchShiter",
    "FeedbackPitchShiter",
    "Reverb",
    "GateReverb",
    "OverdriveChorus",
    "OverdriveFlanger",
    "OverdriveDelay",
    "DistortionChorus",
    "DistortionFlanger",
    "DistortionDelay",
    "EnhancerChorus",
    "EnhancerFlanger",
    "EnhancerDelay",
    "ChorusDelay",
    "FlangerDelay",
    "ChorusFlanger",
    "ChorusDelayParalelo",
    "FlangerDelayParalelo",
    "ChorusFlangerParalelo"
};

const int Efeito::paramsMax[40][13] = {
      {//StereoEQ
        //qtde de params, max levels ...
        11,     1,30,1,30,16,4,30,16,4,30,127
    },
    {//Overdrive,
        6,      127,127,3,30,30,127
    },
    {//Distortion
        6,      127,127,3,30,30,127
    },
    {//Phaser
        7,      125,125,127,127,127,127,127
    },
    {
        //Spectrum
        11,     30,30,30,30,30,30,30,30,4,127,127
    },
    {
        //Enhancer
        5,      127,127,30,30,127
    },
    {
        //AutoWah
        7,      1,125,127,127,127,127,127
    },
    {
        //Rotary
        11,     125,125,125,125,1,15,15,127,127,127,127
    },
    {
        //Compressor
        7,      127,127,127,3,30,30,127
    },
    {
        //Limiter
        8,      127,127,3,127,3,30,30,127
    },
    {
        //HexaChorus
        8,      125,125,127,20,40,20,100,127
    },{
        //        TremoloChorus
        8,      125,125,127,125,127,90,100,127
        }
        ,{
        //        SpaceD
        8,      125,125,127,90,30,30,100,127
        }
        ,{
        //        StereoChorus
        11,     2,16,125,125,127,90,0,30,30,100,127
        }
        ,{
        //        StereoFlanger
        11,     2,16,125,125,127,90,98,30,30,100,127
        }
        ,{
        //        StepFlanger
        10,     125,125,127,98,125,90,30,30,100,127
        }
        ,{
        //        StereoDelay
        11,     1,126,126,1,1,98,17,30,30,100,127
        }
        ,{
        //        ModulationDelay
        12,     1,126,126,98,17,125,127,90,30,30,100,127
        }
        ,{
        //        TripleTapDelay
            12,     125,125,125,98,17,127,127,127,30,30,100,127
        }
        ,{
        //        QuadrupleTapDelay
            12,     125,125,125,125,127,127,127,127,98,17,100,127
        }
        ,{
        //        TimeControlDelay
        9,  120,98,15,17,127,30,30,100,127
        }
        ,{
        //        TwoVoicePitchShiter
        12,     4,36,36,100,100,126,126,127,127,100,100,127
        }
        ,{
        //        FeedbackPitchShiter
        10,     4,36,100,126,98,127,30,30,100,127
        }
        ,{
        //        Reverb
        8,      5,125,127,17,30,30,100,127
        }
        ,{
        //        GateReverb
        7,      3,125,99,30,30,100,127
        }
        ,{
        //        OverdriveChorus
        8,  127,127,125,125,127,0,100,127
        }
        ,{
        //        OverdriveFlanger
        8,  127,127,125,125,127,98,100,127
        }
        ,{
        //        OverdriveDelay
        7,  127,127,126,98,17,100,127
        }
        ,{
        //        DistortionChorus
        8,  127,127,125,125,127,0,100,127
        }
        ,{
        //        DistortionFlanger
        8,  127,127,125,125,127,98,100,127
        }
        ,{
        //        DistortionDelay
        7,  127,127,126,98,17,100,127
        }
        ,{
        //        EnhancerChorus
        8,  127,127,125,125,127,0,100,127
        }
        ,{
        //        EnhancerFlanger
        8,  127,127,125,125,127,98,100,127
        }
        ,{
        //        EnhancerDelay
        8,  127,127,126,98,17,0,100,127
        }
        ,{
        //        ChorusDelay
        10, 125,125,127,0,100,126,98,17,100,127
        }
        ,{
        //        FlangerDelay
        10, 125,125,127,98,100,126,98,17,100,127
        }
        ,{
        //        ChorusFlanger
        10, 125,125,127,100,125,125,127,98,100,127
        }
        ,{
        //        ChorusDelayParalelo
        10, 125,125,127,0,100,126,98,17,100,127
        }
        ,{
        //        FlangerDelayParalelo
        10, 125,125,127,98,100,126,98,17,100,127
        }
        ,{
        //        ChorusFlangerParalelo
        10, 125,125,127,100,125,125,127,98,100,127
    }
};

const QString Efeito::paramsLabel[40][12] = {
    {
       //StereoEQ
        "Low Frequency", "Low Gain", "High Frequency", "High Gain", "Peak1 Freq", "Peak1 Q", "Peak1 Gain", "Peak2 Freq", "Peak2 Q", "Peak2 Gain", "Level"
    },
    {
        //Overdrive,
        "Drive","Output Pan","Amp Simul Type", "Low Gain", "High Gain", "Out Level"
    },
    {
        //Distortion
        "Drive","Output Pan","Amp Simul Type", "Low Gain", "High Gain", "Out Level"
    },
    {
        //Phaser
        "Manual","Rate","Depth","Resonance","Mix Level","Out Pan","Out Level"
    },
    {
        //Spectrum
        "Band Gain 1", "Band Gain 2", "Band Gain 3", "Band Gain 4", "Band Gain 5", "Band Gain 6", "Band Gain 7", "Band Gain 8", "Q", "Out Pan", "Out Level"
    },
    {
        //Enhancer
        "Sens","Mix Level","Low Gain","High Gain","Out Level"
    },
    {
        //AutoWah
        "Filter Type","Rate","Depth","Sens","Manual","Peak","Out Level"
    },
    {
        //Rotary
        "High Freq Slow Rate", "Low Freq Slow Rate", "Hi Freq Fast Rate", "Low Freq Fast Rate", "Speed", "High Freq Accel", "Low Freq Accel", "High Freq Level", "Low Freq Level", "Separation", "Out Level"
    },
    {
        //Compressor
        "Sustain","Attack","Out Pan","Post Gain","Low Gain","Hi Gain","Out Level"
    },
    {
        //Limiter
        "Threshold Level", "Release Time","Compression Ratio","Out Pan","Post Gain","Low Gain", "Hi Gain", "Out Level"
    },
    {
        //HexaChorus
        "Pre Delay Time", "Rate","Depth","Pre Delay Deviation","Depth Deviation","Pan Deviation","Efx Balance","Out Level"
    },{
        //        TremoloChorus
        "Pre Delay Time","Chorus Rate","Chorus Depth","Tremolo Rate","Tremolo Phase","Efx Balance","Out Level"
        }
        ,{
        //        SpaceD
        "Pre Delay Time","Rate","Depth","Phase","Low Gain"
        }
        ,{
        //        StereoChorus
        "Filter Type", "Cutoff Freq", "Pre Delay Time", "Rate", "Depth", "Phase","sem uso","Low Gain","Hi Gain","Efx Balance","Out Level"
        }
        ,{
        //        StereoFlanger
        "Filter Type", "Cutoff Freq", "Pre Delay Time", "Rate", "Depth", "Phase","Feedback Level","Low Gain","Hi Gain","Efx Balance","Out Level"
        }
        ,{
        //        StepFlanger
        "Pre Delay Time","Rate","Depth","Feedback Level", "Step Rate","Phase","Low Gain","Hi Gain","Efx Balance","Out Level"
        }
        ,{
        //        StereoDelay
        "Feedback Mode", "Delay Time Left", "Delay Time Right", "Feedback Phase Left", "Feedback Phase Right", "Feedback Level", "HF Damp","Low Gain","Hi Gain", "Efx Balance","Out Level"
        }
        ,{
        //        ModulationDelay
        "Feedback Mode", "Delay Time Left", "Delay Time Right", "Feedback Level", "HF Damp", "Rate","Depth","Phase","Low Gain","Hi Gain","Efx Balance","Out Level"
        }
        ,{
        //        TripleTapDelay
        "Delay Time Left", "Delay Time Right", "Delay Time Center","Feedback Level","HF Damp","Left Level", "Right Level", "Center Level","Low Gain","Hi Gain","Efx Balance", "Out Level"
        }
        ,{
        //        QuadrupleTapDelay
        "Delay Time 1", "Delay Time 2", "Delay Time 3", "Delay Time 4", "Level 1", "Level 2", "Level 3", "Level 4","Feedback Level", "HF Damp","Efx Balance","Out Level"
        }
        ,{
        //        TimeControlDelay
        "Delay Time", "Feedback Level", "Acceleration","HF Damp", "Out Pan", "Low Gain", "Hi Gain", "Efx Balance", "Out Level"
        }
        ,{
        //        TwoVoicePitchShiter
        "Pitch Shifter Mode", "Coarse Pitch A","Coarse Pitch B","Fine Pitch A","Fine Pitch B", "Pre Delay Time A", "Pre Delay Time B","Out Pan A","Out Pan B","Level Balance","Out Level"
        }
        ,{
        //        FeedbackPitchShiter
        "Pitch Shifter Mode","Coarse Pitch","Fine Pitch","Pre Delay Time","Feedback Level","Out Pan","Low Gain","Hi Gain","Efx Balance","Out Level"
        }
        ,{
        //        Reverb
        "Reverb Type","Pre Delay Time","Gate Time","HF Damp","Low Gain","High Gain","Efx Balance","Out Level"
        }
        ,{
        //        GateReverb
        "Gate-Reverb Type","Pre Delay Type","Gate Time","Low Gain","Hi Gain","Efx Balance","Out Level"
        }
        ,{
        //        OverdriveChorus
        "Drive","Over Drive Pan","Chorus Pre Delay Time","Chorus Rate","Chorus Depth","not used","Chorus Balance","Out Level"
        }
        ,{
        //        OverdriveFlanger
        "Drive","Over Drive Pan","Flanger Pre Delay Time","Flanger Rate","Flanger Depth","Flanger Feedback Level","Flanger Balance","Out Level"
        }
        ,{
        //        OverdriveDelay
        "Drive","Over Drive Pan","Delay Time","Delay Feedback Level","Delay HF Damp","Delay Balance","Out Level"
        }
        ,{
        //        DistortionChorus
        "Distortion Drive",
        "Distortion Pan",
        "Chorus Pre Delay Time",
        "Chorus Rate",
        "Chorus Depth",
        "(not used)",
        "Chorus Balance",
        "Output Level"
        }
        ,{
        //        DistortionFlanger
        "Distortion Drive",
        "Distortion Pan",
        "Flanger Pre Delay Time",
        "Flanger Rate",
        "Flanger Depth",
        "Flanger Feedback Level",
        "Flanger Balance",
        "Output Level",
        }
        ,{
        //        DistortionDelay
        "Distortion Drive",
        "Distortion Pan",
        "Delay Time",
        "Delay Feedback Level",
        "Delay HF Damp",
        "Delay Balance",
        "Output Level",
        }
        ,{
        //        EnhancerChorus
        "Enhancer Sens",
        "Enhancer Mix Level",
        "Chorus Pre Delay Time",
        "Chorus Rate",
        "Chorus Depth",
        "(not used)",
        "Chorus Balance",
        "Output Level",
        }
        ,{
        //        EnhancerFlanger
        "Enhancer Sens",
        "Enhancer Mix Level",
        "Chorus Pre Delay Time",
        "Chorus Rate",
        "Chorus Depth",
        "(not used)",
        "Chorus Balance",
        "Output Level",
        }
        ,{
        //        EnhancerDelay
        "Enhancer Sens",
        "Enhancer Mix Level",
        "Delay Time",
        "Delay Feedback Level",
        "Delay HF Damp",
        "(not used)",
        "Delay Balance",
        "Output Level",
        }
        ,{
        //        ChorusDelay
        "Chorus Pre Delay Time",
        "Chorus Rate",
        "Chorus Depth",
        "(not used)",
        "Chorus Balance",
        "Delay Time",
        "Delay Feedback Level",
        "Delay HF Damp",
        "Delay Balance",
        "Output Level",
        }
        ,{
        //        FlangerDelay
        "Flanger Pre Delay Time",
        "Flanger Rate",
        "Flanger Depth",
        "Flanger Feedback Level",
        "Flanger Balance",
        "Delay Time",
        "Delay Feedback Level",
        "Delay HF Damp",
        "Delay Balance",
        "Output Level",
        }
        ,{
        //        ChorusFlanger
        "Chorus Pre Delay Time",
        "Chorus Rate",
        "Chorus Depth",
        "Chorus Balance",
        "Flanger Pre Delay Time",
        "Flanger Rate",
        "Flanger Depth",
        "Flanger Feedback Level",
        "Flanger Balance",
        "Output Level",
        }
        ,{
        //        ChorusDelayParalelo
        "Chorus Pre Delay Time",
        "Chorus Rate",
        "Chorus Depth",
        "(not used)",
        "Chorus Balance",
        "Delay Time",
        "Delay Feedback Level",
        "Delay HF Damp",
        "Delay Balance",
        "Output Level",
        }
        ,{
        //        FlangerDelayParalelo
        "Flanger Pre Delay Time",
        "Flanger Rate",
        "Flanger Depth",
        "Flanger Feedback Level",
        "Flanger Balance",
        "Delay Time",
        "Delay Feedback Level",
        "Delay HF Damp",
        "Delay Balance",
        "Output Level",
        }
        ,{
        //        ChorusFlangerParalelo
        "Chorus Pre Delay Time",
        "Chorus Rate",
        "Chorus Depth",
        "Chorus Balance",
        "Flanger Pre Delay Time",
        "Flanger Rate",
        "Flanger Depth",
        "Flanger Feedback Level",
        "Flanger Balance",
        "Output Level",
    }
};


QStringList* Efeito::getLabels(int index){
    QStringList * labels = new QStringList();

    for(int i=0; i<getQtdeParametros(index); i++ ){
        labels->append(paramsLabel[index][i]);
    }
    //complementa com vazio
    for(int i=getQtdeParametros(index); i<12; i++ ){
        labels->append("");
    }
    return labels;
}

int Efeito::getQtdeParametros(int index){
    return paramsMax[index][0];
}

QList<int> *Efeito::getParametrosMaxVal(int index){
    QList<int> *params = new QList<int>();

    for(int i=1; i<=getQtdeParametros(index); i++ ){
        params->append(paramsMax[index][i]);

    }

    //complementa com vazio
    for(int i=getQtdeParametros(index); i<12; i++ ){
        params->append(0);
    }

    return params;
}
