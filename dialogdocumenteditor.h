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
    QString getHtml();

private slots:
    void on_btnBold_clicked();

    void on_btnItalic_clicked();



    void on_btnUnderline_clicked();

    void on_buttonBox_rejected();

    void on_comboTag_currentIndexChanged(int index);

private:
    Ui::DialogDocumentEditor *ui;
};

#endif // DIALOGDOCUMENTEDITOR_H
