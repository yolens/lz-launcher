#include "Thread.h"
#include <QThread>

ThreadWorker::ThreadWorker(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_thread->start();
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
        m_isRunning = false;
    }
}

Thread::Thread(QObject *parent)
    : Item{parent, LCType::LC_Thread}
{
    m_typeName = "Thread";
}
#include <QThread>
#include <QTimer>
#include <QCoreApplication>

void Thread::testing(std::function<void(Item *p)> cb)
{
    m_worker->setItem(this);
    m_worker->registerCB(cb);
    emit sig_testing();
}

bool Thread::startTest()
{
    m_testState = TestState::Testing;

    QTimer::singleShot(0, this, [=]{
        update();
    });


    int times = 0;
    while (true)
    {
        int sec = qMin(500, m_pChart->m_delay-times);
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
    m_testState = TestState::Normal;
    QTimer::singleShot(0, this, [=]{
        update();
    });

    if (nullptr == m_worker)
    {
        m_worker = new ThreadWorker();
        connect(this, &Thread::sig_testing, m_worker, &ThreadWorker::on_testing);
        return false;
    }
    if (!m_worker->isRunning())
        return false;
    return true;
}

void Thread::createPoint()
{
    LPoint *p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::input;
    p->chartId = m_pChart->id;
    p->max = 1000;
    insertPoint(p);
    p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::output;
    p->chartId = m_pChart->id;
    insertPoint(p);
}
