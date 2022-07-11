#include "LZGraphicsScene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include "LZLib.h"

LZGraphicsScene::LZGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

void LZGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(Mime_Node_Scene))
    {
        event->setAccepted(true);
    }
    else
    {
        QGraphicsScene::dragEnterEvent(event);
    }
}
void LZGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(Mime_Node_Scene))
    {
        event->setAccepted(true);
    }
    else
    {
        QGraphicsScene::dragMoveEvent(event);
    }
}
void LZGraphicsScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(Mime_Node_Scene))
    {

    }
    else
    {
        QGraphicsScene::dragLeaveEvent(event);
    }
}


void LZGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(Mime_Node_Scene))
    {
        QVariant varData = event->mimeData()->data(Mime_Node_Scene);
        QByteArray byteData = varData.toByteArray();
        QDataStream stream(&byteData, QIODevice::ReadWrite);
        qint64 node;
        stream >> (node);
        emit sig_addItem(node);
    }
    else
    {
        QGraphicsScene::dropEvent(event);
    }
}

