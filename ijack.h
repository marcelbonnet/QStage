#include <QList>
#include <QString>

class IJack{
public:
    virtual ~IJack();
    virtual QList<QString> *listarPortas() = 0;
    virtual void conectarNaPorta(QString nomePorta) = 0;
};
