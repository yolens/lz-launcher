#include "LZGraphicsScene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

LZGraphicsScene::LZGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

void LZGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("Node/OrderTree"))
    {
        event->setAccepted(true);
    }
    else
    {
        event->setAccepted(false);
    }
}
void LZGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
}
void LZGraphicsScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{

}


void LZGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("Node/OrderTree"))
    {
        QVariant varData = event->mimeData()->data("Node/OrderTree");
        QByteArray byteData = varData.toByteArray();
        QDataStream stream(&byteData, QIODevice::ReadWrite);
        qint64 node;
        stream >> (node);

    }
}

