#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class ChartDialog;
}

class ChartDialog : public QDialog
{
    Q_OBJECT
public:
    struct ChartInfo
    {
        QVariant value;
        int delay;
    };
public:
    explicit ChartDialog(QWidget *parent = nullptr);
    ~ChartDialog();

    void setInfo(const ChartInfo& info);
    ChartInfo& info();
private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::ChartDialog *ui;

    ChartInfo m_info;
};

#endif // CHARTDIALOG_H
