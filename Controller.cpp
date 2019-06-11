#include "Controller.h"
#include "QStageException.h"
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

void Controller::persistTema(QString key, QString css) noexcept(false) {
    SQLite::Database db(getDbPath().toUtf8().data(), SQLite::OPEN_READWRITE);
    SQLite::Statement   query(db, "SELECT key FROM config WHERE key = ?");

    if(!key.startsWith("tema-"))
        throw QStageException("O tema deve ter um nome (key) com o prefixo \"tema-\" para ser encontrado na tabela de config quando faço a query de temas existentes.");

    query.bind(1, key.toUtf8().data());

    try {

        bool b = query.executeStep();

        if(b){

            SQLite::Statement   q(db, "UPDATE config SET val = ? WHERE key = ?");
            q.bind(1, css.toUtf8().data());
            q.bind(2, key.toUtf8().data());
            q.exec();

        } else {
            SQLite::Statement   q(db, "INSERT INTO config (key, val) VALUES(?, ?)");
            q.bind(1, key.toUtf8().data());
            q.bind(2, css.toUtf8().data());
            q.exec();
        }

    } catch (SQLite::Exception &e) {
        throw e;
    }

}
void Controller::deleteTema(QString key) noexcept(false) {
    SQLite::Database db(getDbPath().toUtf8().data(), SQLite::OPEN_READWRITE);
    SQLite::Statement   query(db, "DELETE FROM config WHERE key = ?");

    if(!key.startsWith("tema-"))
        throw new QStageException("O tema deve ter um nome (key) com o prefixo \"tema-\" para ser encontrado na tabela de config quando faço a query de temas existentes.");

    query.bind(1, key.toUtf8().data());
    query.exec();
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

QMap<int, QString> Controller::queryPlaylists() throw (std::exception){
    SQLite::Database db(getDbPath().toUtf8().data() );
    SQLite::Statement   query(db, "SELECT id, titulo FROM playlist ORDER BY id ASC");

    QMap<int, QString> rs;
    while (query.executeStep())
    {
        int id            = query.getColumn(0);
        const char* titulo= query.getColumn(1);
        qDebug() << id << QString(titulo);
        rs.insert(id, QString(titulo));
    }
    return rs;
}

void Controller::updateMusicaTag(int musicaId, QRgb tag) throw (std::exception){
    SQLite::Database db(getDbPath().toUtf8().data(), SQLite::OPEN_READWRITE );
    SQLite::Statement   query(db, "UPDATE musicas SET tag = ? WHERE musica_id = ? ");


    query.bind(1, tag );
    query.bind(2, musicaId);
    query.exec();
}

QList<Waveform*> Controller::queryWaveforms() throw (std::exception){
    SQLite::Database db(getDbPath().toUtf8().data() );
    SQLite::Statement   query(db, "SELECT id, nome, groupType, groupId, waveGroup, oneShot, number FROM waveforms ORDER BY id ASC");

    QList<Waveform*> rs;
    while (query.executeStep())
    {
        int id            = query.getColumn(0);
        const char* nome= query.getColumn(1);
        int groupType = query.getColumn(2);
        int groupId = query.getColumn(3);
        const char* waveGroup = query.getColumn(4);
        int oneShot = query.getColumn(5);
        int  number = query.getColumn(6);

        Waveform *w = new Waveform(id, QString(nome),groupType, groupId, QString(waveGroup), oneShot, number);
        rs.append(w);
    }
    return rs;
}
