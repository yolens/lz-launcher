#include "DetectWorker.h"

DetectWorker::DetectWorker(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
}

void DetectWorker::start()
{
    m_thread->start();
}

void DetectWorker::setList(QList<Item*>& list)
{
    m_list = list;
}

void DetectWorker::stopTest()
{
    m_stopTest = true;
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
}
#include <QDebug>
void DetectWorker::testing(Item *item)
{
    QThread::msleep(1);

    //另外线程启动流程回调
    std::function<void(Item *p)> callback = [=](Item *p){
        testing(p);
    };

    if (m_stopTest)
    {
        emit testFinished();
        return;
    }
    if (nullptr == item)
        return;
    bool bTesting = false;
    LPoint *circuitP = nullptr;
    LPoint *valueP = nullptr;
    //选择符合条件的点
    foreach (const auto& p, item->getPointList())
    {
        if (p->type == LPType::circuit)
        {
            if (p->attribute == LPAttribute::output)
                circuitP = p;
        }
        else if (p->type == LPType::value)
        {
            if (p->attribute == LPAttribute::output)
                valueP = p;
        }
    }

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
                            QVariant value = item->getInputValue(p->valueId);
                            Item::FunctionType funcType = dest->setInputValue(destP->valueId, value);
                            switch (funcType)
                            {
                            case Item::FunctionType::Nromal_func:
                                break;
                            case Item::FunctionType::ValueTrigger_func:
                            {
                                const LPoint *nextP = dest->getNextPoint(value);
                                if (nullptr != nextP)
                                {

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
    if (nullptr != circuitP)
    {
        foreach (const auto& a, m_list)
        {
            if (a->getChart()->m_sourcePointId == circuitP->id)
            {
                a->startTest(); //line
                Item *dest = a->getDest();
                if (nullptr != dest)
                {
                    bTesting = true;
                    if (dest->startTest()) //node
                    {
                        testing(dest);
                    }
                    else
                    {
                        dest->testing(callback);
                    }
                    break;
                }
            }
        }
    }

    if (!bTesting)
    {
        emit testFinished();
        //on_detect_start();
    }
}

