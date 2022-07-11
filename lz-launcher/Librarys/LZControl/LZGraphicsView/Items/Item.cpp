#include "Item.h"
#include <QPainter>
#include <QDebug>
#include "LZLib.h"
#include <QTimer>
#include "../ChartDialog.h"
#include <QTimer>
#include <QThread>
#include <QEventLoop>
#include <QCoreApplication>
#include <QGraphicsScene>

Item::Item(QObject *parent, LCType type)
    : QObject(parent)
{
    m_type = type;
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setAcceptHoverEvents(true);

    m_rect = QRectF(0, 0, 200, 160);

}

Item::~Item()
{ 

}

void Item::clear()
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

void Item::updateData()
{
    Plugin::DataCenterPlugin()->updateChart(m_pChart);
    foreach (const auto& p, m_pointVec)
    {
        Plugin::DataCenterPlugin()->updatePoint(p);
    }
    updatePoint();
    update();
}

LCType Item::getItemType()
{
    return m_type;
}

QString& Item::getTypeName()
{
    return m_typeName;
}

void Item::setChart(LChart* p)
{
    m_pChart = p;
    if (nullptr != p)
    {
        this->setPos(QPointF(p->m_pos));
        setSize(QSizeF(p->m_width, p->m_height));

        m_pOrder = Plugin::DataCenterPlugin()->getOrder(p->m_orderId, p->m_orderType);
    }
}

void Item::setSize(const QSizeF& size)
{
    m_rect.setSize(QSizeF(qMax(100.0, size.width()), qMax(80.0, size.height())));
    m_pChart->m_width = m_rect.width();
    m_pChart->m_height = m_rect.height();
    updateChart();

}

void Item::initTest()
{
    m_testState = TestState::Normal;
    m_testingTimes = 0;
    QTimer::singleShot(0, this, [=]{
        update();
    });
}

