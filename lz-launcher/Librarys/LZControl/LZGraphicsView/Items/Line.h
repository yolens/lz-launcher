#ifndef LINE_H
#define LINE_H

#include "Item.h"

class Line : public Item
{
    Q_OBJECT
public:
    enum MyPolyType
    {
        Draw = 0,
        Area,
    };
public:
    explicit Line(QObject *parent = nullptr);
    virtual ~Line();

    virtual void clear() override;
    virtual void setSource(Item *item, const bool adjust = true) override;
    virtual void setDest(Item *item, const bool adjust = true) override;
    virtual Item* getSource() override;
    virtual Item* getDest() override;
    virtual bool startTest() override;
private:
    void adjustLine();
    QPolygonF getPolygon(const MyPolyType type) const;
public:
    virtual QRectF boundingRect() const override;
protected:
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void onAdjust();
signals:

private:
    Item *m_pSource = nullptr;
    Item *m_pDest = nullptr;
    QPointF m_sourcePoint;
    QPointF m_destPoint;

};

#endif // LINE_H
