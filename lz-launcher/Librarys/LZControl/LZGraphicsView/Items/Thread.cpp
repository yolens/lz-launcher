#include "Thread.h"
#include <QThread>

ThreadWorker::ThreadWorker(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_thread->start();
}

ThreadWorker::~ThreadWorker()
{
    if (nullptr != m_thread)
    {
        m_thread->quit();
        m_thread->wait();
        delete m_thread;
        m_thread = nullptr;
    }
}

void ThreadWorker::setItem(Item* item)
{
    m_item = item;
}
#include <QDebug>
void ThreadWorker::registerCB(std::function<void(Item *p)> cb)
{
    m_cb = cb;
}

bool ThreadWorker::isRunning()
{
    return m_isRunning;
}

void ThreadWorker::setNextPointId(const int id)
{
    m_nextPointId = id;
}

int ThreadWorker::nextPointId()
{
    return m_nextPointId;
}

void ThreadWorker::on_testing()
{
    if (m_isRunning)
        return;
    if (nullptr != m_cb)
    {
        m_isRunning = true;
        qInfo() << "MMMMMMMMMMMMMMM222";
        m_cb(m_item);
        qInfo() << "MMMMMMMMMMMMMMM111";

        emit finished();
        m_isRunning = false;
    }
}

Thread::Thread(QObject *parent)
    : Item{parent, LCType::LC_Thread}
{
    m_typeName = "Thread";
    this->setAcceptDrops(true);
}
#include <QThread>
#include <QTimer>
#include <QCoreApplication>

Thread::~Thread()
{
    if (nullptr != m_worker)
    {
        delete m_worker;
        m_worker = nullptr;
    }
}

void Thread::testing(std::function<void(Item *p)> cb)
{
    m_worker->setItem(this);
    m_worker->registerCB(cb);
    emit sig_testing();
}

bool Thread::startTest()
{
    updateTestingState(TestState::Testing);

    waitDelayTime();

    updateTestingState(TestState::Normal);

    if (nullptr == m_worker)
    {
        m_worker = new ThreadWorker();
        m_worker->setNextPointId(m_testingInputId);
        connect(this, &Thread::sig_testing, m_worker, &ThreadWorker::on_testing);
        return false;
    }

    if (m_testingInputId != m_worker->nextPointId())
    {
        if (m_worker->isRunning())
        {
            m_worker->setNextPointId(m_testingInputId);
            QEventLoop loop;
            connect(m_worker, &ThreadWorker::finished, &loop, &QEventLoop::quit);
            m_threadStoped = true;
            loop.exec();
            m_threadStoped = false;
        }
        return false;
    }
    else
    {
        if (!m_worker->isRunning())
            return false;
    }

    return true;
}

const LPoint* Thread::nextCircuitPoint()
{

    LPoint *in = nullptr;
    foreach(const auto& i, m_pointVec)
    {
        if (LPType::circuit == i->type)
        {
            if (LPAttribute::input == i->attribute)
            {
                if (m_testingInputId == i->id)
                {
                    in = i;
                    break;
                }
            }
        }
    }

    if (nullptr != in)
    {
        foreach(const auto& i, m_pointVec)
        {
            if (LPType::circuit == i->type)
            {
                if (LPAttribute::output == i->attribute)
                {
                    if (in->linkId == i->linkId)
                    {
                        return i;
                    }
                }
            }
        }
    }

    return nullptr;
}

void Thread::createPoint()
{
    LPoint *p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::input;
    p->chartId = m_pChart->id;
    insertPoint(p);
    LPoint *in = p;
    p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::output;
    p->chartId = m_pChart->id;
    insertPoint(p);
    LPoint *out = p;
    in->linkId = out->linkId = in->id;
    Plugin::DataCenterPlugin()->updatePoint(in);
    Plugin::DataCenterPlugin()->updatePoint(out);
}

#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
void Thread::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
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
void Thread::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
    QGraphicsItem::dragMoveEvent(event);
}
void Thread::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsItem::dragLeaveEvent(event);
}

#include <QDebug>
#include <QGraphicsScene>
#include "Drag/PointDrag.h"
void Thread::dropEvent(QGraphicsSceneDragDropEvent *event)
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
            createPoint();
            updatePoint();
            update();
        }
    }
    QGraphicsItem::dropEvent(event);
}

#include <QPainter>
void Thread::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Item::paint(painter, option, widget);
    painter->setPen(QPen(Qt::yellow, 1, Qt::DotLine));

    QMap<int, QVector<LPoint*>> linkMap;
    foreach(const auto& i, m_pointVec)
    {
        switch (i->type)
        {
        case LPType::circuit:
        {
            linkMap[i->linkId].append(i);
        }
            break;
        default:
            break;
        }
    }
    for (const auto& v : linkMap.toStdMap())
    {
        if (v.second.size() >= 2)
        {
            painter->drawLine(v.second.at(0)->rect.center(), v.second.at(1)->rect.center());
        }
    }

}
