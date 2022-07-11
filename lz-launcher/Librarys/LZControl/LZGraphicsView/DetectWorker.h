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

    void start();

    void stopTest();
    void setList(QList<Item*>& list);
public:
    void on_detect_start();
private:
    void testing(Item *item);
signals:
    void testFinished();
private:
    QThread *m_thread = nullptr;
    QList<Item*> m_list;
    bool m_stopTest = false;
};

#endif // DETECTWORKER_H
