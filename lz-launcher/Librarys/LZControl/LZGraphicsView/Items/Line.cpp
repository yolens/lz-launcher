#include "Line.h"
#include <QPainter>

Line::Line(QGraphicsItem *parent)
    : Item(parent)
{

}

void Line::setSource(const Item *item)
{
    m_pSource = item;
}
void Line::setDest(const Item *item)
{
    m_pDest = item;
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->fillRect(boundingRect(), Qt::blue);
    Item::paint(painter, option, widget);
}
