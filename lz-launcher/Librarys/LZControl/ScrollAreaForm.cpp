#include "ScrollAreaForm.h"
#include "ui_ScrollAreaForm.h"
#include <QScrollArea>
#include <QLayout>

#include <QPushButton>
ScrollAreaForm::ScrollAreaForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScrollAreaForm)
{
    ui->setupUi(this);

}

ScrollAreaForm::~ScrollAreaForm()
{
    delete ui;
}
void ScrollAreaForm::setItemWidth(const int width)
{
    m_itemWidth = width;
}

void ScrollAreaForm::view(const QList<QWidget*>& list)
{
    m_list = list;
    int number = qMax(1, ui->scrollAreaWidgetContents->width()/m_itemWidth);
    int edge = 20;
    int spacing = 20;
    int x = 0;
    int y = edge;
    int w =  (ui->scrollAreaWidgetContents->width()-number*edge-spacing)/number;
    int h = w*0.7;

    for (int i = 0; i < list.count(); i++)
    {
        QWidget *bt = list.value(i);
        bt->setParent(ui->scrollAreaWidgetContents);
        bt->resize(w,h);

        if (i%number == 0)
        {
            x = edge;
            if (0 != i)
                y += h+spacing;
        }
        else
        {
            x += w+spacing;
        }
        bt->move(x,y);
        bt->show();
    }

    y += h+spacing;
    ui->scrollAreaWidgetContents->setFixedHeight(y);

}

void ScrollAreaForm::resizeEvent(QResizeEvent* event)
{
    view(m_list);
    QWidget::resizeEvent(event);
}
