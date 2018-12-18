#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <QString>
#include <QList>


QString CONTROLES_DEST_DEPTH[19] = {
    "OFF",
    "WG Pitch",
    "TVF Cutoff Freq",
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
