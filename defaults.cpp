#include "defaults.h"

Defaults::Defaults(){

}

QString * Defaults::notas(){
    QString notas[12];

    notas[0] = "C";
    notas[1] = "C#";
    notas[2] = "D";
    notas[3] = "D#";
    notas[4] = "E";
    notas[5] = "F";
    notas[6] = "F#";
    notas[7] = "G";
    notas[8] = "G#";
    notas[9] = "A";
    notas[10] = "A#";
    notas[11] = "B";

    return notas;
}

QStringList Defaults::getPatchToneLabels(){

    QStringList lista;

    lista << "Tone Switch"
          << "???"
          << "Wave"
          << "Gain"
          << "FXM"
          << "FXM Color"
          << "FXM Depth"
          << "Delay Mode"
          << "Delay Time"
          << "Vel Cross Fade"
          << "Vel Rg Lower"
          << "Vel Rg Upper"
          << "Kbd Rg Lower"
          << "Kbd Rg Upper"
          << "Redamper"
          << "Volume Ctrl"
          << "Hold=1 Ctrl"
          << "Pitch Bend Ctrl"
          << "Pan Ctrl"
          << "Ctrl 1 Dest 1"
          << "Ctrl 1 Depth 1"
          << "Ctrl 1 Dest 2"
          << "Ctrl 1 Depth 2"
          << "Ctrl 1 Dest 3"
          << "Ctrl 1 Depth 3"
          << "Ctrl 1 Dest 4"
          << "Ctrl 1 Depth 4"
          << "Ctrl 2 Dest 1"
          << "Ctrl 2 Depth 1"
          << "Ctrl 2 Dest 2"
          << "Ctrl 2 Depth 2"
          << "Ctrl 2 Dest 3"
          << "Ctrl 2 Depth 3"
          << "Ctrl 2 Dest 4"
          << "Ctrl 2 Depth 4"
          << "Ctrl 3 Dest 1"
          << "Ctrl 3 Depth 1"
          << "Ctrl 3 Dest 2"
          << "Ctrl 3 Depth 2"
          << "Ctrl 3 Dest 3"
          << "Ctrl 3 Depth 3"
          << "Ctrl 3 Dest 4"
          << "Ctrl 3 Depth 4"
          << "LFO1 Waveform"
          << "LFO1 Key Sync"
          << "LFO1 Rate"
          << "LFO1 Offset"
          << "LFO1 Delay Time"
          << "LFO1 Fade Mode"
          << "LFO1 Fade Time"
          << "LFO1 Ext Sync"
          << "LFO2 Waveform"
          << "LFO2 Key Sync"
          << "LFO2 Rate"
          << "LFO2 Offset"
          << "LFO2 Delay Time"
          << "LFO2 Fade Mode"
          << "LFO2 Fade Time"
          << "LFO2 Ext Sync"
          << "Coarse Tune"
          << "Fine Tune"
          << "Random Pitch Depth"
          << "Pitch Keyfollow"
          << "Pitch Env Depth"
          << "Pitch Env Vel Sens"
          << "Pitch Env Vel T1"
          << "Pitch Env Vel T4"
          << "Pitch Env Time KF"
          << "Pitch Env Time 1"
          << "Pitch Env Time 2"
          << "Pitch Env Time 3"
          << "Pitch Env Time 4"
          << "Pitch Env Level 1"
          << "Pitch Env Level 2"
          << "Pitch Env Level 3"
          << "Pitch Env Level 4"
          << "Pitch LFO1 Depth"
          << "Pitch LFO2 Depth"

          <<        "Filter Type"
                 <<        "Cutoff Freq"
                 <<        "Cutoff Kflw"
                 <<        "Resonance"
                 <<        "Resonance VelSens"
                 <<        "Filter Env Depth"
                 <<        "Filter Env Vel Curve"
                 <<        "Filter Env Vel Sens"
                 <<        "Filter Env Vel T1"
                 <<        "Filter Env Vel T4"
                 <<        "Filter Env T Kflw"
                 <<        "Filter Env T1"
                 <<        "Filter Env T2"
                 <<        "Filter Env T3"
                 <<        "Filter Env T4"
                 <<        "Filter Env L1"
                 <<        "Filter Env L2"
                 <<        "Filter Env L3"
                 <<        "Filter Env L4"
                 <<        "Filter LFO1 Dep"
                 <<        "Filter LFO2 Dep"
                 <<        "Tone Level"
                 <<        "Bias Direction"
                 <<        "Bias Position"
                 <<        "Bias Level"
                 <<        "Level Env Vel Curve"
                 <<        "Level Env Vel Sens"
                 <<        "Level Env Vel T1"
                 <<        "Level Env Vel T4"
                 <<        "Level Env T Kflw"
                 <<        "Level Env T1"
                 <<        "Level Env T2"
                 <<        "Level Env T3"
                 <<        "Level Env T4"
                 <<        "Level Env L1"
                 <<        "Level Env L2"
                 <<        "Level Env L3"
                 <<        "Level LFO1 Dep"
                 <<        "Level LFO2 Dep"
                 <<        "Tone Pan"
                 <<        "Pan Keyfollow"
                 <<        "Random Pan Depth"
                 <<        "Alternate Pan Depth"
                 <<        "Pan LFO1 Depth"
                 <<        "Pan LFO2 Depth"
                 <<        "Ouput Assign"
                 <<        "Mix/Efx Send Level"
                 <<        "Chorus Send Level"
                        <<        "Reverb Send Level"
                           ;

    return  lista;
}

