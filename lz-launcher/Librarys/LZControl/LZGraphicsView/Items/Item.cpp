#include "Item.h"
#include <QPainter>

Item::Item(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setAcceptHoverEvents(true);
}

Item::~Item()
{

}

QRectF Item::boundingRect() const
{
    return QRectF(0, 0, 60, 60);
}
QPainterPath Item::shape() const
{
    QPainterPath path;
    //path.addEllipse(boundingRect());
    path.addRect(boundingRect());
    return path;
}
void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QColor bgColor = Qt::red;
    switch (m_mouseState)
    {
    case MouseState::Leave:
        bgColor = Qt::darkCyan;
        break;
    case MouseState::Hover:
        bgColor = Qt::darkBlue;
        break;
    case MouseState::Press:
        bgColor = Qt::darkYellow;
        break;
    default:
        break;
    }

    painter->fillRect(boundingRect(), bgColor);
}

void Item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouseState = MouseState::Hover;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouseState = MouseState::Press;
    update();
    QGraphicsItem::mousePressEvent(event);
}
void Item::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverMoveEvent(event);
}
void Item::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_mouseState = MouseState::Hover;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}
void Item::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_mouseState = MouseState::Leave;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}
