#include "DetectWorker.h"

DetectWorker::DetectWorker(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    connect(this, &DetectWorker::valueTrigger, this, &DetectWorker::on_value_trigger);
}

DetectWorker::~DetectWorker()
{
    m_stopTest = true;
    if (nullptr != m_thread)
    {
        m_thread->quit();
        m_thread->wait();
        delete m_thread;
        m_thread = nullptr;
    }
}

void DetectWorker::start()
{
    m_thread->start();
}

void DetectWorker::setList(QList<Item*>& list)
{
    m_list = list;
}

int DetectWorker::runningCount()
{
    return m_threadCount;
}

void DetectWorker::stopTest()
{
    m_stopTest = true;
}

Item* DetectWorker::nextItemRunning(const int pid)
{
    static std::function<void(Item *p)> callback = [=](Item *p){
        testing(p);
    };

    Item *item = nullptr;
    foreach (const auto& a, m_list)
    {
        if (a->getChart()->m_sourcePointId == pid)
        {
            a->startTest(); //line
            item = a->getDest();
            if (nullptr == item)
                return nullptr;
            item->setTestingInput(a->getChart()->m_destPointId);
            if (nullptr != item)
            {
                if (!item->startTest()) //node
                {
                    item->testing(callback);
                    return nullptr;
                }

                break;
            }
        }
    }

    return item;
}

#include <QDebug>
void DetectWorker::on_value_trigger(const int pid)
{
    nextItemRunning(pid);
}

void DetectWorker::on_detect_start()
{
    m_stopTest = false;
    Item *start = nullptr;
    foreach(const auto& a, m_list)
    {
        a->initTest();
        if (a->getChart()->m_type == LCType::LC_Start)
        {
            start = a;
        }
    }
    if (start)
    {
        start->startTest();
        testing(start);
    }
    else
    {
        emit testFinished();
    }
}

void DetectWorker::testing(Item *item)
{
    Item *headItem = item;
    m_threadCount++;
    qInfo() << "EEEEEEEEEEEEEEEEEEEEEEEEE1" << QThread::currentThread() << m_threadCount;
    while (true)
    {
        if (nullptr == item || m_stopTest || headItem->isThreadStoped())
        {
            qInfo() << "DDDDDDDDDDDDDDDDDDDDDDDD";
            break;
        }

        //选择符合条件的点
        const LPoint *circuitP = item->nextCircuitPoint();
        const LPoint *valueP = item->nextValuePoint();

        //先赋值
        if (nullptr != valueP)
        {
            foreach (const auto& p, item->getPointList())
            {
                if (p->type == LPType::value)
                {
                    foreach (const auto& line, m_list)
                    {
                        if (line->getChart()->m_sourcePointId == p->id)
                        {
                            Item *dest = line->getDest();
                            if (nullptr != dest)
                            {
                                LPoint *destP = nullptr;
                                foreach (auto p1, dest->getPointList())
                                {

                                    if (p1->id == line->getChart()->m_destPointId)
                                    {
                                        destP = p1;
                                        break;
                                    }
                                }

                                //把上个元素的值传递到下个元素的点上
                                destP->outValue = p->outValue;
                                destP->outByteType = p->outByteType;

                                //是否有值触发的流程事件
                                switch (dest->witchFunction())
                                {
                                case Item::FunctionType::Nromal_func:
                                    break;
                                case Item::FunctionType::ValueTrigger_func:
                                {
                                    const LPoint *nextP = dest->startTest(destP->outValue, destP->outByteType);
                                    if (nullptr != nextP)
                                    {
                                        emit valueTrigger(nextP->id);
                                    }
                                }
                                    break;
                                default:
                                    break;
                                }

                                break;
                            }
                        }
                    }
                }
            }
        }

        //寻找下一个节点
        item = nullptr;
        if (nullptr != circuitP)
        {
            item = nextItemRunning(circuitP->id);
        }

        QThread::msleep(1);
    }

    m_threadCount--;
    if (m_threadCount <= 0)
        emit testFinished();

    qInfo() << "EEEEEEEEEEEEEEEEEEEEEEEEE2" << QThread::currentThread() << m_threadCount;
}

