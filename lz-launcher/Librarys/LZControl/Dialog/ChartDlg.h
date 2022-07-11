#ifndef CHARTDLG_H
#define CHARTDLG_H

#include "BaseDlg.h"
#include <QVariant>
#include "LZLib.h"

namespace Ui {
class ChartDlg;
}

class Item;
class QGridLayout;
class QVBoxLayout;
class ChartDlg : public BaseDlg
{
    Q_OBJECT
public:
    explicit ChartDlg(QWidget *parent = nullptr);
    ~ChartDlg();

    void setItem(Item *p);
private:
    void updateView();
private:
    Ui::ChartDlg *ui;

    Item *m_item = nullptr;
    QVBoxLayout* m_mainLayout = nullptr;

    QVector<std::function<void()>> m_valueSetList;
};

#endif // CHARTDLG_H
