#ifndef FORMSERIALMIDI_H
#define FORMSERIALMIDI_H

#include <QWidget>
#include <QList>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>

namespace Ui {
class FormSerialMidi;
}

class FormSerialMidi : public QWidget
{
    Q_OBJECT

public:
    explicit FormSerialMidi(QWidget *parent = nullptr);
    ~FormSerialMidi();

    /**
      usará sustain ou não
     */
    bool ignorarNoteOff();
    /**
      todas as notas a serem ativadas para uma nota de dó (0) a si (11)
      Retorna uma lista de inteiros: cada posição é um valor a ser incrementado
      na nota MIDI:
      se foi ativado MIDI NOTA = 48 (dó C4)
      notaRaiz = 0
      se retornar {4, 7} > acionar NOTE ON {48, 48+4, 48+7} (acorde C)
     */
    QList<int>* getNotas(int notaRaiz);

    /**
     * seta se o botão estrá checado ou não
     */
    void setBtnIgnorarNoteOff(bool checked);
    /**
     * set o index da combo de programa pré determinado de notas
     * nota: de 0 a 11
     */
    void setPrograma(int nota, int index);
    /**
      set intervalos da nota
      nota: de 0 a 11
      **/
    void setIntervalos(int nota, QList<int> *notas);

private:
    Ui::FormSerialMidi *ui;
    QPushButton *btnIgnoreNoteOff ;
    QList<QComboBox*> *cmbProgramas;
    QList<QListWidget*> *cmbIntervalos;
};

#endif // FORMSERIALMIDI_H
