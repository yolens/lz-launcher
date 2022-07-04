#include "ComsMgrForm.h"
#include "ui_ComsMgrForm.h"
#include "LZLib.h"
#include "IOrder.h"
#include <QPushButton>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

ComsMgrForm::ComsMgrForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComsMgrForm)
{
    ui->setupUi(this);
}

ComsMgrForm::~ComsMgrForm()
{
    delete ui;
}

void ComsMgrForm::init()
{
    QList<IPlugin*> list = Plugin::Manager()->getPluginsByType(IPlugin::Coms);
    foreach (IPlugin *plugin, list)
    {
        IOrder* order = qobject_cast<IOrder*>(plugin->instance());
        if (nullptr != order)
        {
            QPushButton *button = new QPushButton(this);
            connect(button, &QPushButton::clicked, this, &ComsMgrForm::on_clicked);
            button->setFixedSize(180, 180);
            button->setText(order->displayName());
            button->show();
            button->move(10,10);
            button->setProperty("plugin", (int)order);
            m_buttonList[order->type()] = button;
        }
    }

    std::function<void()> callback = [=](){
        foreach(int type, m_buttonList.keys())
        {
            QString mes;
            QMap<LDevice::DeviceState, int> stateList = Plugin::DataCenterPlugin()->deviceStateListByType(type);
            mes += tr("连接成功:%1").arg(stateList.value(LDevice::DeviceState::connectSuc));
            mes += "\r\n";
            mes += tr("连接失败:%1").arg(stateList.value(LDevice::DeviceState::connectFail));
            m_buttonList.value(type)->setText(mes);
        }

    };
    callback();
    Plugin::DataCenterPlugin()->registerDeviceState(callback);
}

void ComsMgrForm::on_clicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (nullptr == button)
        return;
    IOrder* order = (IOrder*)button->property("plugin").toInt();
    order->showWindow(true);
}
