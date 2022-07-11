#include "DragWidget.h"

#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QBitmap>
#include "LZLib.h"

DragWidget::DragWidget(QWidget *parent)
    : QLabel{parent}
{

}


void DragWidget::drawDrag(QDrag *drag)
{
    if (nullptr == drag)
        return;

    QPixmap pixmap(34, 34);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);
    painter.translate(15, 15);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.end();

    pixmap.setMask(pixmap.createHeuristicMask());

    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(15, 20));


    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
}

void DragWidget::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
}
