#include "Virtual.h"
#include <QPainter>

Virtual::Virtual(QObject *parent)
    : Item(parent, LCType::LC_Virtual)
{
    //m_type = LCType::LC_Virtual;
}

QRectF Virtual::boundingRect() const
{
    return QRectF(0, 0, 0, 0);
}

QPainterPath Virtual::shape() const
{
    QPainterPath path;
    path.addEllipse(0, 0, 0, 0);
    return path;
}

void Virtual::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(boundingRect(), Qt::green);
}
