#ifndef BRANCH_H
#define BRANCH_H

#include "Item.h"

class Branch : public Item
{
    Q_OBJECT
public:
    explicit Branch(QObject *parent = nullptr);

    virtual bool startTest() override;
    virtual void createPoint() override;
    virtual Item::FunctionType setInputValue(const QString& id, const QVariant& value) override;
    virtual const LPoint* getNextPoint(const QVariant& value) override;
private:
    virtual void mouseRightClick(const LPoint* p) override;
protected:
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event) override;
signals:

};

#endif // BRANCH_H
