#include "LZGraphicsView.h"
#include <QMouseEvent>
#include "Items/Node.h"
#include "Items/Line.h"
#include "Items/Virtual.h"
#include "Items/Start.h"
#include "Items/Finish.h"
#include "IPlugin.h"
#include <QUndoView>
#include <QUndoStack>
#include "Commands.h"
#include "LZLib.h"
#include <QDebug>

LZGraphicsView::LZGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    init();
}

void LZGraphicsView::init()
{
    setAcceptDrops(true);
    m_pScene = new LZGraphicsScene(this);
    m_pScene->setBackgroundBrush(QColor(0,0,0));
    m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(m_pScene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    m_worker = new DetectWorker();
    connect(this, &LZGraphicsView::detect_start, m_worker, &DetectWorker::on_detect_start);
    connect(m_worker, &DetectWorker::testFinished, this, &LZGraphicsView::onTestFinish);
    m_worker->start();

    createUndoView();
}

void LZGraphicsView::createUndoView()
{
    m_undoStack = new QUndoStack(this);
    m_undoView = new QUndoView(m_undoStack, this);
    m_undoView->setWindowTitle(tr("Command List"));
    m_undoView->show();
    m_undoView->setAttribute(Qt::WA_QuitOnClose, false);
    m_undoView->hide();
}

QWidget* LZGraphicsView::getUndoView()
{
    return m_undoView;
}

void LZGraphicsView::startTest()
{
    QList<Item*> list;
    foreach (QGraphicsItem *gi, m_pScene->items())
    {
        gi->setEnabled(false);
        list.append(qgraphicsitem_cast<Item*>(gi));
    }
    m_worker->setList(list);

    emit detect_start();
}
#include <QMessageBox>
void LZGraphicsView::onTestFinish()
{
    //QMessageBox::information(nullptr, "finish" , "finish");
    foreach (QGraphicsItem *gi, m_pScene->items())
    {
        gi->setEnabled(true);
    }
}


#include <QDebug>
void LZGraphicsView::view(const QString& title, const QList<LChart*>& list)
{
    Q_UNUSED(title);
    QMap<int, Item*> itemList;
    foreach(LChart* p, list)
    {
        Item* item = addItem(p);
        itemList.insert(p->id, item);
    }
    foreach(Item* p, itemList)
    {
        LChart* lc = p->getChart();
        if (nullptr != lc)
        {
            if (lc->m_type == LC_Line)
            {
                Item* dest = (Item*)itemList.value(Plugin::DataCenterPlugin()->getPoint(lc->m_destPointId)->chartId);
                Item* source = (Item*)itemList.value(Plugin::DataCenterPlugin()->getPoint(lc->m_sourcePointId)->chartId);
                Item* line = p;
                source->setCurrentPointId(lc->m_sourcePointId);
                dest->setCurrentPointId(lc->m_destPointId);
                connect(source, &Item::adjust, (Line*)line, &Line::onAdjust);
                connect(dest, &Item::adjust, (Line*)line, &Line::onAdjust);
                line->setSource(source);
                line->setDest(dest);
            }
        }
    }
}

Item* LZGraphicsView::addItem(LChart* p)
{
    if (nullptr == p)
        return nullptr;
    Item *item = addItem(p->m_type);
    if (nullptr != item)
    {
        m_pScene->addItem(item);
        item->setPos(QPointF(p->m_pos));
        item->setChart(p);
        item->initData();
        item->updatePoint();
    }
    return item;
}
Item* LZGraphicsView::addItem(const LCType type)
{
    Item *item = nullptr;
    switch (type)
    {
    case LCType::LC_Node:
        item = new Node();
        break;
    case LCType::LC_Line:
        item = new Line();
        break;
    case LCType::LC_Virtual:
        item = new Virtual();
        break;
    case LCType::LC_Start:
        item = new Start();
        break;
    case LCType::LC_Finish:
        item = new Finish();
        break;
    default:
        break;
    }
    if (nullptr != item)
    {
        connect(item, &Item::action, this, &LZGraphicsView::onAction);
        connect(item, &Item::remove, this, &LZGraphicsView::onRemove);
        connect(item, &Item::testing, m_worker, &DetectWorker::on_testing);
    }
    return item;
}

Item* LZGraphicsView::addItem(const LCType type, const QPoint& pt)
{
    Item *item = addItem(type);
    if (nullptr != item)
    {
        item->setPos(QPointF(pt));
        if (LCType::LC_Virtual != type)
        {
            item->setChart(new LChart(type)); 
            Plugin::DataCenterPlugin()->insertChart(item->getChart());
            item->initData();
            item->createPoint();
            item->updatePoint();

            QUndoCommand *addCommand = new AddCommand(item, m_pScene);
            m_undoStack->push(addCommand);
        }
    }

    return item;
}


void LZGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (nullptr != m_pVirtual)
    {
        QPointF ptf = this->mapToScene(event->pos());
        m_pVirtual->setPos(ptf.x(), ptf.y());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void LZGraphicsView::cancelEditing()
{
    if (nullptr != m_pVirtual)
    {
        m_pVirtual->deleteLater();
        m_pVirtual = nullptr;
    }
    if (nullptr != m_pLine)
    {
        m_pScene->removeItem(m_pLine);
        m_pLine->deleteLater();
        m_pLine = nullptr;
    }
}

void LZGraphicsView::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::RightButton:
    {
        cancelEditing();
    }
        break;
    default:
        break;
    }

    QPoint mousePos(event->pos());
    const QList<QGraphicsItem *> itemList = items(mousePos);
    m_movingItem = itemList.isEmpty() ? nullptr : itemList.first();
    if (m_movingItem != nullptr && event->button() == Qt::LeftButton)
        m_oldPos = m_movingItem->pos();

    QGraphicsView::mousePressEvent(event);
}

void LZGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_movingItem != nullptr && event->button() == Qt::LeftButton) {
        if (m_oldPos != m_movingItem->pos())
        {
            Item *item = qgraphicsitem_cast<Item *>(m_movingItem);
            LChart *p = item->getChart();
            p->m_pos = m_movingItem->pos().toPoint();
            m_undoStack->push(new MoveCommand(item, m_oldPos));

            item->updateChart();
        }
        m_movingItem = nullptr;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void LZGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0)
            zoomIn();
        else
            zoomOut();
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}


void LZGraphicsView::zoomIn()
{
    //scaleView(qreal(1.2));
    emit sceneZoom(-1);
}

void LZGraphicsView::zoomOut()
{
    //scaleView(1 / qreal(1.2));
    emit sceneZoom(1);
}

void LZGraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}


void LZGraphicsView::onRemove()
{
    Item *item = qobject_cast<Item*>(sender());
    if (nullptr == item)
        return;

    m_pScene->removeItem(item);
    item->deleteLater();
}

void LZGraphicsView::onAction(const Item::ActionType type)
{
    Item *source = qobject_cast<Item*>(sender());
    if (nullptr == source)
        return;
    switch (type)
    {
    case Item::ActionNormal:

        break;
    case Item::ActionLine:
    {

        if (nullptr == m_pVirtual)
        {
            if (Plugin::DataCenterPlugin()->getPoint(source->getCurrentPointId())->attribute == LPAttribute::output)
            {
                Item *dest = addItem(LCType::LC_Virtual, QPoint(source->pos().toPoint().x(), source->pos().toPoint().y()));
                Item *line = addItem(LCType::LC_Line, source->pos().toPoint());
                line->setSource(source);
                line->setDest(dest);
                connect(source, &Item::adjust, (Line*)line, &Line::onAdjust);
                connect(dest, &Item::adjust, (Line*)line, &Line::onAdjust);
                m_pSource = source;
                m_pVirtual = dest;
                m_pLine = line;
            }
        }
        else if (m_pSource == source)
        {
            cancelEditing();

        }
        else
        {
            if (Plugin::DataCenterPlugin()->getPoint(source->getCurrentPointId())->attribute == LPAttribute::input
                    && Plugin::DataCenterPlugin()->getPoint(source->getCurrentPointId())->type == Plugin::DataCenterPlugin()->getPoint(m_pSource->getCurrentPointId())->type)
            {
                if (m_pSource->actionType() == Item::ActionLine)
                {
                    Item *dest = source;
                    m_pLine->setDest(dest);
                    connect(dest, &Item::adjust, (Line*)m_pLine, &Line::onAdjust);

                    m_pVirtual->deleteLater();
                    m_pVirtual = nullptr;
                    m_pSource = nullptr;
                    m_pLine = nullptr;
                }
            }
        }
    }
        break;
    default:
        break;
    }
}
