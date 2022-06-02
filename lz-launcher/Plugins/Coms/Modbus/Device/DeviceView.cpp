#include "DeviceView.h"
#include "ui_DeviceView.h"
#include "DeviceCom.h"
#include "ModbusData.h"
const int ITEM_WIDTH = 300;

DeviceView::DeviceView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceView)
{
    ui->setupUi(this);

    QList<LDevice*> *list = ModbusData::instance()->deviceList();
    if (nullptr != list)
    {
        for (int i = 0; i<list->count(); i++)
        {
            DeviceCom *com = (DeviceCom*)list->value(i);
            initCom(com);
        }

    }
}

DeviceView::~DeviceView()
{
    delete ui;
}

void DeviceView::initCom(DeviceCom *com)
{
    connect(com, &DeviceCom::updateView, this, &DeviceView::on_updateView);
    com->init();
    com->setParent(ui->scrollAreaWidgetContents);
}

void DeviceView::adjustView()
{
    QList<LDevice*> *list = ModbusData::instance()->deviceList();
    if (nullptr == list)
        return;

    int number = qMax(1, ui->scrollAreaWidgetContents->width()/ITEM_WIDTH);
    int edge = 10;
    int spacing = 20;
    int x = 0;
    int y = edge;
    int w = (ui->scrollAreaWidgetContents->width()-number*edge-spacing)/number;
    int h = 0;
    for (int i = 0; i<list->count(); i++)
    {
        DeviceCom *com = (DeviceCom*)list->value(i);
        com->show();
        com->adjustSize();
        h = com->height();
        com->resize(w, h);

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
        com->move(x,y);
        com->show();

    }

    y += h+spacing;
    ui->scrollAreaWidgetContents->setFixedHeight(y);
}

void DeviceView::on_pushButton_add_clicked()
{
    DeviceCom *com = new DeviceCom(ui->scrollAreaWidgetContents);
    ModbusData::instance()->insert(com);

    initCom(com);
}

void DeviceView::resizeEvent(QResizeEvent* event)
{
    adjustView();
    QWidget::resizeEvent(event);
}

void DeviceView::on_updateView()
{
    adjustView();
}
