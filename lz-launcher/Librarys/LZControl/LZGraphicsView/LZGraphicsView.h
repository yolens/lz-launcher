#ifndef LZGRAPHICSVIEW_H
#define LZGRAPHICSVIEW_H

#include <QGraphicsView>
#include "LZControl_global.h"
#include "LZGraphicsScene.h"
#include "Items/Item.h"
#include "DetectWorker.h"

class QUndoStack;
class QUndoView;

class LZCONTROL_EXPORT LZGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LZGraphicsView(QWidget *parent = nullptr);
    ~LZGraphicsView();

    void view(LUnit* pUnit);
    void startTest();
    void stopTest();
    Item* addItem(LChart* p);
    Item* addItem(const LCType type);
    Item* addItem(const LCType type, const QPoint& pt);
    QWidget* getUndoView();
private:
    void init();
    void uninit();
    void cancelEditing();
    void createUndoView();
    void zoomIn();

    void zoomOut();
    void scaleView(qreal scaleFactor);
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
private slots:
    void onAction(const Item::ActionType type);
    void onRemove();
    void onTestFinish();
signals:
    void detect_start();
    void sceneZoom(const int value);
private:
    LZGraphicsScene *m_pScene = nullptr;
    Item *m_pSource = nullptr;
    Item *m_pVirtual = nullptr;
    Item *m_pLine = nullptr;
    DetectWorker *m_worker = nullptr;

    QUndoStack *m_undoStack = nullptr;
    QUndoView *m_undoView = nullptr;
    QGraphicsItem* m_movingItem = nullptr;
    QPointF m_oldPos;

    LUnit* m_pUnit = nullptr;
};

#endif // LZGRAPHICSVIEW_H
