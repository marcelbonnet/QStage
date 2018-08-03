#ifndef DIALOGDOCUMENTEDITOR_H
#define DIALOGDOCUMENTEDITOR_H

#include <QDialog>
#include <QString>

namespace Ui {
class DialogDocumentEditor;
}

class DialogDocumentEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDocumentEditor(QWidget *parent = 0);
    ~DialogDocumentEditor();
    void setHtml(QString str);
    /**
     * @brief getHtml Retorna Código Fonte do HTML do editor
     * @return QString
     */
    QString getHtml();
    int musicaId = 0;   //id da música que está sob edição

private slots:
    void on_btnBold_clicked();

    void on_btnItalic_clicked();

    void on_btnUnderline_clicked();

    void on_buttonBox_rejected();

    void on_comboTag_currentIndexChanged(int index);

    void on_tabWidget_currentChanged(int index);

    void on_btnCor_clicked();

    void on_btnCorFundo_clicked();

    void on_buttonBox_accepted();

private:
    Ui::DialogDocumentEditor *ui;

signals:
    /**
     * @brief edicaoHTMLTerminada ID da música e o fonte HTML
     */
    void edicaoHTMLTerminada(int, QString);
};

#endif // DIALOGDOCUMENTEDITOR_H
