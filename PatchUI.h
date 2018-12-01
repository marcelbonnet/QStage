#ifndef PATCHUI_H
#define PATCHUI_H

#include <QWidget>

namespace Ui {
class PatchUI;
}

class PatchUI : public QWidget
{
    Q_OBJECT

public:
    explicit PatchUI(QWidget *parent = 0);
    ~PatchUI();

private:
    Ui::PatchUI *ui;
};

#endif // PATCHUI_H
