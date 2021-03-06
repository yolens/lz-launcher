#include "Virtual.h"
#include <QPainter>

Virtual::Virtual(QObject *parent)
    : Item(parent, LCType::LC_Virtual)
{
    m_typeName = "Virtual";
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
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->fillRect(boundingRect(), Qt::green);
}
