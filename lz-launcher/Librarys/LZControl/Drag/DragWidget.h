#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include "LZLib.h"
#include <QLabel>
#include <QDrag>

class DragWidget : public QLabel
{
    Q_OBJECT
public:
    explicit DragWidget(QWidget *parent = nullptr);

    void drawDrag(QDrag *drag);
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

};

#endif // DRAGWIDGET_H
