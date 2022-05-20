#ifndef COMSMANAGERDLG_H
#define COMSMANAGERDLG_H

#include <QDialog>

namespace Ui {
class ComsManagerDlg;
}

class ComsManagerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ComsManagerDlg(QWidget *parent = nullptr);
    ~ComsManagerDlg();

    void init();
private:
    Ui::ComsManagerDlg *ui;
};

#endif // COMSMANAGERDLG_H