QStringList Defaults::getControlesDestDepth(){
    QStringList lista;

    lista <<        "OFF"
    <<        "WG Pitch"
    <<        "TVF Cutoff Frequency"
    <<        "TVF Resonance"
    <<        "TVA Level"
    <<        "TVA Pan"
    <<        "Output Level Of Tone"
    <<        "Chorus Send Level of Tone"
    <<        "Reverb Send Level of Tone"
    <<        "LFO1 depth of WG Pitch"
    <<        "LFO2 depth of WG Pitch"
    <<        "LFO1 depth of TVF Cutoff Frequency"
    <<        "LFO2 depth of TVF Cutoff Frequency"
    <<        "LFO1 depth of TVA Level"
    <<        "LFO2 depth of TVA Level"
    <<        "LFO1 depth of TVA Pan"
    <<        "LFO2 depth of TVA Pan"
    <<        "LFO1 Rate"
    <<        "LFO2 Rate";

    return lista;
}



QStringList Defaults::getCategorias(){
    QStringList lista;

    lista <<        "NO ASSIGN"
          <<        "AC PNO"
          <<        "E PNO"
          <<        "KEYBOARD"
          <<        "BELL"
          <<        "MALLET"
          <<        "ORGAN"
          <<        "ACCORDION"
          <<        "HARMONICA"
          <<        "AC GUITAR"
          <<        "EL GUITAR"
          <<        "DIST GUITAR"
          <<        "BASS"
          <<        "SYNTH BASS"
          <<        "STRINGS"
          <<        "ORCHESTRA"
          <<        "HIT/STAB"
          <<        "WIND"
          <<        "FLUTE"
          <<        "AC BRASS"
          <<        "SYNTH BRASS"
          <<        "SAX"
          <<        "HARD LEAD"
          <<        "SOFT LEAD"
          <<        "TECHNO SYNTH"
          <<        "PULSATING"
          <<        "SYNTH EFX"
          <<        "OTHER SYNTH"
          <<        "BRIGHT PAD"
          <<        "SOFT PAD"
          <<        "VOX"
          <<        "PLUCKED"
          <<        "ETHNIC"
          <<        "FRETTED"
          <<        "PERCUSSION"
          <<        "SOUND FX"
          <<        "BEAT/GROOVE"
          <<        "DRUMS"
          <<        "COMBINATION";
    return lista;
}

