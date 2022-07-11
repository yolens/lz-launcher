#include "ChartDrag.h"

ChartDrag::ChartDrag(QWidget *parent) : DragWidget(parent)
{

}

void ChartDrag::setType(const LCType type)
{
    m_type = type;
}

#include <QMimeData>
void ChartDrag::mouseMoveEvent(QMouseEvent *event)
{
    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData;

    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);
    stream << (qint64)(m_type);
    mime->setData(Mime_Node_Scene, encoded);

    drag->setMimeData(mime);

    DragWidget::drawDrag(drag);
}
