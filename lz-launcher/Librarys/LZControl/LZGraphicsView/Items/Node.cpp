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
        int edge = POINT_SIZE + POINT_EDGE + 3;
        QRectF textRect = this->boundingRect().adjusted(edge, edge, -edge, -edge);

        QString dtext = QString("名称：%1").arg(m_pOrder->name());
        dtext += "\r\n";
        dtext += QString("测试：%1").arg("dsf");
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, dtext);
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

#include <QDebug>
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
            foreach (LPoint *p, m_pointVec)
            {
                if (p->type == LPType::value)
                {
                    Plugin::DataCenterPlugin()->removePoint(p);
                    m_pointVec.removeAll(p);
                }
            }

            m_pOrder = com;
            m_pChart->m_orderId = com->id;
            m_pChart->m_orderType = com->type();

            LPoint *p = new LPoint();
            p->type = LPType::value;
            if (LOrder::Read == com->rwType())
                p->attribute = LPAttribute::output;
            else
                p->attribute = LPAttribute::input;
            p->valueId = com->valueId();
            p->chartId = m_pChart->id;
            insertPoint(p);
            updatePoint();

            updateChart();
            update();
        }

    }
    QGraphicsItem::dropEvent(event);
}

