#include "DataCenterPlugin.h"

DataCenterPlugin::DataCenterPlugin(QObject *parent)
    : QObject(parent)
{

}

QObject* DataCenterPlugin::instance()
{
    return this;
}
QUuid DataCenterPlugin::pluginUuid() const
{
    return DATACENTER_UUID;
}
IPlugin::Type DataCenterPlugin::pluginType() const
{
    return Type::Main;
}
void DataCenterPlugin::pluginInfo(IPluginInfo *pluginInfo)
{
    pluginInfo->author = "yolens";
    pluginInfo->name = "DataCenterPlugin";
    pluginInfo->version = "1.0.0";
}
bool DataCenterPlugin::initConnections(IPluginManager *pluginManager, int& initOrder)
{
    Q_UNUSED(pluginManager);
    initOrder = Type::Main+1;
    return true;
}
#include <QDebug>
bool DataCenterPlugin::initObjects()
{
    QList<LPoint*> pointList = LPoint::get();
    foreach (LPoint* p, pointList)
    {
        m_pointList.insert(p->id, p);
    }

    QList<LChart*> chartList = LChart::get();
    foreach (LChart* p, chartList)
    {
        m_chartList.insert(p->id, p);
    }

    return true;
}
bool DataCenterPlugin::initSettings()
{
    return true;
}
bool DataCenterPlugin::startPlugin()
{

    return true;
}
bool DataCenterPlugin::stopPlugin()
{
    return true;
}

QList<LChart*> DataCenterPlugin::getChartList(const int id)
{
    QList<LChart*> list;
    foreach(LChart* p, m_chartList)
    {
        //if (id == p->id)
            list.push_back(p);

    }
    return list;
}

QList<LPoint*> DataCenterPlugin::getPointList(const int chartId)
{
    QList<LPoint*> list;
    foreach(LPoint* p, m_pointList)
    {
        if (chartId == p->chartId)
            list.push_back(p);
    }
    return list;
}

LPoint* DataCenterPlugin::getPoint(const int id)
{
    return m_pointList.value(id);
}

bool DataCenterPlugin::insertChart(LChart* p)
{
    if (nullptr == p)
        return false;
    if (p->insertDb())
    {
        m_chartList.insert(p->id, p);
        return true;
    }
    return false;
}

bool DataCenterPlugin::updateChart(LChart* p)
{
    if (nullptr == p)
        return false;
    if (p->updateDb())
    {
        return true;
    }
    return false;
}

bool DataCenterPlugin::removeChart(LChart* p)
{
    if (nullptr == p)
        return false;
    if (p->removeDb())
    {
        m_chartList.remove(p->id);
        return true;
    }
    return false;
}

bool DataCenterPlugin::insertPoint(LPoint* p)
{
    if (nullptr == p)
        return false;
    if (p->insertDb())
    {
        m_pointList.insert(p->id, p);
        return true;
    }
    return false;
}

bool DataCenterPlugin::updatePoint(LPoint* p)
{
    if (nullptr == p)
        return false;
    if (p->updateDb())
    {
        return true;
    }
    return false;
}
bool DataCenterPlugin::removePoint(LPoint* p)
{
    if (nullptr == p)
        return false;
    if (p->removeDb())
    {
        m_pointList.remove(p->id);
        return true;
    }
    return false;
}

bool DataCenterPlugin::insertPoint(LOrder* p)
{
    if (nullptr == p)
        return false;
    if (p->insertDb())
    {
        QMap<int, LOrder*> list;
        if (m_orderList.contains(p->type()))
            list = m_orderList.value(p->type());
        list.insert(p->id, p);
        if (!m_orderList.contains(p->type()))
            m_orderList.insert(p->type(), list);
        return true;
    }
    return false;
}
bool DataCenterPlugin::updatePoint(LOrder* p)
{
    if (nullptr == p)
        return false;
    if (p->updateDb())
    {
        return true;
    }
    return false;
}
bool DataCenterPlugin::removePoint(LOrder* p)
{
    if (nullptr == p)
        return false;
    if (p->removeDb())
    {
        QMap<int, LOrder*> list;
        if (m_orderList.contains(p->type()))
            list = m_orderList.value(p->type());
        list.remove(p->id);
        return true;
    }
    return false;
}
