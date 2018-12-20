#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <QDebug>
#include <QString>
#include <QList>
#include <QComboBox>
#include <QPixmap>
#include <QIcon>
#include <QSpinBox>
#include <QSlider>


QString notas[12] = {
    "C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
};

const int PATCH_TONE_LABELS_COUNT = 126;
QString PATCH_TONE_LABELS[PATCH_TONE_LABELS_COUNT] = {
    "ON/OFF",
    "Wave",
    "Gain",
    "FXM",
    "FXM Color",
    "FXM Depth",
    "Delay Mode",
    "Delay Time",
    "Vel CrossF",
    "Vel Low",
    "Vel Hi",
    "Key Low",
    "Key Hi",
    "Redamper",
    "Vol Ctrl",
    "Hold1 Ctrl",
    "P.Bend Ctrl",
    "Pan Ctrl",
    "Ctr1 Dest1",
    "Ctr1 Depth1",
    "Ctr1 Dest2",
    "Ctr1 Depth2",
    "Ctr1 Dest3",
    "Ctr1 Depth3",
    "Ctr1 Dest4",
    "Ctr1 Depth4",
    "Ctr2 Dest1",
    "Ctr2 Depth1",
    "Ctr2 Dest2",
    "Ctr2 Depth2",
    "Ctr2 Dest3",
    "Ctr2 Depth3",
    "Ctr2 Dest4",
    "Ctr2 Depth4",
    "Ctr3 Dest1",
    "Ctr3 Depth1",
    "Ctr3 Dest2",
    "Ctr3 Depth2",
    "Ctr3 Dest3",
    "Ctr3 Depth3",
    "Ctr3 Dest4",
    "Ctr3 Depth4",
    "LFO1 Waveform",
    "LFO1 Key Sync",
    "LFO1 Rate",
    "LFO1 Offset",
    "LFO1 DelayT",
    "LFO1 FadeMode",
    "LFO1 FadeT",
    "LFO1 ExtSync",
    "LFO2 Waveform",
    "LFO2 Key Sync",
    "LFO2 Rate",
    "LFO2 Offset",
    "LFO2 DelayT",
    "LFO2 FadeMode",
    "LFO2 FadeT",
    "LFO2 ExtSync",
    "Coarse Tune",
    "Fine Tune",
    "Rnd. Pitch Depth",
    "Pitch Keyfollow",
    "Pitch Env Depth",
    "Pitch Env Vel Sens",
    "Pitch Env Vel T1",
    "Pitch Env Vel T4",
    "Pitch Env T Kflw",
    "Pitch Env T1",
    "Pitch Env T2",
    "Pitch Env T3",
    "Pitch Env T4",
    "Pitch Env L1",
    "Pitch Env L2",
    "Pitch Env L3",
    "Pitch Env L4",
    "Pitch LFO1 Dep",
    "Pitch LFO2 Dep",
    "Filter Type",
    "Cutoff Freq",
    "Cutoff Kflw",
    "Resonance",
    "Resonance VelSens",
    "Filter Env Depth",
    "Filter Env Vel Curve",
    "Filter Env Vel Sens",
    "Filter Env Vel T1",
    "Filter Env Vel T4",
    "Filter Env T Kflw",
    "Filter Env T1",
    "Filter Env T2",
    "Filter Env T3",
    "Filter Env T4",
    "Filter Env L1",
    "Filter Env L2",
    "Filter Env L3",
    "Filter Env L4",
    "Filter LFO1 Dep",
    "Filter LFO2 Dep",
    "Tone Level",
    "Bias Direction",
    "Bias Position",
    "Bias Level",
    "Level Env Vel Curve",
    "Level Env Vel Sens",
    "Level Env Vel T1",
    "Level Env Vel T4",
    "Level Env T Kflw",
    "Level Env T1",
    "Level Env T2",
    "Level Env T3",
    "Level Env T4",
    "Level Env L1",
    "Level Env L2",
    "Level Env L3",
    "Level LFO1 Dep",
    "Level LFO2 Dep",
    "Tone Pan",
    "Pan Keyfollow",
    "Random Pan Depth",
    "Pan LFO1 Depth",
    "Pan LFO2 Depth",
    "Ouput Assign",
    "Mix/Efx Send Level",
    "Chorus Send Level",
    "Reverb Send Level"

};

