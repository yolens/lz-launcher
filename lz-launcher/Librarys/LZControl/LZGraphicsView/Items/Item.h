#ifndef ITEM_H
#define ITEM_H
#include <QObject>
#include <QGraphicsItem>
#include <QMetaEnum>

class Item : public QGraphicsItem
{
    //Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    enum ItemType {
        Node,
        Line
    };
    enum MouseState {
        Leave,
        Hover,
        Press,
    };
public:
    explicit Item(QGraphicsItem *parent = nullptr);
    virtual ~Item();

    virtual void setSource(const Item *item){};
    virtual void setDest(const Item *item){};
protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:

    MouseState m_mouseState = MouseState::Leave;
};

#endif // ITEM_H
