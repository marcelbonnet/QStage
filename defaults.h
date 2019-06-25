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

class Defaults {

public:
    Defaults();

    static QString * notas();

    static int getPatchToneLabelsCount(){ return getPatchToneLabels().count();  }

    static QStringList getPatchToneLabels();

    static QStringList getControlesDestDepth();

    static QStringList getCategorias();

    static void addNotas(QComboBox *combo);

    static void addPatchToneControllerDestinations(QComboBox *c);


    static void setSliderRange(QSlider *w, int min, int max);

    static void setSpinRange(QSpinBox *w, int min, int max);

    static void setLfoOffset(QComboBox *c);

    static void setLfoFadeMode(QComboBox *c);

    static void setExternalSync(QComboBox *c);

    static void setRandomPitchDepth(QComboBox *c);

    static void setKeyfollow15(QComboBox *c);

    static void setTime17(QComboBox *c);

    static void setFilterType18(QComboBox *c);

    /**
      Bias direction
     * @brief setPatchTone19
     * @param c
     */
    static void setPatchTone19(QComboBox *c);

    static void setOutputAssign(QComboBox *c);



    static void setLfoWaveform(QComboBox *c);

    static void setVelocityCurves(QComboBox *c);



};
#endif // DEFAULTS_H
