#include "Branch.h"
#include "LZLib.h"

Branch::Branch(QObject *parent)
    : Item{parent, LCType::LC_Branch}
{
    m_typeName = "Branch";
    this->setAcceptDrops(true);
}

bool Branch::startTest()
{
    return true;
}

Item::FunctionType Branch::setInputValue(const QString& id, const QVariant& value)
{
    m_inputValueList[id] = value;
    return FunctionType::ValueTrigger_func;
}

const LPoint* Branch::getNextPoint(const QVariant& value)
{
    LPoint *p = nullptr;

    return p;
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

    p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::output;
    p->chartId = m_pChart->id;
    insertPoint(p);
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
