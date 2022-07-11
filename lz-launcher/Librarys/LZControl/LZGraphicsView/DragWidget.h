#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H
#include "LDB.h"
#include <QLabel>

class DragWidget : public QLabel
{
    Q_OBJECT
public:
    struct DragInfo
    {
        LCType type;
        QString name;
        QString dragNode;
    };
public:
    explicit DragWidget(QWidget *parent = nullptr);

    void setDragInfo(const DragInfo& info);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    QColor color;

    DragInfo m_dragInfo;

};

#endif // DRAGWIDGET_H
