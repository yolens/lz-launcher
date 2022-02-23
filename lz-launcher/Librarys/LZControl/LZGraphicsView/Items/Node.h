#ifndef NODE_H
#define NODE_H

#include "Item.h"

class Node : public Item
{
    //Q_OBJECT
public:
    explicit Node(QGraphicsItem *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:

};

#endif // NODE_H