QString CONTROLES_DEST_DEPTH[19] = {
    "OFF",
    "WG Pitch",
    "TVF Cutoff Frequency",
    "TVF Resonance",
    "TVA Level",
    "TVA Pan",
    "Output Level Of Tone",
    "Chorus Send Level of Tone",
    "Reverb Send Level of Tone",
    "LFO1 depth of WG Pitch",
    "LFO2 depth of WG Pitch",
    "LFO1 depth of TVF Cutoff Frequency",
    "LFO2 depth of TVF Cutoff Frequency",
    "LFO1 depth of TVA Level",
    "LFO2 depth of TVA Level",
    "LFO1 depth of TVA Pan",
    "LFO2 depth of TVA Pan",
    "LFO1 Rate",
    "LFO2 Rate"
};

void addNotas(QComboBox *combo){
    for(int oitava=-1; oitava<=9; oitava++){
        for(int nota=0; nota<12 && nota*oitava <= 127  ; nota++){
            QString notaOit = QString("%1 %2").arg(notas[nota]).arg(oitava) ;
            combo->addItem(notaOit, QVariant::fromValue(nota*oitava) );
        }
    }
}

void addPatchToneControllerDestinations(QComboBox *c){
    for(int i=0; i<19; i++)
        c->addItem(CONTROLES_DEST_DEPTH[i]);
}

void setSliderRange(QSlider *w, int min, int max){
    w->setMinimum(min);
    w->setMaximum(max);
}

void setSpinRange(QSpinBox *w, int min, int max){
    w->setMinimum(min);
    w->setMaximum(max);
}

void setLfoOffset(QComboBox *c){
    c->addItem("-100");
    c->addItem("-50");
    c->addItem("0");
    c->addItem("50");
    c->addItem("100");
}

void setLfoFadeMode(QComboBox *c){
    c->addItem("Key On In");
    c->addItem("Key On Out");
    c->addItem("Key Off In");
    c->addItem("Key Off Out");
}
void setExternalSync(QComboBox *c){
    c->addItem("Off");
    c->addItem("Clock");
    c->addItem("Tap");
}

void setLfoWaveform(QComboBox *c){
    QString icons[8] = {
        "tri", "sin", "saw", "sqr", "trap", "samplehold",
        "random", "caos"
    };
    QString l[8] = {
        "Triangular", "Senóide", "Dente Serra",
        "Quadrada", "Trapezoidal",
        "Sample And Hold", "Aleatória", "Caótica"
    };

    for(int i=0; i<8; i++){
        QPixmap pixmap(QString(":/icones/patchtone/lfo/%1.png").arg(icons[i]));
        QIcon icon(pixmap);
        c->addItem(icon, l[i]);
        c->setIconSize(QSize(50,50));
        c->setMaximumSize(120,60);
    }

}

/**
  Nesta ordem, o valor decimal correspondente no XP-30
  vai de 0 até 38.
 * */
QString categorias[39] = {
    "NO ASSIGN",
    "AC PNO",
    "E PNO",
    "KEYBOARD",
    "BELL",
    "MALLET",
    "ORGAN",
    "ACCORDION",
    "HARMONICA",
    "AC GUITAR",
    "EL GUITAR",
    "DIST GUITAR",
    "BASS",
    "SYNTH BASS",
    "STRINGS",
    "ORCHESTRA",
    "HIT/STAB",
    "WIND",
    "FLUTE",
    "AC BRASS",
    "SYNTH BRASS",
    "SAX",
    "HARD LEADD",
    "SOFT LEADD",
    "TECHNO SYNTH",
    "PULSATING",
    "SYNTH EFX",
    "OTHER SYNTH",
    "BRIGHT PAD",
    "SOFT PAD",
    "VOX",
    "PLUCKED",
    "ETHNIC",
    "FRETTED",
    "PERCUSSION",
    "SOUND FX",
    "BEAT/GROOVE",
    "DRUMS",
    "COMBINATION"
};

#endif // DEFAULTS_H
