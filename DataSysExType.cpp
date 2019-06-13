#include "DataSysExType.h"

DataSysExType::DataSysExType()
{

}



const QString DataSysExType::getDataRepresentation()
{
    if(TYPE == DATASET)
        return QString("DS %1%2%3%4%5 %6%7%8%9 %10%11 %12%13")
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
    else
        return QString("RQ %1%2%3%4%5 %6%7%8%9 %10%11%12%13 %13%14")
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
                .arg(QString::number(b13,16))
                .arg(QString::number(b14,16))
                ;
}
