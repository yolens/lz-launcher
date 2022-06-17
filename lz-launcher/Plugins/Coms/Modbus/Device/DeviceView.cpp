﻿#include "DeviceView.h"
#include "ui_DeviceView.h"
#include "DeviceCom.h"
#include "ModbusData.h"
#include <QtDebug>
#include <QAbstractSpinBox>
#include <QComboBox>
const int ITEM_WIDTH = 300;

//IEEE 754浮点数十六进制相互转换
//32位 四字节 单精度
long FloatToHex(float f)
{
    return *(long*)&f;
}

float HexToFloat(long hex)
{
    return *(float*)&hex;
}

DeviceView::DeviceView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceView)
{
    ui->setupUi(this);

    QList<LDevice*> *list = ModbusData::instance()->deviceList();
    //list->clear();
    if (nullptr != list)
    {
        for (int i = 0; i<list->count(); i++)
        {
            DeviceCom *com = dynamic_cast<DeviceCom*>(list->value(i));
            initCom(com);
        }

    }

    foreach (QAbstractSpinBox *sb, this->findChildren<QAbstractSpinBox*>())
    {
        sb->installEventFilter(this);
    }
    foreach (QComboBox *cb, this->findChildren<QComboBox*>())
    {
        cb->installEventFilter(this);
    }

    float ff = 2.345;
    long l = FloatToHex(ff);
    qDebug() << "UUU= " << QString::number(l, 16) << l;

    float rf = HexToFloat(l);
    qDebug() << "UUUF= " << rf;

    QString kkstr = "4016147b";
    long kk = kkstr.toLong(nullptr, 16);
    float ddrf = HexToFloat(kk);
    qDebug() << "UUUFss= " << ddrf;
}

DeviceView::~DeviceView()
{

    delete ui;
}

void DeviceView::initCom(DeviceCom *com)
{
    connect(com, &DeviceCom::updateView, this, &DeviceView::on_updateView);
    connect(com, &DeviceCom::changeDeviceName, this, &DeviceView::changeDeviceName);
    com->init();
    com->setParent(ui->scrollAreaWidgetContents);
}
#include <QtDebug>
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
    adjustView();

    emit changeDeviceName();
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

bool DeviceView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->inherits("QAbstractSpinBox") || watched->inherits("QComboBox"))
    {
        if (QEvent::Wheel == event->type())
            return true;
    }
    return QWidget::eventFilter(watched, event);
}
