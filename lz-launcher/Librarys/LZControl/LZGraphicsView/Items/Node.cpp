#include "Node.h"
#include <QPainter>

Node::Node(QGraphicsItem *parent)
    : Item(parent)
{

}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->fillRect(boundingRect(), Qt::green);
    Item::paint(painter, option, widget);
}
