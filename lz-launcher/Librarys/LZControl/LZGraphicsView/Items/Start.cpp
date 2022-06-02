#include "Start.h"
#include "LZLib.h"

Start::Start(QObject *parent)
    : Item{parent, LCType::LC_Start}
{
    //m_type = LCType::LC_Start;
    m_rect = QRectF(0, 0, 60, 60);
}

void Start::createPoint()
{
    LPoint *p = new LPoint();
    p->type = LPType::circuit;
    p->attribute = LPAttribute::output;
    p->chartId = m_pChart->id;
    insertPoint(p);
}

void Start::updatePoint()
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

#include <QTimer>
void Start::startTest()
{
    m_testState = TestState::Testing;
    update();

    foreach (LPoint *p, m_pointVec)
    {
        if (p->attribute == LPAttribute::output)
        {
            emit testing(p->id);
            break;
        }
    }
}
