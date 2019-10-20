#ifndef ARPEGGIO_H
#define ARPEGGIO_H

#include <QString>
#include <QVariant>
#include <QMetaType>

/**
 * @brief The Arpeggio class para servir de Transfer Object
 * durante queries no banco de dados e como um tipo  de User Data
 * para as Combo Boxes da UI ,armazenando informação de Style, Motif ou Beat Pattern
 */
class Arpeggio : public QVariant
{
public:
    Arpeggio();
    Arpeggio(int value, QString label);
    int value;
    QString label;
};
Q_DECLARE_METATYPE(Arpeggio*)

#endif // ARPEGGIO_H
