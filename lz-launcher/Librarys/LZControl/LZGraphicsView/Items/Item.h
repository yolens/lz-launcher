#ifndef ITEM_H
#define ITEM_H
#include <QObject>
#include <QGraphicsItem>
#include <QMetaEnum>
#include "LDB.h"
#include "Dialog/ChartDlg.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

const int POINT_SIZE = 14;
const int POINT_EDGE = 4;
const int POINT_BIG_SIZE = 30;
const int DRAG_WIDTH = 2;

class Item : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    friend class Virtual;
    friend class Node;
    friend class Start;
    friend class Finish;
    friend class Line;
    friend class Thread;
    friend class Branch;
    friend class ControlPanel;
public:
    enum ActionType {
        ActionNormal,
        ActionLine,          //连线
    };

    enum MouseState {
        Leave,
        Hover,
        Press,
    };

    enum TestState {
        Normal,
        Testing,
        Ok,
        Fail,
    };

    enum DragType
    {
        //没有进行拖拽
        Release = 0,

        //五个点的定义
        Left,
        Right,
        Top,
        Bottom,
        Center,
        RightBottom,
    };

    enum FunctionType
    {
        Nromal_func = 0,
        ValueTrigger_func,
    };

public:
    explicit Item(QObject *parent = nullptr, LCType type = LC_None);
    virtual ~Item();

    static bool runningState();
    static void setRunningState(const bool state);

    void updateData();
    LCType getItemType();
    QString& getTypeName();
    void setChart(LChart* p);
    void initData();
    void updateChart();
    void insertPoint(LPoint* p);
    LChart* getChart();
    LOrder* getOrder();
    QVector<LPoint*>& getPointList();
    void setTestingInput(const int id);
    bool isThreadStoped();
    void updateTestingState(const TestState state);
    void waitDelayTime();
    void addLineCount(const int id);
    void removeLineCount(const int id);
    int getLineCount(const int id);

    void initTest(); //初始化测试
    virtual bool startTest(); //开始检测
    virtual const LPoint* startTest(const QVariant& value, const LOrder::ByteType type){Q_UNUSED(value); Q_UNUSED(type); return nullptr;};
    virtual void testing(std::function<void(Item *p)> cb){Q_UNUSED(cb);};
    virtual Item::FunctionType witchFunction(){return FunctionType::Nromal_func;};
    virtual void mouseRightClick(const LPoint* p){Q_UNUSED(p);};
    virtual const LPoint* nextCircuitPoint();
    virtual const LPoint* nextValuePoint();

    Item::ActionType actionType();
    int getCurrentPointId();
    void setCurrentPointId(const int id);
    void addLine(Item *item);

    virtual void clear();
    virtual void setSource(Item *item, const bool adjust = true){Q_UNUSED(item); Q_UNUSED(adjust);};
    virtual void setDest(Item *item, const bool adjust = true){Q_UNUSED(item); Q_UNUSED(adjust);};
    virtual Item* getSource(){return nullptr;};
    virtual Item* getDest(){return nullptr;};
    virtual void createPoint(){};
    virtual void updatePoint();
private:
    void setSize(const QSizeF& size);
    LPoint* getPointInPos(const QPoint& pt);
public:
    virtual QRectF boundingRect() const override;
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private slots:
    void onRemoveLine();
signals:
    void finished();
    void action(const Item::ActionType type);
    void remove();
    void adjust();
    void removeLine();
    void testFinish();
private:
    QString m_typeName; //类型名称
    MouseState m_mouseState = MouseState::Leave;
    ActionType m_action = ActionType::ActionNormal;
    LCType m_type;

    int m_currentPointId = 0;
    QVector<LPoint*> m_pointVec;
    QMap<int, int> m_pointLineCountMap;
    QRectF m_rect;

    LOrder *m_pOrder = nullptr;
    LChart *m_pChart = nullptr;
    QVector<Item*> m_lineList;
    TestState m_testState = TestState::Normal;
    int m_testingTimes = 0;
    QTimer *m_testingTimer = nullptr;
    bool m_bDrawRect = false;
    DragType m_dragType = DragType::Release;
    QPointF m_startPos;
    int m_testingInputId = 0;
    bool m_threadStoped = false;

    static bool m_runningState;
};

#endif // ITEM_H
