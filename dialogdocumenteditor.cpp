#include "dialogdocumenteditor.h"
#include "ui_dialogdocumenteditor.h"
#include <QTextCursor>
#include <QTextCharFormat>
#include <QColorDialog>

DialogDocumentEditor::DialogDocumentEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDocumentEditor)
{
    ui->setupUi(this);
}

DialogDocumentEditor::~DialogDocumentEditor()
{
    delete ui;
}

void DialogDocumentEditor::setHtml(QString str){
    ui->textEdit->setHtml(str);
}

QString DialogDocumentEditor::getHtml(){
    return ui->textEdit->toHtml();
}

/*
 * QColor cor = QColorDialog::getColor(ui->textEdit->textColor(), this);
        if (!cor.isValid())
            return;

    QTextCharFormat fmt;
    QFont font;
    font.setBold(true);
    fmt.setFont(font);
    fmt.setForeground(cor);
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(fmt);
    */
