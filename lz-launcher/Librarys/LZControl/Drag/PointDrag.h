#ifndef POINTDRAG_H
#define POINTDRAG_H

#include "DragWidget.h"

/*struct MyMimeData
{
    MyMimeData() {}
    LPType type;
    LPAttribute attribute;
    operator QVariant() const
    {
        return QVariant::fromValue(*this);

    }
    friend QDataStream& operator << (QDataStream& out, const MyMimeData& data)
    {
        out << data.type;
        out << data.attribute;
        return out;
    };
    friend QDataStream& operator >> (QDataStream& in, MyMimeData& data)
    {
        in >> data.type;
        in >> data.attribute;
        return in;
    };
};
Q_DECLARE_METATYPE(MyMimeData);*/

class PointDrag : public DragWidget
{
    Q_OBJECT
public:
    explicit PointDrag(QWidget *parent = nullptr);


protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;

public:
    LPType m_type;
    LPAttribute m_attribute;
};


#endif // POINTDRAG_H
