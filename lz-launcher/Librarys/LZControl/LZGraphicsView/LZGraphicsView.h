#ifndef LZGRAPHICSVIEW_H
#define LZGRAPHICSVIEW_H

#include <QGraphicsView>
#include "LZControl_global.h"
#include "LZGraphicsScene.h"
#include "Items/Item.h"

class QUndoStack;
class QUndoView;

class LZCONTROL_EXPORT LZGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LZGraphicsView(QWidget *parent = nullptr);

    void view(const QString& title, const QList<LChart*>& list);
    void startTest();
    Item* addItem(LChart* p);
    Item* addItem(const LCType type);
    Item* addItem(const LCType type, const QPoint& pt);
    QWidget* getUndoView();
private:
    void init();
    void cancelEditing();
    void createUndoView();
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
private slots:
    void onAction(const Item::ActionType type);
    void onRemove();
    void onTesting(const int outPointId);
    void onTestFinish();
signals:

private:
    LZGraphicsScene *m_pScene = nullptr;
    Item *m_pSource = nullptr;
    Item *m_pVirtual = nullptr;
    Item *m_pLine = nullptr;

    QUndoStack *m_undoStack = nullptr;
    QUndoView *m_undoView = nullptr;
    QGraphicsItem* m_movingItem = nullptr;
    QPointF m_oldPos;
};

#endif // LZGRAPHICSVIEW_H
