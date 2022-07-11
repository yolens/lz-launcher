#ifndef THREAD_H
#define THREAD_H

#include "Item.h"

class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = nullptr);

    void setItem(Item* item);
    void registerCB(std::function<void(Item *p)> cb);
public slots:
    void on_testing();
private:
    QThread *m_thread = nullptr;
    std::function<void(Item *p)> m_cb = nullptr;
    Item *m_item = nullptr;
};

class Thread : public Item
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = nullptr);

    virtual bool startTest() override;
    virtual void testing(std::function<void(Item *p)> cb) override;
    virtual void createPoint() override;
signals:
    void sig_testing();
private:
    ThreadWorker *m_worker = nullptr;
};

#endif // THREAD_H
