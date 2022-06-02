#include "Finish.h"
#include "LZLib.h"
Finish::Finish(QObject *parent)
    : Item{parent, LCType::LC_Finish}
{
    //m_type = LCType::LC_Finish;
}

void Finish::createPoint()
{
    LPoint *p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::input;
    p->chartId = m_pChart->id;
    insertPoint(p);
}

void Finish::updatePoint()
{
    QVector<LPoint*> mv;
    foreach (auto i, m_pointVec)
    {
        if (i->type == LPType::circuit)
            mv.append(i);
    }

    if (!mv.isEmpty())
    {
        int y = m_rect.height()/2;
        int x = m_rect.width()/2;
        mv.at(0)->rect = QRect(QPoint(x-POINT_BIG_SIZE/2,y-POINT_BIG_SIZE/2), QSize(POINT_BIG_SIZE,POINT_BIG_SIZE));
    }
}

void Finish::startTest()
{
    m_testState = TestState::Testing;
    update();
    emit testFinish();
}
