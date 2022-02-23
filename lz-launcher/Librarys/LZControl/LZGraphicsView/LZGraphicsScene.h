#ifndef LZGRAPHICSSCENE_H
#define LZGRAPHICSSCENE_H

#include <QGraphicsScene>

class LZGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit LZGraphicsScene(QObject *parent = nullptr);
protected:
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event) override;

signals:

private:

};

#endif // LZGRAPHICSSCENE_H
