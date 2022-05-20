#include "ModbusRtuPlugin.h"

ModbusRtuPlugin::ModbusRtuPlugin(QObject *parent)
    : QObject(parent)
{

}

QObject* ModbusRtuPlugin::instance()
{
    return this;
}
QUuid ModbusRtuPlugin::pluginUuid() const
{
    return ORDER_UUID;
}
void ModbusRtuPlugin::pluginInfo(IPluginInfo *pluginInfo)
{
    pluginInfo->author = "yolens";
    pluginInfo->name = "ModbusRtuPlugin";
    pluginInfo->version = "1.0.0";
}
bool ModbusRtuPlugin::initConnections(IPluginManager *pluginManager, int& initOrder)
{
    Q_UNUSED(pluginManager);
    initOrder = 2;
    return true;
}
bool ModbusRtuPlugin::initObjects()
{
    return true;
}
bool ModbusRtuPlugin::initSettings()
{
    return true;
}
bool ModbusRtuPlugin::startPlugin()
{
    return true;
}
bool ModbusRtuPlugin::stopPlugin()
{
    return true;
}
