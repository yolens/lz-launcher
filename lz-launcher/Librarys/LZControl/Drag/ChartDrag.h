#ifndef CHARTDRAG_H
#define CHARTDRAG_H

#include "DragWidget.h"

class ChartDrag : public DragWidget
{
    Q_OBJECT
public:
    explicit ChartDrag(QWidget *parent = nullptr);

    void setType(const LCType type);
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    LCType m_type;
};

#endif // CHARTDRAG_H
