#ifndef LZGRAPHICSVIEW_H
#define LZGRAPHICSVIEW_H

#include <QGraphicsView>
#include "LZControl_global.h"
#include "LZGraphicsScene.h"
#include "Items/Item.h"

class LZCONTROL_EXPORT LZGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LZGraphicsView(QWidget *parent = nullptr);

    Item* addItem(const Item::ItemType type, const QPoint& pt = QPoint(0, 0));
private:
    void init();
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
signals:

private:
    LZGraphicsScene *m_pScene = nullptr;
};

#endif // LZGRAPHICSVIEW_H
