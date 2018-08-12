#ifndef MUSICA_H
#define MUSICA_H

#include <QMetaType>
#include <QString>
#include <QRgb>

class Musica
{
public:
    Musica();

//protected:
    int musicaId;
    QString titulo;
    QString html;
    QString programa;
    QRgb tag;
};

Q_DECLARE_METATYPE(Musica*)

#endif // MUSICA_H
