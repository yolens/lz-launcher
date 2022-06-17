#ifndef ITEM_H
#define ITEM_H
#include <QObject>
#include <QGraphicsItem>
#include <QMetaEnum>
#include "LDB.h"

const int POINT_SIZE = 10;
const int POINT_EDGE = 3;
const int POINT_BIG_SIZE = 30;

class Item : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    friend class Node;
    friend class Start;
    friend class Finish;
    friend class Line;
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
public:
    explicit Item(QObject *parent = nullptr, LCType type = LC_None);
    virtual ~Item();

    void setChart(LChart* p);
    void initData();
    void updateChart();
    void insertPoint(LPoint* p);
    LChart* getChart();

    void initTest(); //初始化测试
    virtual void startTest(); //开始检测

    Item::ActionType actionType();
    int getCurrentPointId();
    void setCurrentPointId(const int id);
    void addLine(Item *item);

    virtual void setSource(Item *item){Q_UNUSED(item);};
    virtual void setDest(Item *item){Q_UNUSED(item);};
    virtual Item* getSource(){return nullptr;};
    virtual Item* getDest(){return nullptr;};
    virtual void createPoint(){};
    virtual void updatePoint();
private:

public:
    virtual QRectF boundingRect() const override;
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private slots:
    void onRemoveLine();
    void onTestingTimer();
signals:
    void finished();
    void action(const Item::ActionType type);
    void remove();
    void adjust();
    void removeLine();
    void testing(const int outPointId);
    void testFinish();
private:

    MouseState m_mouseState = MouseState::Leave;
    ActionType m_action = ActionType::ActionNormal;

    int m_currentPointId = 0;
    QVector<LPoint*> m_pointVec;
    QRectF m_rect;

    LOrder *m_pOrder = nullptr;
    LChart *m_pChart = nullptr;
    QVector<Item*> m_lineList;
    TestState m_testState = TestState::Normal;
    int m_testingTimes = 0;
    QTimer *m_testingTimer = nullptr;
    bool m_bDrawRect = false;
};

#endif // ITEM_H
