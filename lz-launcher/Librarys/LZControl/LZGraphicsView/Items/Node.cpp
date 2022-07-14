#include "Node.h"
#include <QPainter>
#include "LZLib.h"

Node::Node(QObject *parent)
    : Item(parent, LCType::LC_Node)
{
    m_typeName = "Node";
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
    if (nullptr != m_pChart)
    {
        int edge = POINT_SIZE + POINT_EDGE + 3;
        QRectF textRect = this->boundingRect().adjusted(edge, edge+10, -edge, -edge-10);

        QString dtext = QString("名称：%1").arg(m_pChart->m_name);
        dtext += "\r\n";
        dtext += QString("输入值：%1").arg(m_pChart->m_value.toString());
        dtext += "\r\n";
        dtext += QString("延时：%1ms").arg(m_pChart->m_delay);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, dtext);
    }
    painter->drawText(this->boundingRect().x()+5, this->boundingRect().bottom()-5, QString::number(m_testingTimes));
}


#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
void Node::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(Mime_Node_Chart) || event->mimeData()->hasFormat(Mime_Node_Point))
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
#include "../../Drag/PointDrag.h"
void Node::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(Mime_Node_Chart))
    {
        QVariant varData = event->mimeData()->data(Mime_Node_Chart);
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
            m_pChart->m_name = com->name();
            m_pChart->m_orderId = com->id;
            m_pChart->m_orderType = com->type();
            if (m_pChart->m_value.isNull())
                m_pChart->m_value = com->value();

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
    else if (event->mimeData()->hasFormat(Mime_Node_Point))
    {
        QVariant varData = event->mimeData()->data(Mime_Node_Point);
        QByteArray byteData = varData.toByteArray();
        QDataStream stream(&byteData, QIODevice::ReadWrite);
        qint64 node;
        stream >> (node);
        PointDrag* drag = (PointDrag*)(node);

        bool isExist = false;
        if (nullptr != drag)
        {
            if (drag->m_type == LPType::value)
            {
                foreach (const auto &p, m_pointVec)
                {
                    if ((p->type == drag->m_type) && (p->attribute == drag->m_attribute))
                    {
                        isExist  = true;
                        break;
                    }
                }
            }

            if (!isExist)
            {
                LPoint *p = new LPoint();
                p->type = drag->m_type;
                p->attribute = drag->m_attribute;
                p->chartId = m_pChart->id;
                insertPoint(p);
                updatePoint();
                update();
            }
        }

    }
    QGraphicsItem::dropEvent(event);
}

