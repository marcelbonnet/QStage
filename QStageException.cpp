#include "QStageException.h"

QStageException::QStageException()
{

}

QStageException::QStageException(QString msg)
{
    this->msg = msg;
}

QString QStageException::getMessage(){
    return this->msg;
}
