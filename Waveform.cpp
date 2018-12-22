#include "Waveform.h"

Waveform::Waveform()
{

}

Waveform::Waveform(int id, QString nome, int groupType, int groupId, QString waveGroup, int oneShot, int number)
{
    this->id=id;
    this->nome=nome;
    this->groupId = groupId;
    this->groupType = groupType;
    this->waveGroup = waveGroup;
    this->oneShot = oneShot;
    this->number = number;

}
