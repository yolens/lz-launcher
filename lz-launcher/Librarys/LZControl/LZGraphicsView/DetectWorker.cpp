#include "DetectWorker.h"

DetectWorker::DetectWorker(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    connect(this, &DetectWorker::valueTrigger, this, &DetectWorker::on_value_trigger);
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
#include <QDebug>
void DetectWorker::on_value_trigger(const int pid)
{
    std::function<void(Item *p)> callback = [=](Item *p){
        testing(p);
    };

    foreach (const auto& a, m_list)
    {
        if (a->getChart()->m_sourcePointId == pid)
        {
            a->startTest(); //line
            Item* item = a->getDest();
            //testing(item);
            if (nullptr != item)
            {
                if (!item->startTest()) //node
                {
                    item->testing(callback);
                }

                break;
            }
        }
    }

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

void DetectWorker::testing(Item *item)
{
    qInfo() << "YYYYYYYYYYYYYYYYYYYYY";
    //另外线程启动流程回调
    std::function<void(Item *p)> callback = [=](Item *p){
        testing(p);
    };
    while (true)
    {
        QThread::msleep(1);

        if (m_stopTest)
        {
            emit testFinished();
            break;
        }
        if (nullptr == item)
            break;

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

                                QVariant value = p->outValue;

                                destP->outValue = value;
                                destP->outByteType = p->outByteType;
                                switch (dest->witchFunction())
                                {
                                case Item::FunctionType::Nromal_func:
                                    break;
                                case Item::FunctionType::ValueTrigger_func:
                                {
                                    const LPoint *nextP = dest->startTest(value);
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
            foreach (const auto& a, m_list)
            {
                if (a->getChart()->m_sourcePointId == circuitP->id)
                {
                    a->startTest(); //line
                    item = a->getDest();
                    if (nullptr != item)
                    {
                        if (!item->startTest()) //node
                        {
                            item->testing(callback);
                            return;
                        }
                        break;
                    }
                }
            }
        }

        if (nullptr == item)
        {

            emit testFinished();
            //on_detect_start();
            break;
        }
    }

    qInfo() << "EEEEEEEEEEEEEEEEEEEEEEEEE" << QThread::currentThread();
}

