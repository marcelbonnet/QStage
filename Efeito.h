#ifndef EFEITO_H
#define EFEITO_H

#include <QString>
#include <QStringList>

class Efeito
{
public:
    Efeito();

    static const QString nomes[40] ;
    static const int paramsMax[40][13];
    static const QString paramsLabel[40][12];

    QStringList *getLabels(int index);
    int getQtdeParametros(int index);
    QList<int>* getParametrosMaxVal(int index);
};

#endif // EFEITO_H
