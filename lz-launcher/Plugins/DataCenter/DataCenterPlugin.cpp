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
    foreach (LProduct* p, LProduct::get())
    {
        m_productList.insert(p->id, p);
    }

    foreach (LUnit* p, LUnit::get())
    {
        m_unitList.insert(p->id, p);
    }

    foreach (LPoint* p, LPoint::get())
    {
        m_pointList.insert(p->id, p);
    }

    foreach (LChart* p, LChart::get())
    {
        m_chartList.insert(p->id, p);
    }

    foreach (IPlugin *plugin, Plugin::Manager()->getPluginsByType(IPlugin::Coms))
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
    foreach(const auto& p, m_pointList.values())
    {
        delete p;
    }
    foreach(const auto& p, m_chartList.values())
    {
        delete p;
    }
    return true;
}

LProduct* DataCenterPlugin::getProduct(const int id)
{
    return m_productList.value(id);
}

QList<LProduct*> DataCenterPlugin::getProductList()
{
    QList<LProduct*> list;
    foreach(const auto& p, m_productList.values())
    {
        list.push_back(p);
    }
    return list;
}

bool DataCenterPlugin::insertProduct(LProduct* p)
{
    if (nullptr == p)
        return false;
    if (p->insertDb())
    {
        m_productList.insert(p->id, p);
        return true;
    }
    return false;
}

bool DataCenterPlugin::updateProduct(LProduct* p)
{
    if (nullptr == p)
        return false;
    if (p->updateDb())
    {
        return true;
    }
    return false;
}

bool DataCenterPlugin::removeProduct(LProduct* p)
{
    if (nullptr == p)
        return false;
    //1、删除子单元
    foreach(const auto& u, getUnitList(p->id))
    {
        //2、删除Chart
        foreach(const auto& c, getChartList(p->id))
        {
            //3、删除点
            foreach(const auto&p, getPointList(c->id))
            {
                removePoint(p);
            }
            removeChart(c);
        }
        removeUnit(u);
    }
    //4、删除产品本身
    if (p->removeDb())
    {
        m_chartList.remove(p->id);
        return true;
    }
    return false;
}

QList<LUnit*> DataCenterPlugin::getUnitList(const int id)
{
    QList<LUnit*> list;
    foreach(const auto& p, m_unitList)
    {
        if (id == p->productId)
            list.push_back(p);
    }
    return list;
}

bool DataCenterPlugin::insertUnit(LUnit* p)
{
    if (nullptr == p)
        return false;
    if (p->insertDb())
    {
        m_unitList.insert(p->id, p);
        return true;
    }
    return false;
}

LUnit* DataCenterPlugin::getUnit(const int id)
{
    return m_unitList.value(id);
}

bool DataCenterPlugin::hasUnit(const int id)
{
    return m_unitList.contains(id);
}

bool DataCenterPlugin::updateUnit(LUnit* p)
{
    if (nullptr == p)
        return false;
    if (p->updateDb())
    {
        return true;
    }
    return false;
}

bool DataCenterPlugin::removeUnit(LUnit* p)
{
    if (nullptr == p)
        return false;
    //1、删除Chart
    foreach(const auto& c, getChartList(p->id))
    {
        //2、删除点
        foreach(const auto&p, getPointList(c->id))
        {
            removePoint(p);
        }
        removeChart(c);
    }
    //3、删除自己
    if (p->removeDb())
    {
        m_unitList.remove(p->id);
        return true;
    }
    return false;
}

QList<LChart*> DataCenterPlugin::getChartList(const int id)
{
    QList<LChart*> list;
    foreach(const auto& p, m_chartList)
    {
        if (id == p->m_unitId)
            list.push_back(p);

    }
    return list;
}

QList<LPoint*> DataCenterPlugin::getPointList(const int chartId)
{
    QList<LPoint*> list;
    foreach(const auto& p, m_pointList)
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

    //1、删除点
    foreach(const auto&p, getPointList(p->id))
    {
        removePoint(p);
    }
    //3、删除自己
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

bool DataCenterPlugin::execute(LOrder* p)
{
    if (nullptr == p)
        return false;
    QList<IPlugin*> list = Plugin::Manager()->getPluginsByType(IPlugin::Coms);
    foreach (IPlugin *plugin, list)
    {
        IOrder* orderPlugin = qobject_cast<IOrder*>(plugin->instance());
        if (nullptr != orderPlugin)
        {
            if (p->type() == orderPlugin->type())
            {
                return orderPlugin->execute(p);
            }

        }
    }
    return false;
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
