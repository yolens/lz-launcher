#include "PointDrag.h"

PointDrag::PointDrag(const LPType type, const LPAttribute attri, QWidget *parent) : DragWidget(parent)
{
    m_type = type;
    m_attribute = attri;
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
