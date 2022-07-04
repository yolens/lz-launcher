#include "DataCenterPlugin.h"
#include "LZLib.h"
#include "IOrder.h"

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


    return true;
}
bool DataCenterPlugin::initSettings()
{
    return true;
}
bool DataCenterPlugin::startPlugin()
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
    QList<IPlugin*> list = Plugin::Manager()->getPluginsByType(IPlugin::Coms);
    foreach (IPlugin *plugin, list)
    {
        IOrder* order = qobject_cast<IOrder*>(plugin->instance());
        if (nullptr != order)
        {
            m_orderMap[order->type()] = order->orderList();
        }
    }
    return true;
}
bool DataCenterPlugin::stopPlugin()
{
    foreach(LPoint* p, m_pointList.values())
    {
        delete p;
    }
    foreach(LChart* p, m_chartList.values())
    {
        delete p;
    }
    return true;
}

QList<LChart*> DataCenterPlugin::getChartList(const int id)
{
    Q_UNUSED(id);
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

QMap<LOrder::Type, QList<LOrder*>>& DataCenterPlugin::getOrderList()
{
    qDebug() << "qqqqq= " << m_orderMap.size() << m_orderMap[LOrder::Modbus].size();
    return m_orderMap;
}

void DataCenterPlugin::execute(LOrder* p)
{
    if (nullptr == p)
        return;
    QList<IPlugin*> list = Plugin::Manager()->getPluginsByType(IPlugin::Coms);
    foreach (IPlugin *plugin, list)
    {
        IOrder* orderPlugin = qobject_cast<IOrder*>(plugin->instance());
        if (nullptr != orderPlugin)
        {
            if (p->type() == orderPlugin->type())
            {
                orderPlugin->execute(p);
                break;
            }

        }
    }

}

LOrder* DataCenterPlugin::newOrder(LOrder* p)
{
    if (nullptr == p)
        return nullptr;
    QList<IPlugin*> list = Plugin::Manager()->getPluginsByType(IPlugin::Coms);
    foreach (IPlugin *plugin, list)
    {
        IOrder* orderPlugin = qobject_cast<IOrder*>(plugin->instance());
        if (nullptr != orderPlugin)
        {
            if (p->type() == orderPlugin->type())
            {
                return orderPlugin->newOrder(p);
            }
        }
    }
    return nullptr;
}

LOrder* DataCenterPlugin::getOrder(const int id, const int type)
{
    QList<IPlugin*> list = Plugin::Manager()->getPluginsByType(IPlugin::Coms);
    foreach (IPlugin *plugin, list)
    {
        IOrder* orderPlugin = qobject_cast<IOrder*>(plugin->instance());
        if (nullptr != orderPlugin)
        {
            if (static_cast<LOrder::Type>(type) == orderPlugin->type())
            {
                QList<LOrder*> orderList = orderPlugin->orderList();
                foreach (LOrder* item, orderList)
                {
                    if (id == item->id)
                        return item;
                }
            }
        }
    }
    return nullptr;
}

QMap<LDevice::DeviceState, int> DataCenterPlugin::deviceStateList()
{
    QMap<LDevice::DeviceState, int> stateList;
    QList<IPlugin*> list = Plugin::Manager()->getPluginsByType(IPlugin::Coms);
    foreach (IPlugin *plugin, list)
    {
        IOrder* orderPlugin = qobject_cast<IOrder*>(plugin->instance());
        if (nullptr != orderPlugin)
        {
            QMap<LDevice::DeviceState, int> tempList = orderPlugin->deviceStateList();
            foreach (LDevice::DeviceState state, tempList.keys())
            {
                if (stateList.contains(state))
                    stateList[state] += tempList.value(state);
                else
                    stateList[state] = tempList.value(state);
            }
        }
    }
    return stateList;
}

QMap<LDevice::DeviceState, int> DataCenterPlugin::deviceStateListByType(const int type)
{
    QMap<LDevice::DeviceState, int> stateList;
    QList<IPlugin*> list = Plugin::Manager()->getPluginsByType(IPlugin::Coms);
    foreach (IPlugin *plugin, list)
    {
        IOrder* orderPlugin = qobject_cast<IOrder*>(plugin->instance());
        if (nullptr != orderPlugin)
        {
            if (static_cast<LOrder::Type>(type) == orderPlugin->type())
            {
                return orderPlugin->deviceStateList();
            }
        }
    }
    return stateList;
}

void DataCenterPlugin::registerDeviceState(std::function<void()> cb)
{
    m_deviceStateCallback.append(cb);
}

void DataCenterPlugin::noticeDeviceState()
{
    foreach (std::function<void()> callback, m_deviceStateCallback)
    {
        callback();
    }
}
