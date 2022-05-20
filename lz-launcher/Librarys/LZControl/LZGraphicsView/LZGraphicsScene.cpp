#include "LZGraphicsScene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

LZGraphicsScene::LZGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

void LZGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("Node/OrderTree1"))
    {
        event->setAccepted(true);
    }
    else
    {
        event->setAccepted(false);
    }
    QGraphicsScene::dragEnterEvent(event);
}
void LZGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
    QGraphicsScene::dragMoveEvent(event);
}
void LZGraphicsScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragLeaveEvent(event);
}


void LZGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("Node/OrderTree1"))
    {
        QVariant varData = event->mimeData()->data("Node/OrderTree1");
        QByteArray byteData = varData.toByteArray();
        QDataStream stream(&byteData, QIODevice::ReadWrite);
        qint64 node;
        stream >> (node);

    }
    QGraphicsScene::dropEvent(event);
}

