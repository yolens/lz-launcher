#ifndef THREAD_H
#define THREAD_H

#include "Item.h"
#include <QMutex>
class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = nullptr);
    virtual ~ThreadWorker();

    void setItem(Item* item);
    void registerCB(std::function<void(Item *p)> cb);
    bool isRunning();
    void setNextPointId(const int id);
    int nextPointId();
public slots:
    void on_testing();
signals:
    void finished();
private:
    QThread *m_thread = nullptr;
    std::function<void(Item *p)> m_cb = nullptr;
    Item *m_item = nullptr;
    bool m_isRunning = false;
    int m_nextPointId = 0;

};

class Thread : public Item
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = nullptr);
    virtual ~Thread();

    virtual bool startTest() override;
    virtual void testing(std::function<void(Item *p)> cb) override;
    virtual void createPoint() override;
    virtual const LPoint* nextCircuitPoint() override;
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event) override;
signals:
    void sig_testing();
private:
    ThreadWorker *m_worker = nullptr;
    QMutex m_mutex;
};

#endif // THREAD_H
