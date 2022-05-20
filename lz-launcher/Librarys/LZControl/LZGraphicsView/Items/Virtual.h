#ifndef VIRTUAL_H
#define VIRTUAL_H

#include "Item.h"

class Virtual : public Item
{
    Q_OBJECT
public:
    explicit Virtual(QObject *parent = nullptr);
public:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
signals:

};

#endif // VIRTUAL_H
