#ifndef BRANCH_H
#define BRANCH_H

#include "Item.h"

class Branch : public Item
{
    Q_OBJECT
public:
    explicit Branch(QObject *parent = nullptr);

    virtual bool startTest() override;
    virtual const LPoint* startTest(const QVariant& value) override;
    virtual void createPoint() override;
    virtual Item::FunctionType witchFunction() override;
private:
    virtual void mouseRightClick(const LPoint* p) override;
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event) override;
signals:

};

#endif // BRANCH_H
