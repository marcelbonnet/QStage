#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <QMap>
#include <QRgb>
#include <QList>

#include "Waveform.h"

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

    /**
     * @brief query wave forms : uma wave form é a WAVE de cada TONE de um PATCH
     * @return
     */
    static QList<Waveform*> queryWaveforms() throw (std::exception);

    /**
     * @brief query tabela temporária em que salvei os Patches Group type, id e patch number
     * para usar em SYSEX de RequestData para fazer DUMP do Teclado
     * @return
     */
    static QList<QList<int>>* queryDefaultPatches() noexcept(false);
};

#endif // CONTROLLER_H
