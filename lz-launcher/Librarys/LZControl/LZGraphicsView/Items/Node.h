#ifndef NODE_H
#define NODE_H

#include "Item.h"

class Node : public Item
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = nullptr);

    virtual void createPoint() override;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event) override;
signals:

};

#endif // NODE_H
