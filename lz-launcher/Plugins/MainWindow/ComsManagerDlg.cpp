#include "ComsManagerDlg.h"
#include "ui_ComsManagerDlg.h"
#include "LZLib.h"
#include "IOrder.h"
#include <QPushButton>

ComsManagerDlg::ComsManagerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComsManagerDlg)
{
    ui->setupUi(this);
}

ComsManagerDlg::~ComsManagerDlg()
{
    delete ui;
}

void ComsManagerDlg::init()
{
    QList<IPlugin*> list = Plugin::Manager()->getPluginsByType(IPlugin::Coms);
    foreach (IPlugin *plugin, list)
    {
        IOrder* order = qobject_cast<IOrder*>(plugin->instance());
        if (nullptr != order)
        {
            QPushButton *button = new QPushButton(this);
            button->setText(plugin->pluginUuid().toString());
            button->show();
            button->move(10,10);
        }
    }
}
