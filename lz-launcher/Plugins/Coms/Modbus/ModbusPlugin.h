#ifndef MODBUSPLUGIN_H
#define MODBUSPLUGIN_H

#include "IPlugin.h"
#include "IOrder.h"

class ModbusPlugin : public QObject, public IOrder, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "LZ.IOrder/1.0" FILE "Modbus.json")
    Q_INTERFACES(IOrder IPlugin)

public:
    explicit ModbusPlugin(QObject *parent = nullptr);

public:
    //IPlugin
    virtual QObject* instance() override;
    virtual QUuid pluginUuid() const override;
    IPlugin::Type pluginType() const override;
    virtual void pluginInfo(IPluginInfo *pluginInfo) override;
    virtual bool initConnections(IPluginManager *pluginManager, int& initOrder) override;
    virtual bool initObjects() override;
    virtual bool initSettings() override;
    virtual bool startPlugin() override;
    virtual bool stopPlugin() override;

    //IOrder
    virtual bool execute(LOrder *order) override;
    virtual void write(QVariant value) override;
    virtual QVariant read() override;
    virtual QWidget* getWidget() override;
    virtual void showWindow(const bool show) override;
    virtual QString displayName() override;
    virtual LOrder::Type type() override;
    virtual QList<LOrder*>& orderList() override;
    virtual LOrder* newOrder(LOrder* p) override;
    virtual QMap<LDevice::DeviceState, int> deviceStateList() override;
};

#endif // MODBUSPLUGIN_H
