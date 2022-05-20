#ifndef START_H
#define START_H

#include "Item.h"

class Start : public Item
{
    Q_OBJECT
public:
    explicit Start(QObject *parent = nullptr);

    virtual void startTest() override;
public:
    virtual void createPoint() override;
    virtual void updatePoint() override;
signals:

};

#endif // START_H
