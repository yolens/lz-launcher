#include "Node.h"
#include <QPainter>
#include "LZLib.h"

Node::Node(QObject *parent)
    : Item(parent, LCType::LC_Node)
{
    //m_type = LCType::LC_Node;
    this->setAcceptDrops(true);
}


void Node::createPoint()
{
    LPoint *p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::input;
    p->chartId = m_pChart->id;
    insertPoint(p);
    p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::output;
    p->chartId = m_pChart->id;
    insertPoint(p);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Item::paint(painter, option, widget);
    painter->setPen(QPen(Qt::black));
    if (nullptr != m_pOrder)
    {
        QRectF textRect = this->boundingRect();
        textRect.adjust(20.0, 0.0, -20.0, 0.0);
        painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, m_pOrder->name());
    }
    painter->drawText(this->boundingRect().x()+5, this->boundingRect().bottom()-5, QString::number(m_testingTimes));
}


#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
void Node::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("Node/OrderTree"))
    {
        event->setAccepted(true);
    }
    else
    {
        event->setAccepted(false);
    }
    QGraphicsItem::dragEnterEvent(event);
}
void Node::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
    QGraphicsItem::dragMoveEvent(event);
}
void Node::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsItem::dragLeaveEvent(event);
}


void Node::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("Node/OrderTree"))
    {
        QVariant varData = event->mimeData()->data("Node/OrderTree");
        QByteArray byteData = varData.toByteArray();
        QDataStream stream(&byteData, QIODevice::ReadWrite);
        qint64 node;
        stream >> (node);
        LOrder* com = (LOrder*)(node);
        if (com != nullptr)
        {
            m_pOrder = com;
            m_pChart->m_orderId = com->id;
            m_pChart->m_orderType = com->type();
            updateChart();
            update();
        }

    }
    QGraphicsItem::dropEvent(event);
}

