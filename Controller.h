#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <QMap>

class Controller
{
public:
    Controller();

    static QString getDbPath();

    static QMap<QString,QString> queryTemasUI() throw (std::exception);

};

#endif // CONTROLLER_H