bool Item::startTest()
{
    m_testState = TestState::Testing;
    QTimer::singleShot(0, this, [=]{
        update();
    });

    int times = 0;
    while (true)
    {
        int sec = qMin(1000, m_pChart->m_delay-times);
        if (sec <= 0)
            break;
        QThread::msleep(sec);
        m_testingTimes++;
        m_bDrawRect = (m_testingTimes%2 != 0);
        QTimer::singleShot(0, this, [=]{
            update();
        });
        QCoreApplication::processEvents();
        times += sec;
    }

    if (nullptr != m_pOrder)
    {

        QEventLoop loop;
        connect(this, &Item::finished, &loop, &QEventLoop::quit);
        LOrder *item = Plugin::DataCenterPlugin()->newOrder(m_pOrder);

        QVariant sendValue;
        foreach (const auto& p, m_pointVec)
        {
            if (p->type == LPType::value && p->attribute == LPAttribute::input)
            {
                sendValue = p->outValue;
                break;
            }
        }
        if (sendValue.isNull())
        {
            sendValue = m_pChart->m_value;
        }

        std::function<void(const QVariant value)> valueCallback = [=](const QVariant value)
        {
            foreach (const auto& p, m_pointVec)
            {
                if (p->type == LPType::value && p->attribute == LPAttribute::output)
                {
                    p->outValue = value;
                    p->outByteType = item->byteType();
                    break;
                }
            }
            emit finished();
        };


        item->setValueCallback(valueCallback);

        item->setValue(sendValue);
        Plugin::DataCenterPlugin()->execute(item);
        loop.exec();
    }

    m_testState = TestState::Ok;

    m_bDrawRect = false;
    QTimer::singleShot(0, this, [=]{
        update();
    });
    return true;
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
    foreach (const auto& i, m_pointVec)
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
        foreach(auto&& p, lv)
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
        foreach(auto&& p, rv)
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
        foreach(auto&& p, bv)
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
        foreach(auto&& p, tv)
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


    foreach(const auto& i, m_pointVec)
    {
        switch (i->type)
        {
        case LPType::circuit:
        {
            if (i->attribute == LPAttribute::input)
                painter->setBrush(Chart_Circuit_Input_Color);
            else
                painter->setBrush(Chart_Circuit_Output_Color);

        }
            break;
        case LPType::value:
        {
            if (i->attribute == LPAttribute::input)
                painter->setBrush(Chart_Value_Input_Color);
            else
                painter->setBrush(Chart_Value_Output_Color);


            painter->drawText(i->rect.x(), i->rect.y() + ((i->attribute == LPAttribute::input) ? 22 : -8), i->outValue.toString());
        }
            break;
        default:
            break;
        }

        painter->setPen(Qt::NoPen);
        painter->drawEllipse(i->rect);
    }

    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF textRect = QRectF(0, 0, this->boundingRect().width(), 20);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, m_typeName);


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
#include <QGraphicsScene>
void Item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_mouseState == MouseState::Press)
    {
        const QPointF point = (event->pos() - m_startPos);
        m_startPos = event->pos();

        switch (m_dragType)
        {
        case DragType::Right:
        {
            m_rect.adjust(0, 0, point.x(), 0);
            setSize(m_rect.size());
        }
            break;
        case DragType::Bottom:
        {
            //m_rect.adjust(0, 0, -point.x(), 0);
            //this->setPos(this->pos().x()+point.x(), this->pos().y());
            m_rect.adjust(0, 0, 0, point.y());
            setSize(m_rect.size());
        }
            break;
        default:
            break;
        }

        scene()->update();
        updatePoint();
        update();
        emit adjust();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

LPoint* Item::getPointInPos(const QPoint& pt)
{
    foreach(const auto& i, m_pointVec)
    {
        if (i->rect.contains(pt))
        {
            return i;
        }
    }
    return nullptr;
}

#include <QMenu>
void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        m_startPos = event->pos();
        m_mouseState = MouseState::Press;

        m_currentPointId = 0;
        LPoint* lp = getPointInPos(event->pos().toPoint());
        if (nullptr != lp)
        {
            m_action = ActionType::ActionLine;
            m_currentPointId = lp->id;
            qInfo() << "QQQQQQ= " << lp->count << lp->max;
            if (lp->max == 0 || lp->count < lp->max)
                emit action(m_action);
            break;
        }

    }
        break;
    case Qt::RightButton:
    {
        LPoint* p = getPointInPos(event->pos().toPoint());
        if (nullptr != p)
        {
            mouseRightClick(p);
        }
        else
        {
            /*QString tip = "删除";
            if (m_type == LCType::LC_Line)
                tip = "删除连线";
            QMenu menu;
            QAction *deleteActon = new QAction(tip, &menu);
            menu.addAction(deleteActon);
            connect(deleteActon, &QAction::triggered, this, [=]{
                emit remove();
            });
            menu.exec(QCursor::pos());*/
            emit remove();
        }
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
    if (m_type != LCType::LC_Line)
    {
        QRectF left(this->boundingRect().left(), this->boundingRect().top(), DRAG_WIDTH, this->boundingRect().height());
        QRectF top(this->boundingRect().left(), this->boundingRect().top(), this->boundingRect().width(), DRAG_WIDTH);
        QRectF right(this->boundingRect().width()-DRAG_WIDTH, this->boundingRect().top(), DRAG_WIDTH, this->boundingRect().height());
        QRectF bottom(this->boundingRect().left(), this->boundingRect().height()-DRAG_WIDTH, this->boundingRect().width(), DRAG_WIDTH);
        setFlag(ItemIsMovable, false);
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
            setFlag(ItemIsMovable);
            m_dragType = DragType::Center;
            setCursor(Qt::ArrowCursor);
        }
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
    ChartDlg dlg;
    dlg.setItem(this);
    switch (m_type)
    {
    case LCType::LC_Line:
        break;
    default:
        dlg.exec();
        break;
    }

    QGraphicsItem::mouseDoubleClickEvent(event);
}
