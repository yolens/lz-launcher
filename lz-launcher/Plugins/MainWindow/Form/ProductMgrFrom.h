#ifndef PRODUCTMGRFROM_H
#define PRODUCTMGRFROM_H

#include <QWidget>
#include "LZGraphicsView/LZWindow.h"
#include "Card/NodeCard.h"
#include "Card/AddCard.h"
#include <QButtonGroup>

namespace Ui {
class ProductMgrFrom;
}

class ProductMgrFrom : public QWidget
{
    Q_OBJECT

public:
    explicit ProductMgrFrom(QWidget *parent = nullptr);
    ~ProductMgrFrom();

    void init();
private:
    void adjustProductView();
    void adjustUnitView();
    void addProductCard(LProduct* p);
    void addUnitCard(LUnit* p);


private:
    Ui::ProductMgrFrom *ui;

    LZWindow *m_pWindow = nullptr;

    QButtonGroup* m_productGroup = nullptr;
    QButtonGroup* m_unitGroup = nullptr;
    Card *m_addProductCard = nullptr;
    Card *m_addUnitCard = nullptr;
};

#endif // PRODUCTMGRFROM_H
