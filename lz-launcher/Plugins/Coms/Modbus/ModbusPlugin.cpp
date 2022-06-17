#include "ModbusPlugin.h"
#include <QVariant>
#include "ModbusData.h"
#include "ModbusWindow.h"

ModbusPlugin::ModbusPlugin(QObject *parent)
    : QObject(parent)
{

}

QObject* ModbusPlugin::instance()
{
    return this;
}
QUuid ModbusPlugin::pluginUuid() const
{
    return MODBUS_UUID;
}
IPlugin::Type ModbusPlugin::pluginType() const
{
    return Type::Coms;
}
void ModbusPlugin::pluginInfo(IPluginInfo *pluginInfo)
{
    pluginInfo->author = "yolens";
    pluginInfo->name = "ModbusPlugin";
    pluginInfo->version = "1.0.0";
}
bool ModbusPlugin::initConnections(IPluginManager *pluginManager, int& initOrder)
{
    Q_UNUSED(pluginManager);
    initOrder = Type::Coms+1;
    return true;
}

bool ModbusPlugin::initObjects()
{

    return true;
}
bool ModbusPlugin::initSettings()
{
    return true;
}

bool ModbusPlugin::startPlugin()
{
    ModbusData::instance()->loadDb();
    return true;
}
bool ModbusPlugin::stopPlugin()
{
    QList<LDevice*> *list = ModbusData::instance()->deviceList();
    if (nullptr != list)
    {
        for (int i = 0; i<list->count(); i++)
        {
            delete list->value(i);
        }

    }
    return true;
}

void ModbusPlugin::execute(LOrder *order)
{
    QList<LDevice*> *list =  ModbusData::instance()->deviceList();
    if (nullptr != list)
    {
        for (int i = 0; i<list->count(); i++)
        {
            if (order->deviceId() == list->at(i)->id)
            {
                list->at(i)->execute(order);
                break;
            }
        }

    }

}
void ModbusPlugin::write(QVariant value)
{

}
QVariant ModbusPlugin::read()
{
    return QVariant();
}

QWidget* ModbusPlugin::getWidget()
{
    return nullptr;
}

void ModbusPlugin::showWindow(const bool show)
{
    if (show)
        ModbusWindow::instance()->show();
    else
        ModbusWindow::instance()->hide();
}

QString ModbusPlugin::displayName()
{
    return "Modbus";
}

LOrder::Type ModbusPlugin::type()
{
    return LOrder::Modbus;
}

QList<LOrder*>& ModbusPlugin::orderList()
{
 return ModbusData::instance()->orderList();
}

LOrder* ModbusPlugin::newOrder(LOrder* p)
{
    if (nullptr == p)
        return nullptr;
    OrderCom *item = new OrderCom(*((OrderCom*)p));
    return item;
}
