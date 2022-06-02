#include "ModbusPlugin.h"
#include <QVariant>
#include "Order/OrderView.h"
#include "Device/DeviceView.h"
#include "ModbusData.h"

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

    OrderView *o = new OrderView;
    o->show();

    DeviceView *com = new DeviceView;
    com->show();
    com->adjustView();
    return true;
}
bool ModbusPlugin::stopPlugin()
{
    return true;
}

void ModbusPlugin::execute()
{

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
