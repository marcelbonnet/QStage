#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <QMap>
#include <QRgb>
#include <QList>

#include "Waveform.h"
#include "patch.h"

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
      @deprecated
     * @brief query tabela temporária em que salvei os Patches Group type, id e patch number
     * para usar em SYSEX de RequestData para fazer DUMP do Teclado
     * @return
     */
    static QList<QList<int>>* queryDefaultPatches() noexcept(false);

    static QList<QString>* getPatch(int id) noexcept(false);
    /**
     * @brief retorna todos os patches (da Roland e os criados no QStage)
     * @return
     */
    static QList<Patch*>* queryPatches() noexcept(false);
    /**
     * @brief retorna os patches da roland (roland=true) ou somente os feitos no QStage (roland=false)
     * @param roland true|false
     * @return
     */
    static QList<Patch*>* queryPatches(bool roland) noexcept(false);

    static void insertPatch(int groupType, int groupId, int number, QString name, QString common, QString tone0
                           , QString tone1 , QString tone2, QString tone3) noexcept(false);


};

#endif // CONTROLLER_H
