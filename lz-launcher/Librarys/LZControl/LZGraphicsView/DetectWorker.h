#ifndef DETECTWORKER_H
#define DETECTWORKER_H

#include <QObject>
#include <QThread>
#include "Items/Item.h"

class DetectWorker : public QObject
{
    Q_OBJECT
public:
    explicit DetectWorker(QObject *parent = nullptr);
    virtual ~DetectWorker();

    void start();

    void stopTest();
    void setList(QList<Item*>& list);
    int runningCount();
private:
    Item* nextItemRunning(const int pid);
public slots:
    void on_detect_start();
    void on_value_trigger(const int pid);
private:
    void testing(Item *item);
signals:
    void testFinished();
    void valueTrigger(const int pid);
private:
    QThread *m_thread = nullptr;
    QList<Item*> m_list;
    bool m_stopTest = false;
    int m_threadCount = 0;
};

#endif // DETECTWORKER_H
