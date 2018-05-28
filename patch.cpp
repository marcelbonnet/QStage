#include "patch.h"

Patch::Patch(){

}

Patch::Patch(QString nome,QString categoria, QString categoriaPai, int groupType, int groupId, int number)
{
    this->nome = nome;
    this->categoria = categoria;
    this->categoriaPai = categoriaPai;
    this->groupType = groupType;
    this->groupId = groupId;
    this->number = number;
}
