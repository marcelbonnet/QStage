#ifndef PATCH_H
#define PATCH_H

#include <QString>
#include <QVariant>
#include <QMetaType>

class Patch : public QVariant
{
public:
    Patch(); //default
    Patch(QString nome, QString categoria, QString categoriaPai, int groupType, int groupId, int number);

    QString getNome(){
        return nome;
    }

    QString getCategoria(){
        return categoria;
    }

    QString getCategoriaPai(){
        return categoriaPai;
    }

    int getGroupType(){
        return groupType;
    }

    int getGroupId(){
        return groupId;
    }

    int getNumber(){
        return number;
    }

//private:
    QString nome;
    QString categoria;
    QString categoriaPai;
    int groupType;
    int groupId;
    int number;
};

Q_DECLARE_METATYPE(Patch*)

#endif // PATCH_H
