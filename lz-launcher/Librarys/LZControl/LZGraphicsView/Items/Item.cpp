#include "Item.h"
#include <QPainter>
#include <QDebug>
#include "LZLib.h"
#include <QTimer>
#include "../ChartDialog.h"


Item::Item(QObject *parent, LCType type)
    : QObject(parent)
{
    Q_UNUSED(type);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setAcceptHoverEvents(true);

    m_rect = QRectF(0, 0, 200, 160);

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
    if (nullptr != p)
    {
        m_pOrder = Plugin::DataCenterPlugin()->getOrder(p->m_orderId, p->m_orderType);
    }
}

void Item::initTest()
{
    m_testState = TestState::Normal;
    m_testingTimes = 0;
    update();
}


#include <QTimer>
#include <QThread>
#include <QEventLoop>
#include <QCoreApplication>
void Item::startTest()
{
    m_testingTimer = new QTimer();
    m_testingTimer->setInterval(1000);
    m_testingTimes = 0;
    connect(m_testingTimer, &QTimer::timeout, this, [=]{
        m_testingTimes++;
        m_bDrawRect = (m_testingTimes%2 != 0);
        update();
    });
    m_testingTimer->start();

    m_testState = TestState::Testing;
    update();


    if (nullptr != m_pOrder)
    {
        int times = 0;
        while (true)
        {
            int sec = qMin(1000, m_pChart->m_delay-times);
            QThread::msleep(sec);
            QCoreApplication::processEvents();
            times += sec;
            if (times >= m_pChart->m_delay-times)
                break;
        }

        QEventLoop loop;
        connect(this, &Item::finished, &loop, &QEventLoop::quit);
        LOrder *item = Plugin::DataCenterPlugin()->newOrder(m_pOrder);
        std::function<void(const QVariant value)> valueCallback = [=](const QVariant value)
        {
            m_inputValueList[item->valueId()] = item->value();
            emit finished();
        };


        item->setValueCallback(valueCallback);
        QVariant sendValue;
        if (m_inputValueList.contains(item->valueId()))
            sendValue = m_inputValueList.value(item->valueId());
        else
            sendValue = m_pChart->m_value;
        item->setValue(sendValue);
        Plugin::DataCenterPlugin()->execute(item);
        loop.exec();


    }

    m_testState = TestState::Ok;
    m_testingTimer->stop();
    m_bDrawRect = false;
    update();

    emit testing();
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

LOrder* Item::getOrder()
{
    return m_pOrder;
}

QVector<LPoint*>& Item::getPointList()
{
    return m_pointVec;
}

void Item::setInputValue(const QString& id, const QVariant& value)
{
    m_inputValueList[id] = value;
}

QVariant Item::getInputValue(const QString& id)
{
    return m_inputValueList.value(id);
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
                    && i->attribute == LPAttribute::output)
        {
            bv.append(i);
        }
        else if (i->type == LPType::value
                    && i->attribute == LPAttribute::input)
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
    //path.addEllipse(boundingRect());
    path.addRect(boundingRect());
    return path;
}
void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QColor bgColor = QColor(26, 159, 255);
    QColor penColor = Qt::white;

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
        penColor = Qt::red;
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

    painter->setPen(QPen(penColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->fillRect(boundingRect(), bgColor);
    if (m_bDrawRect || bHover)
        painter->drawRect(this->boundingRect());

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    int edge = POINT_SIZE + POINT_EDGE + 1;
    QRectF inRect = this->boundingRect().adjusted(edge, edge, -edge, -edge);
    painter->drawRect(inRect);

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

void Item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    const QPointF point = (event->pos() - m_startPos);
    qDebug() << "HHHHHHHHH= " << point;
    QGraphicsItem::mouseMoveEvent(event);
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        m_startPos = event->pos();
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
    QRectF left(this->boundingRect().left(), this->boundingRect().top(), DRAG_WIDTH, this->boundingRect().height());
    QRectF top(this->boundingRect().left(), this->boundingRect().top(), this->boundingRect().width(), DRAG_WIDTH);
    QRectF right(this->boundingRect().width()-DRAG_WIDTH, this->boundingRect().top(), DRAG_WIDTH, this->boundingRect().height());
    QRectF bottom(this->boundingRect().left(), this->boundingRect().height()-DRAG_WIDTH, this->boundingRect().width(), DRAG_WIDTH);
    if (left.contains(event->pos()))
    {
        m_dragType = DragType::Left;
        setCursor(Qt::SizeHorCursor);
    }
    else if (right.contains(event->pos()))
    {
        m_dragType = DragType::Right;
        setCursor(Qt::SizeHorCursor);
    }
    else if (top.contains(event->pos()))
    {
        m_dragType = DragType::Top;
        setCursor(Qt::SizeVerCursor);
    }
    else if (bottom.contains(event->pos()))
    {
        m_dragType = DragType::Bottom;
        setCursor(Qt::SizeVerCursor);
    }
    else
    {
        m_dragType = DragType::Center;
        setCursor(Qt::ArrowCursor);
    }
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

void Item::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (nullptr == m_pChart || nullptr == m_pOrder)
        return;
    ChartDialog dlg;
    ChartDialog::ChartInfo info;
    info.value = m_pChart->m_value.isNull() ? m_pOrder->value() : m_pChart->m_value;
    info.delay = m_pChart->m_delay;
    dlg.setInfo(info);
    dlg.exec();
    m_pChart->m_value = dlg.info().value;
    m_pChart->m_delay = dlg.info().delay;
    updateChart();
    QGraphicsItem::mouseDoubleClickEvent(event);
}
