#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "Items/Item.h"
//! [0]
class MoveCommand : public QUndoCommand
{
public:
    enum { Id = 1234 };

    MoveCommand(Item *diagramItem, const QPointF &oldPos,
                QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override { return Id; }

private:
    Item *myDiagramItem;
    QPointF myOldPos;
    QPointF newPos;
};
//! [0]

//! [1]
class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(QGraphicsScene *graphicsScene, QList<Item*>& itemList, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    Item *myDiagramItem;
    QGraphicsScene *myGraphicsScene;
    QList<Item*> m_itemList;
};
//! [1]

//! [2]
class AddCommand : public QUndoCommand
{
public:
    AddCommand(Item *diagramItem, QGraphicsScene *graphicsScene,
               QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;

private:
    Item *myDiagramItem;
    QGraphicsScene *myGraphicsScene;
    QPointF initialPosition;
};
//! [2]

QString createCommandString(Item *item, const QPointF &point);

#endif // COMMANDS_H
