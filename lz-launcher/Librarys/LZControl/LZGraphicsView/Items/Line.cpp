#include "Line.h"
#include <QPainter>
#include <QtMath>
#include "LZLib.h"
#include <QDebug>
Line::Line(QObject *parent)
    : Item(parent, LCType::LC_Line)
{
    m_typeName = "Line";
    setFlag(ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

Line::~Line()
{

}

void Line::clear()
{
    Item::clear();
    if (nullptr == m_pChart)
        return;
    LPoint *p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_sourcePointId);
    if (p)
    {
        m_pSource->removeLineCount(p->id);
        memset(&p->transferData, 0, sizeof(LPoint::TransferData));
        qInfo() << "YY1----------------------" << getLineCount(p->id);//p->count;
    }
    p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_destPointId);
    if (p)
    {
        m_pDest->removeLineCount(p->id);
        memset(&p->transferData, 0, sizeof(LPoint::TransferData));
        qInfo() << "YY2----------------------" << getLineCount(p->id);//p->count;
    }

    emit removeLine();
}

void Line::setSource(Item *item, const bool adjust)
{
    m_pSource = item;
    if (nullptr == item)
        return;
    m_pChart->m_sourcePointId = item->getCurrentPointId();
    LPoint *p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_sourcePointId);
    if (p)
    {
        //p->count++;
        m_pSource->addLineCount(p->id);
    }
    item->addLine(this);
    if (adjust)
        onAdjust();
    updateChart();


}
void Line::setDest(Item *item, const bool adjust)
{
    m_pDest = item;
    if (nullptr == item)
        return;
    m_pChart->m_destPointId = item->getCurrentPointId();
    LPoint *p = Plugin::DataCenterPlugin()->getPoint(m_pChart->m_destPointId);
    if (p)
    {
        //p->count++;
        m_pDest->addLineCount(p->id);
    }
    item->addLine(this);
    if (adjust)
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

    update();
}

QPainterPath Line::shape() const
{
    QPainterPath path;
    path.addPolygon(getPolygon(MyPolyType::Area));
    return path;
}

QRectF Line::boundingRect() const
{
    /*if (!m_pSource || !m_pDest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + 6) / 2.0;

    return QRectF(m_sourcePoint, QSizeF(m_destPoint.x() - m_sourcePoint.x(),
                                      m_destPoint.y() - m_sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);*/
    QPainterPath path;
    path.addPolygon(getPolygon(MyPolyType::Area));
    return path.boundingRect();
}

bool Line::startTest()
{
    m_testState = TestState::Testing;
    //update();
    return true;
}

QPolygonF Line::getPolygon(const MyPolyType type) const
{
    QLineF line(m_sourcePoint, m_destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return QPolygonF();

    qreal arrowSize = 10;
    //QLineF line(m_sourcePoint, m_destPoint);
    double angle = std::atan2(-line.dy(), line.dx());

    QPolygonF py;
    if (type == MyPolyType::Draw)
    {
        QPointF destArrowP1 = m_destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                                      cos(angle - M_PI / 3) * arrowSize);
        QPointF destArrowP2 = m_destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                                  cos(angle - M_PI + M_PI / 3) * arrowSize);

        py << line.p1() << line.p2() << destArrowP1 << destArrowP2 << line.p2();
    }
    else
    {
        QPointF sourceArrowP1 = m_sourcePoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                                      cos(angle - M_PI / 3) * arrowSize);
        QPointF sourceArrowP2 = m_sourcePoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                                      cos(angle - M_PI + M_PI / 3) * arrowSize);

        QPointF destArrowP1 = m_destPoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI / 3) * arrowSize);
        QPointF destArrowP2 = m_destPoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI - M_PI / 3) * arrowSize);

        py << sourceArrowP1 << destArrowP1 << destArrowP2 << sourceArrowP2;
    }

    //if (type == MyPolyType::Draw)
    //    py << line.p1() << line.p2() << destArrowP1 << destArrowP2 << line.p2();
   // else
        //py << sourceArrowP1 << destArrowP1 << destArrowP2 << sourceArrowP2;
    //py << sourceArrowP1 << sourceArrowP2 << line.p1() << line.p2() << destArrowP1 << destArrowP2;
    return py;
}


void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //painter->fillRect(this->boundingRect(), QColor(255,0,0));

    //QLineF line(m_sourcePoint, m_destPoint);
    //if (qFuzzyCompare(line.length(), qreal(0.)))
    //    return;

    QColor bgColor = Qt::gray;
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

    painter->setBrush(bgColor);

    painter->drawPolyline(getPolygon(MyPolyType::Draw));

}

