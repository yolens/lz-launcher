#include "PointDrag.h"

PointDrag::PointDrag(QWidget *parent) : DragWidget(parent)
{
    m_type = LPType::circuit;
    m_attribute = LPAttribute::output;
}

#include <QMimeData>
void PointDrag::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData;


    //MyMimeData myData;
    //myData.type = LPType::value;
    //myData.attribute = LPAttribute::output;
    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);
    stream << (qint64)this;
    mime->setData(Mime_Node_Point, encoded);

    drag->setMimeData(mime);

    DragWidget::drawDrag(drag);
}
