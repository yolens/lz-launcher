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
    void setList(QList<Item*>& list);
public:
    void on_testing();
    void on_detect_start();
signals:
    void testFinished();
private:
    QThread *m_thread = nullptr;
    QList<Item*> m_list;
};

#endif // DETECTWORKER_H
