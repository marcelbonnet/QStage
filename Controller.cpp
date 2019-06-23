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

QList<QList<int>>* Controller::queryDefaultPatches() noexcept(false){
    SQLite::Database db(getDbPath().toUtf8().data() );
    SQLite::Statement   query(db, "SELECT a,b,c FROM temp LIMIT 5");

    QList<QList<int>>* rs = new QList<QList<int>>();
    while (query.executeStep())
    {
        int groupType = query.getColumn(0);
        int groupId = query.getColumn(1);
        int  number = query.getColumn(2);
        QList<int> lista;
        lista.append(groupType);
        lista.append(groupId);
        lista.append(number);
        rs->append(lista);
    }
    return rs;
}

QList<QString>* Controller::getPatch(int id) noexcept(false){
    SQLite::Database db(getDbPath().toUtf8().data() );
    SQLite::Statement   query(db, "SELECT common, tone0, tone1, tone2, tone3, name FROM patches WHERE patch_id = ?");

    query.bind(1, id);

    QList<QString>* lista = new QList<QString>();
    while (query.executeStep())
    {
        const char* common = query.getColumn(0);
        const char* t0 = query.getColumn(1);
        const char* t1 = query.getColumn(2);
        const char* t2 = query.getColumn(3);
        const char* t3 = query.getColumn(4);
        const char* name = query.getColumn(5);


        lista->append(common);//deve ser o primeiro no append
        lista->append(t0); //a seguir append dos tones 0 a 3
        lista->append(t1);
        lista->append(t2);
        lista->append(t3);
        //dados adicionais, manter appendo no final
        lista->append(name);

    }
    return lista;
}

QList<Patch*>* Controller::queryPatches() noexcept(false){
    SQLite::Database db(getDbPath().toUtf8().data() );
    SQLite::Statement   query(db, "SELECT patch_id, patchGroupType, patchGroupId, patchNumber, name, roland, categoria FROM patches ");



    QList<Patch*>* lista = new QList<Patch*>();
    while (query.executeStep())
    {
        int id = query.getColumn(0);
        int gt = query.getColumn(1);
        int gid = query.getColumn(2);
        int number = query.getColumn(3);
        const char* nome = query.getColumn(4);
        int roland = query.getColumn(5);
        int categid = query.getColumn(6);

        Patch* p = new Patch(id, nome, gt, gid, number, (roland == 1), categid );

        lista->append(p);

    }
    return lista;
}

QList<Patch*>* Controller::queryPatches(bool r) noexcept(false){
    SQLite::Database db(getDbPath().toUtf8().data() );
    SQLite::Statement   query(db, "SELECT patch_id, patchGroupType, patchGroupId, patchNumber, name, roland, categoria FROM patches WHERE roland = ? ");

    query.bind(1, r? 1 : 0 );

    QList<Patch*>* lista = new QList<Patch*>();
    while (query.executeStep())
    {
        int id = query.getColumn(0);
        int gt = query.getColumn(1);
        int gid = query.getColumn(2);
        int number = query.getColumn(3);
        const char* nome = query.getColumn(4);
        int roland = query.getColumn(5);
        int categid = query.getColumn(6);

        Patch* p = new Patch(id, nome, gt, gid, number, (roland == 1), categid );
        lista->append(p);

    }
    return lista;
}

int Controller::insertPatch(int groupType, int groupId, int number, QString name, QString common
                            , QString tone0, QString tone1, QString tone2, QString tone3, int categoria) noexcept(false)  {
    SQLite::Database db(getDbPath().toUtf8().data(), SQLite::OPEN_READWRITE );
    SQLite::Statement   query(db, "INSERT INTO patches ( patchGroupType, patchGroupId, patchNumber,  name, common, tone0, tone1, tone2, tone3, categoria  ) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )  ");

    try {
        query.bind(1, groupType );
        query.bind(2, groupId);
        query.bind(3, number);
        query.bind(4, name.toUtf8().data());
        query.bind(5, common.toUtf8().data());
        query.bind(6, tone0.toUtf8().data());
        query.bind(7, tone1.toUtf8().data());
        query.bind(8, tone2.toUtf8().data());
        query.bind(9, tone3.toUtf8().data());
        query.bind(10, categoria);
        query.exec();
        return getLastInsertRowid();
    } catch (SQLite::Exception &e) {
        throw e;
    }

}

int Controller::updatePatch(int id, int groupType, int groupId, int number, QString name, QString common
                            , QString tone0, QString tone1, QString tone2, QString tone3, int categoria) noexcept(false)  {
    SQLite::Database db(getDbPath().toUtf8().data(), SQLite::OPEN_READWRITE );
    SQLite::Statement   query(db, "UPDATE patches SET patchGroupType = ?, patchGroupId = ?, patchNumber = ?,  name = ?, common = ?, tone0 = ?, tone1 = ?, tone2 = ?, tone3 = ?, categoria = ? WHERE roland=0 AND patch_id = ? ");

    try {
        query.bind(1, groupType );
        query.bind(2, groupId);
        query.bind(3, number);
        query.bind(4, name.toUtf8().data());
        query.bind(5, common.toUtf8().data());
        query.bind(6, tone0.toUtf8().data());
        query.bind(7, tone1.toUtf8().data());
        query.bind(8, tone2.toUtf8().data());
        query.bind(9, tone3.toUtf8().data());
        query.bind(10, categoria);
        query.bind(11, id);
        query.exec();
        return getLastInsertRowid();
    } catch (SQLite::Exception &e) {
        throw e;
    }

}

int Controller::getLastInsertRowid()  {
    SQLite::Database db(getDbPath().toUtf8().data(), SQLite::OPEN_READWRITE );
    SQLite::Statement   query(db, "SELECT last_insert_rowid()");

    try {
        int id = -1;
        while(query.executeStep()){
            id = query.getColumn(0);
        }
        return id;
    } catch (SQLite::Exception &e) {
        throw e;
    }

}

void Controller::removePatch(int id)  {
    SQLite::Database db(getDbPath().toUtf8().data(), SQLite::OPEN_READWRITE );
    SQLite::Statement   query(db, "DELETE FROM patches WHERE roland=0 AND patch_id = ?");

    try {
        query.bind(1, id);
        query.exec();
    } catch (SQLite::Exception &e) {
        throw e;
    }

}