void Defaults::addNotas(QComboBox *combo){
    int oitava = -1;
    int nota=0;
    for(int i=0; i<=127; i++){
        QString notaOit = QString("%1 %2").arg(notas()[nota]).arg(oitava) ;
        combo->addItem(notaOit, QVariant::fromValue(nota*oitava) );
        nota++;
        if(nota>11) {nota=0; oitava++;}
    }
}


void Defaults::addPatchToneControllerDestinations(QComboBox *c){
    for(int i=0; i<19; i++)
        c->addItem(getControlesDestDepth().at(i));
}


void Defaults::setSliderRange(QSlider *w, int min, int max){
    w->setMinimum(min);
    w->setMaximum(max);
}

void Defaults::setSpinRange(QSpinBox *w, int min, int max){
    w->setMinimum(min);
    w->setMaximum(max);

}

void Defaults::setLfoOffset(QComboBox *c){
    c->addItem("-100");
    c->addItem("-50");
    c->addItem("0");
    c->addItem("50");
    c->addItem("100");
}

void Defaults::setLfoFadeMode(QComboBox *c){
    c->addItem("Key On In");
    c->addItem("Key On Out");
    c->addItem("Key Off In");
    c->addItem("Key Off Out");
}

void Defaults::setExternalSync(QComboBox *c){
    c->addItem("Off");
    c->addItem("Clock");
    c->addItem("Tap");
}

void Defaults::setRandomPitchDepth(QComboBox *c){
    for(int i=0; i<=9; i++){
        c->addItem(QString::number(i));
    }
    for(int i=10; i<=90; i=i+10){
        c->addItem(QString::number(i));
    }
    for(int i=100; i<=1200; i=i+100){
        c->addItem(QString::number(i));
    }

}

void Defaults::setKeyfollow15(QComboBox *c){
    int v[16] = {
        -100,-70,-50,-30,-10,0,10,20,30,40,50,70,100,120,150,200
    };

    for(int i=0; i<16; i++){
        c->addItem(QString::number(v[i]));
    }
}

void Defaults::setTime17(QComboBox *c){
    int v[15] = {
        -100,-70,-50,-40,-30,-20,-10,0,10,20,30,40,50,70,100
    };

    for(int i=0; i<15; i++){
        c->addItem(QString::number(v[i]));
    }
}

void Defaults::setFilterType18(QComboBox *c){
    QString l[5] = {
        "OFF",
        "Low Pass Filter",
        "Band Pass Filter",
        "Hi Pass Filter",
        "Peaking Filter (ênfase)"
    };

    for(int i=0; i<5; i++){
        c->addItem(l[i]);
    }
    c->setMaximumWidth(120);
}

/**
Bias direction
* @brief setPatchTone19
* @param c
*/
void Defaults::setPatchTone19(QComboBox *c){
    QString l[4] = {
        "Lower",
        "Upper",
        "Lower/Upper",
        "All (Cruzado)"
    };

    for(int i=0; i<4; i++){
        c->addItem(l[i]);
    }
    c->setMaximumWidth(120);
}

void Defaults::setOutputAssign(QComboBox *c){
    QString l[4] = {
        "MIX",
        "EFX",
        "<DIRECT-1>",
        "<DIRECT-2>"
    };

    for(int i=0; i<4; i++){
        c->addItem(l[i]);
    }
    c->setMaximumWidth(120);
}



void Defaults::setLfoWaveform(QComboBox *c){
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

void Defaults::setVelocityCurves(QComboBox *c){
    for(int i=1; i<=7; i++){
        QPixmap pixmap(QString(":/icones/patchtone/tvf/v-curve-%1.png").arg(i));
        QIcon icon(pixmap);
        c->addItem(icon, QString::number(i));
        c->setIconSize(QSize(50,50));
    }
    c->setMaximumWidth(120);
}

