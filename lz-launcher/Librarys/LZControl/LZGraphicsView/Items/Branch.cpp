#include "Branch.h"
#include "LZLib.h"

Branch::Branch(QObject *parent)
    : Item{parent, LCType::LC_Branch}
{
    m_typeName = "Branch";
    this->setAcceptDrops(true);
}
#include <QTimer>
bool Branch::startTest()
{
    m_testState = TestState::Testing;

    QTimer::singleShot(0, this, [=]{
        update();
    });
    return true;
}
#include <QDebug>
Item::FunctionType Branch::witchFunction()
{
    return FunctionType::ValueTrigger_func;
}

const LPoint* Branch::startTest(const QVariant& value, const LOrder::ByteType type)
{
    m_testState = TestState::Testing;

    QTimer::singleShot(0, this, [=]{
        update();
    });

    QVariant u64 = LZLib::instance()->toLonglong(type, value);

    foreach(const auto& p, m_pointVec)
    {
        if (p->type == LPType::circuit && p->attribute == LPAttribute::output)
        {
            LOrder::ByteType bt = LOrder::DEC;
            if (p->minValue.toString().contains("."))
                bt = LOrder::Float;
            if (LOrder::DEC == bt)
            {
                qlonglong mid = LZLib::instance()->fromLonglong(LOrder::DEC, u64).toLongLong();
                //qInfo() << "DDDD dec= " << p->minValue << mid << p->maxValue;
                if (p->minValue.toLongLong() <= mid && p->maxValue.toLongLong() >= mid)
                {
                    return p;
                }
            }
            else
            {
                float mid = LZLib::instance()->fromLonglong(LOrder::Float, u64).toFloat();
                //qInfo() << "DDDD float= " << p->minValue << mid << p->maxValue;
                if (p->minValue.toFloat() <= mid && p->maxValue.toFloat() >= mid)
                {
                    return p;
                }
            }

            /*if (p->minValue.toLongLong() <= value.toLongLong() && p->maxValue.toLongLong() >= value.toLongLong())
            {
                qInfo() << "DDDD2= " << p->minValue.toLongLong() << value.toLongLong() << p->maxValue.toLongLong();
                return p;
            }*/
        }
    }
    return nullptr;
}

void Branch::createPoint()
{
    LPoint *p = new LPoint();
    p->type = LPType::value;
    p->attribute = LPAttribute::input;
    p->chartId = m_pChart->id;
    insertPoint(p);

    p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::output;
    p->chartId = m_pChart->id;
    insertPoint(p);
    p->name = QString("O%1").arg(p->id);
    Plugin::DataCenterPlugin()->updatePoint(p);

    p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::output;
    p->chartId = m_pChart->id;
    insertPoint(p);
    p->name = QString("O%1").arg(p->id);
    Plugin::DataCenterPlugin()->updatePoint(p);
}
#include <QMenu>
void Branch::mouseRightClick(const LPoint* p)
{
    if (nullptr == p)
        return;
    QMenu menu;
    QAction *deleteActon = new QAction("删除点", &menu);
    menu.addAction(deleteActon);
    connect(deleteActon, &QAction::triggered, this, [=]{
        Plugin::DataCenterPlugin()->removePoint((LPoint*)p);
        m_pointVec.removeAll((LPoint*)p);
        updatePoint();
        update();
    });
    menu.exec(QCursor::pos());
}


#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
void Branch::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(Mime_Node_Point))
    {
        event->setAccepted(true);
    }
    else
    {
        event->setAccepted(false);
    }
    QGraphicsItem::dragEnterEvent(event);
}
void Branch::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
    QGraphicsItem::dragMoveEvent(event);
}
void Branch::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsItem::dragLeaveEvent(event);
}

#include <QDebug>
#include <QGraphicsScene>
#include "Drag/PointDrag.h"
void Branch::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(Mime_Node_Point))
    {
        QVariant varData = event->mimeData()->data(Mime_Node_Point);
        QByteArray byteData = varData.toByteArray();
        QDataStream stream(&byteData, QIODevice::ReadWrite);
        qint64 node;
        stream >> (node);
        PointDrag* drag = (PointDrag*)(node);

        if (nullptr != drag)
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
    QGraphicsItem::dropEvent(event);
}
#include <QPainter>
void Branch::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Item::paint(painter, option, widget);

    /*foreach(const auto& i, m_pointVec)
    {
        if (i->type == LPType::circuit && i->attribute == LPAttribute::output)
        {
            painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            QRect pointRect(i->rect.x()-20, i->rect.top(), 20, i->rect.bottom());
            painter->drawText(pointRect, Qt::AlignHCenter | Qt::AlignRight, QString("P%1").arg(m_pointVec.indexOf(i)));
        }
    }*/

}
