#include "ModbusWindow.h"
#include "ui_ModbusWindow.h"
#include "Order/OrderView.h"
#include "Device/DeviceView.h"
#include <QSplitter>

ModbusWindow::ModbusWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModbusWindow)
{
    ui->setupUi(this);



    OrderView *orderView = new OrderView(ui->centralwidget);
    DeviceView *deviceView = new DeviceView(ui->centralwidget);

    connect(deviceView, &DeviceView::changeDeviceName, orderView, &OrderView::on_changeDeviceName);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    QString style = QString("QSplitter::handle{background-color:rgb(179,179,179);}");
    splitter->setStyleSheet(style);
    //splitter->setChildrenCollapsible(false);
    splitter->addWidget(deviceView);
    splitter->addWidget(orderView);
    ui->centralwidget->layout()->addWidget(splitter);

}

ModbusWindow::~ModbusWindow()
{
    delete ui;
}

ModbusWindow* ModbusWindow::instance()
{
    static ModbusWindow ins;
    return &ins;
}

void ModbusWindow::init()
{

}
