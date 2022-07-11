#ifndef LINE_H
#define LINE_H

#include "Item.h"

class Line : public Item
{
    Q_OBJECT
public:
    explicit Line(QObject *parent = nullptr);
    virtual ~Line();

    virtual void setSource(Item *item) override;
    virtual void setDest(Item *item) override;
    virtual Item* getSource() override;
    virtual Item* getDest() override;
    virtual bool startTest() override;
private:
    void adjustLine();
public:
    QRectF boundingRect() const override;
protected:
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
public slots:
    void onAdjust();
signals:

private:
    Item *m_pSource = nullptr;
    Item *m_pDest = nullptr;
    QPointF m_sourcePoint;
    QPointF m_destPoint;

    QPolygonF m_arrowHead;
};

#endif // LINE_H
