#include "Commands.h"

#include <QGraphicsScene>

//! [0]
MoveCommand::MoveCommand(Item *diagramItem, const QPointF &oldPos,
                         QUndoCommand *parent)
    : QUndoCommand(parent), myDiagramItem(diagramItem)
    , myOldPos(oldPos), newPos(diagramItem->pos())
{
}
//! [0]

//! [1]
bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    Item *item = moveCommand->myDiagramItem;

    if (myDiagramItem != item)
        return false;

    newPos = item->pos();
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));

    return true;
}
//! [1]

//! [2]
void MoveCommand::undo()
{
    myDiagramItem->setPos(myOldPos);
    myDiagramItem->scene()->update();
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));
}
//! [2]

//! [3]
void MoveCommand::redo()
{
    myDiagramItem->setPos(newPos);
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));
}
//! [3]

//! [4]
DeleteCommand::DeleteCommand(QGraphicsScene *scene, QList<Item*>& itemList, QUndoCommand *parent)
    : QUndoCommand(parent), myGraphicsScene(scene)
{
   // QList<QGraphicsItem *> list = myGraphicsScene->selectedItems();
   // list.first()->setSelected(false);
   // myDiagramItem = static_cast<Item *>(list.first());
    m_itemList = itemList;
    QString str;
    for (int i=0; i<m_itemList.size(); i++)
    {
        str += createCommandString(m_itemList.at(i), m_itemList.at(i)->pos());
    }
    setText(QObject::tr("Delete %1")
        .arg(str));
}

//! [4]

//! [5]
void DeleteCommand::undo()
{
    //myGraphicsScene->addItem(myDiagramItem);
    for (int i=0; i<m_itemList.size(); i++)
    {
        myGraphicsScene->addItem(m_itemList.at(i));
    }
    myGraphicsScene->update();
}
//! [5]

//! [6]
void DeleteCommand::redo()
{
   // myGraphicsScene->removeItem(myDiagramItem);
    for (int i=0; i<m_itemList.size(); i++)
    {
        myGraphicsScene->removeItem(m_itemList.at(i));
    }
}
//! [6]

//! [7]
AddCommand::AddCommand(Item *diagramItem,
                       QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent), myGraphicsScene(scene)
{
    static int itemCount = 0;

    myDiagramItem = diagramItem;
    initialPosition = QPointF((itemCount * 15) % qMax(1, int(scene->width())),
                              (itemCount * 15) % qMax(1, int(scene->height())));
    scene->update();
    ++itemCount;
    setText(QObject::tr("Add %1")
        .arg(createCommandString(myDiagramItem, initialPosition)));
}
//! [7]

AddCommand::~AddCommand()
{
  //  if (!myDiagramItem->scene())
  //     delete myDiagramItem;
}

//! [8]
void AddCommand::undo()
{
    myGraphicsScene->removeItem(myDiagramItem);
    myGraphicsScene->update();
}
//! [8]

//! [9]
void AddCommand::redo()
{
    myGraphicsScene->addItem(myDiagramItem);
    //myDiagramItem->setPos(initialPosition);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}
//! [9]

QString createCommandString(Item *item, const QPointF &pos)
{
    return QObject::tr("%1 at (%2, %3)")
        .arg(item->getChart()->m_type)
        .arg(pos.x()).arg(pos.y());
}
