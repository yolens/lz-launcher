#ifndef DATACENTERPLUGIN_H
#define DATACENTERPLUGIN_H

#include "IPlugin.h"
#include "IDataCenter.h"
#include "LDB.h"
#include <QMap>

class DataCenterPlugin : public QObject, public IDataCenter, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "LZ.IDataCenter/1.0" FILE "DataCenter.json")
    Q_INTERFACES(IDataCenter IPlugin)

public:
    explicit DataCenterPlugin(QObject *parent = nullptr);

public:
    //IPlugin
    virtual QObject* instance() override;
    virtual QUuid pluginUuid() const override;
    virtual IPlugin::Type pluginType() const override;
    virtual void pluginInfo(IPluginInfo *pluginInfo) override;
    virtual bool initConnections(IPluginManager *pluginManager, int& initOrder) override;
    virtual bool initObjects() override;
    virtual bool initSettings() override;
    virtual bool startPlugin() override;
    virtual bool stopPlugin() override;

    //IDataCenter
    virtual LProduct* getProduct(const int id) override;
    virtual QList<LProduct*> getProductList() override;
    virtual bool insertProduct(LProduct* p) override;
    virtual bool updateProduct(LProduct* p) override;
    virtual bool removeProduct(LProduct* p) override;

    virtual QList<LUnit*> getUnitList(const int id) override;
    virtual LUnit* getUnit(const int id) override;
    virtual bool insertUnit(LUnit* p) override;
    virtual bool hasUnit(const int id) override;
    virtual bool updateUnit(LUnit* p) override;
    virtual bool removeUnit(LUnit* p) override;

    virtual QList<LChart*> getChartList(const int id) override;
    virtual LPoint* getPoint(const int id) override;
    virtual QList<LPoint*> getPointList(const int chartId) override;

    virtual bool insertChart(LChart* p) override;
    virtual bool updateChart(LChart* p) override;
    virtual bool removeChart(LChart* p) override;

    virtual bool insertPoint(LPoint* p) override;
    virtual bool updatePoint(LPoint* p) override;
    virtual bool removePoint(LPoint* p) override;


    virtual void execute(LOrder* p) override;
    virtual QMap<LOrder::Type, QList<LOrder*>>& getOrderList() override;
    virtual LOrder* newOrder(LOrder* p) override;
    virtual LOrder* getOrder(const int id, const int type) override;

    virtual void registerDeviceState(std::function<void()> cb) override;
    virtual void noticeDeviceState() override;
    virtual QMap<LDevice::DeviceState, int> deviceStateList() override;
    virtual QMap<LDevice::DeviceState, int> deviceStateListByType(const int type) override;
private:
   // QMap<int, QMap<int, LChart>> m_chartList; //图表信息
    QMap<int, LProduct*> m_productList;
    QMap<int, LUnit*> m_unitList;
    QMap<int, LPoint*> m_pointList; //自定义点信息列表
    QMap<int, LChart*> m_chartList;
    QMap<LOrder::Type, QList<LOrder*>> m_orderMap;

    QList<std::function<void()>> m_deviceStateCallback;

};

#endif // DATACENTERPLUGIN_H
