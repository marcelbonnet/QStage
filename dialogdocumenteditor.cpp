#include "dialogdocumenteditor.h"
#include "ui_dialogdocumenteditor.h"
#include <QTextCursor>
#include <QTextCharFormat>
#include <QColorDialog>
#include <QDebug>

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

void DialogDocumentEditor::on_btnBold_clicked()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    QTextCharFormat fmt;
    fmt.setFontWeight( cursor.charFormat().fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold );
    cursor.mergeCharFormat(fmt);

}

void DialogDocumentEditor::on_btnItalic_clicked()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    QTextCharFormat fmt;
    fmt.setFontItalic(!cursor.charFormat().fontItalic());

    cursor.mergeCharFormat(fmt);
}

void DialogDocumentEditor::on_btnUnderline_clicked()
{

    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    QTextCharFormat fmt = cursor.charFormat();
    fmt.setFontUnderline(!cursor.charFormat().fontUnderline());
    cursor.mergeCharFormat(fmt);
}


void DialogDocumentEditor::on_buttonBox_rejected()
{
    hide();
}

void DialogDocumentEditor::on_comboTag_currentIndexChanged(int index)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    QString texto = cursor.selectedText();

    QTextCharFormat fmt;
    //usar sempre ou usar uma seleção de propriedades conforme a TAG/Estilo aplicado
    fmt.setFontWeight(cursor.charFormat().fontWeight());
    fmt.setFontItalic(cursor.charFormat().fontItalic());
    fmt.setFontUnderline(cursor.charFormat().fontUnderline());
    fmt.setForeground(cursor.charFormat().foreground());
    fmt.setBackground(cursor.charFormat().background());

    cursor.removeSelectedText();
    QString novoHtml = "<"+ui->comboTag->currentText()+">"
            + texto
            + "</"+ui->comboTag->currentText()+">";

    int inicio = cursor.position();
    int final = novoHtml.length() - inicio;
    cursor.insertHtml(novoHtml);

    cursor.setPosition(inicio, QTextCursor::KeepAnchor);
    cursor.mergeCharFormat(fmt);

}

void DialogDocumentEditor::on_tabWidget_currentChanged(int index)
{
    if(index == 1){
        ui->textEditSource->setPlainText( getHtml() );
    }

    if(index == 0){
        setHtml(ui->textEditSource->toPlainText());
    }
}

void DialogDocumentEditor::on_btnCor_clicked()
{
    QColor cor = QColorDialog::getColor(ui->textEdit->textColor(), this);
    if (!cor.isValid())
        return;

    QTextCharFormat fmt;
    fmt.setForeground(cor);

    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(fmt);

}

void DialogDocumentEditor::on_btnCorFundo_clicked()
{
    QColor cor = QColorDialog::getColor(ui->textEdit->textBackgroundColor(), this);
    if (!cor.isValid())
        return;

    QTextCharFormat fmt;
    fmt.setBackground(cor);

    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(fmt);
}

void DialogDocumentEditor::on_buttonBox_accepted()
{
    emit edicaoHTMLTerminada(musicaId, getHtml());
    hide();
}
