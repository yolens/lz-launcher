#ifndef LINE_H
#define LINE_H

#include "Item.h"

class Line : public Item
{
    //Q_OBJECT
public:
    explicit Line(QGraphicsItem *parent = nullptr);

    virtual void setSource(const Item *item) override;
    virtual void setDest(const Item *item) override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:

private:
    const Item *m_pSource = nullptr;
    const Item *m_pDest = nullptr;
};

#endif // LINE_H
