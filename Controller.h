#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <QMap>
#include <QRgb>

class Controller
{
public:
    Controller();

    static QString getDbPath();

    static QMap<QString,QString> queryTemasUI() throw (std::exception);
    static void setUltimoTemaUsado(QString nomeTema) throw (std::exception);
    static QString getUltimoTemaUsado() throw (std::exception);
    static void persistTema(QString key, QString css) noexcept(false);
    static void deleteTema(QString key) noexcept(false);

    static QMap<int, QString> queryPlaylists() throw (std::exception);
    static void updateMusicaTag(int musicaId, QRgb tag) throw (std::exception);
};

#endif // CONTROLLER_H
