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
    painter->drawText(this->boundingRect(), Qt::AlignCenter, m_name);
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
            m_name = com->name();
            update();
        }

    }
    QGraphicsItem::dropEvent(event);
}

