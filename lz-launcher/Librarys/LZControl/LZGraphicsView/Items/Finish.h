#ifndef FINISH_H
#define FINISH_H

#include "Start.h"

class Finish : public Item
{
    Q_OBJECT
public:
    explicit Finish(QObject *parent = nullptr);

    virtual void startTest() override;
public:
    virtual void createPoint() override;
    virtual void updatePoint() override;
signals:

};

#endif // FINISH_H
