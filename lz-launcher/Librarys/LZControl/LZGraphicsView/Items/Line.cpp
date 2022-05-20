#include "Line.h"
#include <QPainter>
#include <QtMath>
#include "LZLib.h"
#include <QDebug>
Line::Line(QObject *parent)
    : Item(parent, LCType::LC_Line)
{
    //m_type = LCType::LC_Line;
    setFlag(ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

Line::~Line()
{
    if (nullptr == m_pChart)
        return;
    LPoint *p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_sourcePointId);
    if (p)
        p->count--;
    p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_destPointId);
    if (p)
        p->count--;
    emit removeLine();
}

void Line::setSource(Item *item)
{
    m_pSource = item;
    if (nullptr == item)
        return;
    m_pChart->m_sourcePointId = item->getCurrentPointId();
    LPoint *p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_sourcePointId);
    if (p)
        p->count++;
    item->addLine(this);
    onAdjust();
    updateChart();
}
void Line::setDest(Item *item)
{
    m_pDest = item;
    if (nullptr == item)
        return;
    m_pChart->m_destPointId = item->getCurrentPointId();
    LPoint *p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_destPointId);
    if (p)
        p->count++;
    item->addLine(this);
    onAdjust();
    updateChart();
}

Item* Line::getSource()
{
    return m_pSource;
}
Item* Line::getDest()
{
    return m_pDest;
}

void Line::onAdjust()
{
    adjustLine();
    update();
}

void Line::adjustLine()
{
    if (!m_pSource || !m_pDest)
        return;

    QLineF line(mapFromItem(m_pSource, 0, 0), mapFromItem(m_pDest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QRect rect;
        LPoint *p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_sourcePointId);
        if (nullptr != p)
            rect = p->rect;
        else
            rect = QRect(0,0,0,0);
        m_sourcePoint = line.p1() + QPointF(rect.left()+rect.width()/2, rect.top()+rect.height()/2);

        p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_destPointId);
        if (nullptr != p)
            rect = p->rect;
        else
            rect = QRect(0,0,0,0);
        m_destPoint = line.p2() + QPointF(rect.left()+rect.width()/2, rect.top()+rect.height()/2);
    } else {
        m_sourcePoint = m_destPoint = line.p1();
    }

}

QRectF Line::boundingRect() const
{
    if (!m_pSource || !m_pDest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + 6) / 2.0;

    return QRectF(m_sourcePoint, QSizeF(m_destPoint.x() - m_sourcePoint.x(),
                                      m_destPoint.y() - m_sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Line::startTest()
{
    m_testState = TestState::Testing;
    update();
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   // painter->fillRect(this->boundingRect(), QColor(255,0,0));

    QLineF line(m_sourcePoint, m_destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    QColor bgColor = Qt::white;
    switch (m_testState)
    {
    case TestState::Normal:
        break;
    case TestState::Testing:
        bgColor = Qt::darkYellow;
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
    // Draw the line itself
    painter->setPen(QPen(bgColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);


    // Draw the arrows
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF destArrowP1 = m_destPoint + QPointF(sin(angle - M_PI / 3) * 6,
                                              cos(angle - M_PI / 3) * 6);
    QPointF destArrowP2 = m_destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * 6,
                                              cos(angle - M_PI + M_PI / 3) * 6);

    painter->setBrush(bgColor);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
/*
void Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mousePressEvent(event);
}*/
