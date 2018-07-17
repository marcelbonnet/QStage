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

private:
    Ui::DialogDocumentEditor *ui;
};

#endif // DIALOGDOCUMENTEDITOR_H
