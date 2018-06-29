#ifndef MUSICA_H
#define MUSICA_H

#include <QMetaType>
#include <QString>

class Musica
{
public:
    Musica();

//protected:
    int musicaId;
    QString titulo;
    QString html;
    QString programa;
};

Q_DECLARE_METATYPE(Musica*)

#endif // MUSICA_H
