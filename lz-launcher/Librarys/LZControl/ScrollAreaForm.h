#ifndef SCROLLAREAFORM_H
#define SCROLLAREAFORM_H

#include <QWidget>
#include "LZControl_global.h"

namespace Ui {
class ScrollAreaForm;
}

class LZCONTROL_EXPORT ScrollAreaForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollAreaForm(QWidget *parent = nullptr);
    ~ScrollAreaForm();

    void setItemWidth(const int width);
    void view(const QList<QWidget*>& list);
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
private:
    Ui::ScrollAreaForm *ui;

    QList<QWidget*> m_list;
    int m_itemWidth = 200;
};

#endif // SCROLLAREAFORM_H
