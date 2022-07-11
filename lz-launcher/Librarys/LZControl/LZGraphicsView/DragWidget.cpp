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

void DragWidget::setDragInfo(const DragInfo& info)
{
    m_dragInfo = info;
    this->setText(m_dragInfo.name);
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
}


void DragWidget::mouseMoveEvent(QMouseEvent *event)
{
    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData;

    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);
    stream << (qint64)(m_dragInfo.type);
    mime->setData(m_dragInfo.dragNode, encoded);

    drag->setMimeData(mime);



    mime->setColorData(color);
    mime->setText(QString("#%1%2%3")
                  .arg(color.red(), 2, 16, QLatin1Char('0'))
                  .arg(color.green(), 2, 16, QLatin1Char('0'))
                  .arg(color.blue(), 2, 16, QLatin1Char('0')));

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
void DragWidget::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
}
