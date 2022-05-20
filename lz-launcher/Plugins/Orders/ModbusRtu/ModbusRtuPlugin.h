#ifndef MODBUSRTUPLUGIN_H
#define MODBUSRTUPLUGIN_H

#include "IOrder.h"

class ModbusRtuPlugin : public QObject, public IOrder, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QGenericPluginFactoryInterface_iid FILE "ModbusRtu.json")
    Q_INTERFACES(IOrder IPlugin)

public:
    explicit ModbusRtuPlugin(QObject *parent = nullptr);

    //IPlugin
    virtual QObject* instance() override;
    virtual QUuid pluginUuid() const override;
    virtual void pluginInfo(IPluginInfo *pluginInfo) override;
    virtual bool initConnections(IPluginManager *pluginManager, int& initOrder) override;
    virtual bool initObjects() override;
    virtual bool initSettings() override;
    virtual bool startPlugin() override;
    virtual bool stopPlugin() override;
};

#endif // MODBUSRTUPLUGIN_H
