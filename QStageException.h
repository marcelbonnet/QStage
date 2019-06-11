#ifndef QSTAGEEXCEPTION_H
#define QSTAGEEXCEPTION_H

#include <QException>
#include <QString>

class QStageException : public QException
{
public:
    QStageException();
    QStageException(QString msg);
    QString getMessage();
protected:
    QString msg;
};

#endif // QSTAGEEXCEPTION_H
