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

void DetectWorker::on_detect_start()
{
    Item *start = nullptr;
    foreach(auto a, m_list)
    {
        a->initTest();
        if (a->getChart()->m_type == LCType::LC_Start)
        {
            start = a;
        }
    }
    if (start)
        start->startTest();
}
#include <QDebug>
void DetectWorker::on_testing()
{
    Item *item = qobject_cast<Item*>(sender());
    if (nullptr == item)
        return;
    bool bTesting = false;
    LPoint *circuitP = nullptr;
    LPoint *valueP = nullptr;
    foreach (LPoint *p, item->getPointList())
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


    if (nullptr != valueP) //先赋值
    {
        foreach (auto p, item->getPointList())
        {
            if (p->type == LPType::value)
            {
                foreach (auto line, m_list)
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
                            dest->setInputValue(destP->valueId, item->getInputValue(p->valueId));
                            break;
                        }
                    }
                }
            }
        }
    }
    if (nullptr != circuitP) //走下一个节点
    {
        foreach (auto a, m_list)
        {
            if (a->getChart()->m_sourcePointId == circuitP->id)
            {
                a->startTest(); //line
                Item *dest = a->getDest();
                if (nullptr != dest)
                {
                    bTesting = true;
                    dest->startTest(); //node
                    break;
                }
            }
        }
    }

    if (!bTesting)
    {
        emit testFinished();
    }
}
