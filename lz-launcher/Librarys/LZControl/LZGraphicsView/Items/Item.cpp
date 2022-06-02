#include "Item.h"
#include <QPainter>
#include <QDebug>
#include "LZLib.h"
#include <QTimer>

Item::Item(QObject *parent, LCType type)
    : QObject(parent)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setAcceptHoverEvents(true);

    m_rect = QRectF(0, 0, 100, 80);

    m_testingTimer = new QTimer(this);
    m_testingTimer->setInterval(1000);
    connect(m_testingTimer, &QTimer::timeout, this, &Item::onTestingTimer);

}

Item::~Item()
{ 
    if (nullptr == m_pChart)
        return;
    //移除所有的连线
    foreach (Item* item, m_lineList)
    {
        Plugin::DataCenterPlugin()->removeChart(item->getChart());
        emit item->remove();
    }

    //移除所有的点
    foreach (LPoint *p, m_pointVec)
    {
        Plugin::DataCenterPlugin()->removePoint(p);
    }
    //移除自己
    Plugin::DataCenterPlugin()->removeChart(m_pChart);
}


void Item::setChart(LChart* p)
{
    m_pChart = p;

}

void Item::initTest()
{
    m_testState = TestState::Normal;
    m_testingTimes = 0;
    m_testingTimer->stop();
    update();
}

#include <QTimer>
void Item::startTest()
{
    m_testingTimer->start();
    m_testState = TestState::Testing;
    update();

    QTimer::singleShot(6000, this, [=]()
    {
        m_testState = TestState::Ok;
        m_testingTimer->stop();
        m_bDrawRect = false;
        update();

        foreach (LPoint *p, m_pointVec)
        {
            if (p->attribute == LPAttribute::output)
            {
                emit testing(p->id);
                break;
            }
        }
    });

}

void Item::initData()
{
    if (nullptr == m_pChart)
        return;
    m_pointVec.clear();

    QList<LPoint*> list = Plugin::DataCenterPlugin()->getPointList(m_pChart->id);
    foreach (LPoint* p, list)
    {
        m_pointVec.push_back(p);
    }
}

LChart* Item::getChart()
{
    return m_pChart;
}

void Item::addLine(Item *item)
{
    connect(item, &Item::removeLine, this, &Item::onRemoveLine);
    m_lineList << item;
    updateChart();
}

void Item::onRemoveLine()
{
    Item *line = qobject_cast<Item*>(sender());
    if (nullptr == line)
        return;
    m_lineList.removeAll(line);
}

void Item::onTestingTimer()
{
    m_testingTimes++;
    m_bDrawRect = (m_testingTimes%2 != 0);
    update();
}

void Item::updateChart()
{
    Plugin::DataCenterPlugin()->updateChart(m_pChart);
}



void Item::insertPoint(LPoint* p)
{
    Plugin::DataCenterPlugin()->insertPoint(p);
    m_pointVec.push_back(p);
}

Item::ActionType Item::actionType()
{
    return m_action;
}

int Item::getCurrentPointId()
{
    return m_currentPointId;
}

void Item::setCurrentPointId(const int id)
{
    m_currentPointId = id;
}

void Item::updatePoint()
{
    QVector<LPoint*> lv, rv, bv, tv;
    foreach (auto i, m_pointVec)
    {
        if (i->type == LPType::circuit
                && i->attribute == LPAttribute::input)
        {
            lv.append(i);
        }
        else if (i->type == LPType::circuit
                    && i->attribute == LPAttribute::output)
        {
            rv.append(i);
        }
        else if (i->type == LPType::value
                    && i->attribute == LPAttribute::input)
        {
            bv.append(i);
        }
        else if (i->type == LPType::value
                    && i->attribute == LPAttribute::output)
        {
            tv.append(i);
        }
    }


    if (!lv.isEmpty())
    {
        int spacing = m_rect.height()/(lv.size()+1);
        int y = 0;
        int x = POINT_EDGE;
        foreach(LPoint* p, lv)
        {
            y += spacing;
            p->rect = QRect(QPoint(x,y-POINT_SIZE/2), QSize(POINT_SIZE,POINT_SIZE));
        }
    }
    if (!rv.isEmpty())
    {
        int spacing = m_rect.height()/(rv.size()+1);
        int y = 0;
        int x = m_rect.width()-POINT_SIZE-POINT_EDGE;
        foreach(LPoint* p, rv)
        {
            y += spacing;
            p->rect = QRect(QPoint(x,y-POINT_SIZE/2), QSize(POINT_SIZE,POINT_SIZE));
        }
    }
    if (!bv.isEmpty())
    {
        int spacing = m_rect.width()/(bv.size()+1);
        int y = m_rect.height()-POINT_SIZE-POINT_EDGE;
        int x = 0;
        foreach(LPoint* p, bv)
        {
            x += spacing;
            p->rect = QRect(QPoint(x-POINT_SIZE/2,y), QSize(POINT_SIZE,POINT_SIZE));
        }
    }
    if (!tv.isEmpty())
    {
        int spacing = m_rect.width()/(tv.size()+1);
        int y = POINT_EDGE;
        int x = 0;
        foreach(LPoint* p, tv)
        {
            x += spacing;
            p->rect = QRect(QPoint(x-POINT_SIZE/2,y), QSize(POINT_SIZE,POINT_SIZE));
        }
    }
}

QRectF Item::boundingRect() const
{
    return m_rect;
}
QPainterPath Item::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    //path.addRect(boundingRect());
    return path;
}
void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QColor bgColor = QColor(26, 159, 255);

    bool bHover = false;
    switch (m_mouseState)
    {
    case MouseState::Leave:
        bHover = false;
        break;
    case MouseState::Hover:
        bHover = true;
        break;
    case MouseState::Press:
        bHover = true;
        break;
    default:
        break;
    }

    switch (m_testState)
    {
    case TestState::Normal:

        break;
    case TestState::Testing:
        bgColor = Qt::yellow;
        break;
    case TestState::Ok:
        bgColor = Qt::green;
        break;
    case TestState::Fail:
        bgColor = Qt::red;
        break;
    default:
        break;
    }

    painter->setPen(QPen(Qt::white, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->fillRect(boundingRect(), bgColor);
    if (m_bDrawRect || bHover)
        painter->drawRect(this->boundingRect());

    painter->setPen(Qt::NoPen);
    foreach(auto i, m_pointVec)
    {
        if (i->attribute == LPAttribute::input)
            painter->setBrush(QColor(0,0,0));
        else
            painter->setBrush(QColor(255,0,0));
        painter->drawEllipse(i->rect);
    }

}

QVariant Item::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        emit adjust();
        break;
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}

void Item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouseState = MouseState::Hover;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
#include <QGraphicsSceneMouseEvent>

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        m_mouseState = MouseState::Press;

        m_currentPointId = 0;
        foreach(auto i, m_pointVec)
        {
            if (i->rect.contains(event->pos().toPoint()))
            {
                m_action = ActionType::ActionLine;
                m_currentPointId = i->id;
                if (i->max == 0 || i->count < i->max)
                    emit action(m_action);
                break;
            }
        }

    }
        break;
    case Qt::RightButton:
    {
          emit remove();
    }
        break;
    default:
        break;
    }


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

