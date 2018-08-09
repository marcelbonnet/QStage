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
    SQLite::Statement   query(db, "SELECT * FROM config WHERE key LIKE 'tema-%' AND key != 'tema-default' ORDER BY key ASC");
    QMap<QString, QString> rs;
    while (query.executeStep())
    {
        const char* key   = query.getColumn(0);
        const char* val   = query.getColumn(1);

        rs.insert(QString(key), QString(val));
    }
    return rs;
}

void Controller::setUltimoTemaUsado(QString nomeTema) throw (std::exception) {
    SQLite::Database db(getDbPath().toUtf8().data(), SQLite::OPEN_READWRITE);
    SQLite::Statement   query(db, "UPDATE config SET val = ? WHERE key = 'tema-default'");

    query.bind(1, nomeTema.toUtf8().data());
    query.exec();
}

QString Controller::getUltimoTemaUsado() throw (std::exception) {
    SQLite::Database db(getDbPath().toUtf8().data() );
    SQLite::Statement   query(db, "SELECT val FROM config WHERE key = (SELECT val FROM config WHERE key = 'tema-default')");

    query.executeStep();
    const char* css = query.getColumn(0);
    return QString(css);
}