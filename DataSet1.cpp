#include "DataSet1.h"

DataSet1::DataSet1()
{

}

const QString DataSet1::getDataRepresentation()
{
    return QString("%1%2%3%4%5 %6%7%8%9 %10%11 %12%13")
            .arg(QString::number(b0,16))
            .arg(QString::number(b1,16))
            .arg(QString::number(b2,16))
            .arg(QString::number(b3,16))
            .arg(QString::number(b4,16))
            .arg(QString::number(b5,16))
            .arg(QString::number(b6,16))
            .arg(QString::number(b7,16))
            .arg(QString::number(b8,16))
            .arg(QString::number(b9,16))
            .arg(QString::number(b10,16))
            .arg(QString::number(b11,16))
            .arg(QString::number(b12,16))
            ;
}
