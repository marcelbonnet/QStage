#include "Controller.h"
#include <QDebug>

Controller::Controller()
{

}

QString Controller::getDbPath(){
    return (QString(getenv("XDG_CONFIG_HOME")) + "/QStage/qstage.db");
}


QMap<QString, QString> Controller::queryTemasUI() throw (std::exception) {
    SQLite::Database db(getDbPath().toUtf8().data());
    SQLite::Statement   query(db, "SELECT * FROM config ORDER BY key ASC");
    QMap<QString, QString> rs;
    while (query.executeStep())
    {
        const char* key   = query.getColumn(0);
        const char* val   = query.getColumn(1);

        rs.insert(QString(key), QString(val));
    }
    return rs;
}
