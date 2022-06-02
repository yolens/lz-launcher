#include "ModbusData.h"
#include "Order/OrderCom.h"
#include "Device/DeviceCom.h"

ModbusData::ModbusData()
{

}

ModbusData* ModbusData::instance()
{
    static ModbusData ins;
    return &ins;
}

void ModbusData::loadDb()
{
    OrderCom::loadDb();
    DeviceCom::loadDb();
}

bool ModbusData::insert(LOrder *p)
{
    if (nullptr == p)
        return false;
    if (p->insertDb())
    {
        m_orderList.push_back(p);
        return true;
    }
    return false;
}
bool ModbusData::update(LOrder *p)
{
    if (nullptr == p)
        return false;
    if (p->updateDb())
    {
        return true;
    }
    return false;
}
bool ModbusData::remove(LOrder *p)
{
    if (nullptr == p)
        return false;
    if (p->removeDb())
    {
        m_orderList.removeAll(p);
        return true;
    }
    return false;
}

bool ModbusData::insert(LDevice *p)
{
    if (nullptr == p)
        return false;
    if (p->insertDb())
    {
        m_deviceList.push_back(p);
        return true;
    }
    return false;
}
bool ModbusData::update(LDevice *p)
{
    if (nullptr == p)
        return false;
    if (p->updateDb())
    {
        return true;
    }
    return false;
}
bool ModbusData::remove(LDevice *p)
{
    if (nullptr == p)
        return false;
    if (p->removeDb())
    {
        m_deviceList.removeAll(p);
        return true;
    }
    return false;
}

QList<LOrder*>* ModbusData::orderList()
{
    return &m_orderList;
}

void ModbusData::setOrderList(QList<LOrder*>& list)
{
    m_orderList = list;
}

QList<LDevice*>* ModbusData::deviceList()
{
    return &m_deviceList;
}

void ModbusData::setDeviceList(QList<LDevice*>& list)
{
    m_deviceList = list;
}
