#include "LZGraphicsView.h"
#include <QMouseEvent>
#include "Items/Node.h"
#include "Items/Line.h"

LZGraphicsView::LZGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    init();
}

void LZGraphicsView::init()
{
    setAcceptDrops(true);
    m_pScene = new LZGraphicsScene(this);
    m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(m_pScene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    //addItem(Item::ItemType::Node, QPoint(130,130));

}

Item* LZGraphicsView::addItem(const Item::ItemType type, const QPoint& pt)
{
    Item *item = nullptr;
    switch (type)
    {
    case Item::ItemType::Node:
        item = new Node();
        break;
    case Item::ItemType::Line:
        item = new Line();
        break;
    default:
        break;
    }
    if (nullptr != item)
    {
        m_pScene->addItem(item);
        //QPoint gpt = mapFromGlobal(pt);
        QPointF spt = mapToScene(pt);
        item->setPos(QPointF(spt));

    }

    return item;
}


void LZGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

    QGraphicsView::mouseMoveEvent(event);
}

void LZGraphicsView::mousePressEvent(QMouseEvent *event)
{

    QGraphicsView::mousePressEvent(event);
}
