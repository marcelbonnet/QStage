#ifndef WAVEFORM_H
#define WAVEFORM_H

#include<QVariant>
#include<QMetaType>
#include<QString>

class Waveform : public QVariant
{
public:
    Waveform();
    Waveform(int id, QString nome, int groupType, int groupId, QString waveGroup, int oneShot, int number);
    int id;
    QString nome;
    int groupType;
    int groupId;
    QString waveGroup;
    int oneShot;
    int number;

};

Q_DECLARE_METATYPE(Waveform*)

#endif // WAVEFORM_H
