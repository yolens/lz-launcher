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
    virtual QList<LChart*> getChartList(const int id) override;
    virtual LPoint* getPoint(const int id) override;
    virtual QList<LPoint*> getPointList(const int chartId) override;

    virtual bool insertChart(LChart* p) override;
    virtual bool updateChart(LChart* p) override;
    virtual bool removeChart(LChart* p) override;

    virtual bool insertPoint(LPoint* p) override;
    virtual bool updatePoint(LPoint* p) override;
    virtual bool removePoint(LPoint* p) override;

    virtual bool insertPoint(LOrder* p) override;
    virtual bool updatePoint(LOrder* p) override;
    virtual bool removePoint(LOrder* p) override;
private:
   // QMap<int, QMap<int, LChart>> m_chartList; //图表信息
    QMap<int, LPoint*> m_pointList; //自定义点信息列表
    QMap<int, LChart*> m_chartList;
    QMap<LOrder::Type, QMap<int, LOrder*>> m_orderList;

};

#endif // DATACENTERPLUGIN_H
