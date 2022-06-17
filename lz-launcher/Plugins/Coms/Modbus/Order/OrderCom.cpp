#include "OrderCom.h"
#include <QtSql>
#include "ModbusData.h"

OrderCom::OrderCom()
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

        QSqlQuery query;
        query.exec(CREATE_SQL_Order_Modbus);
        foreach (QString col, ALTER_Order_Modbus_LIST)
        {
            QString cmd = ALTER_SQL_Order_Modbus.arg(col);
            query.exec(cmd);
        }
    }
    this->setType(LOrder::Modbus);
}

bool OrderCom::loadDb()
{
    QList<LOrder*> list;
    QSqlQuery query;
    if (!query.exec(SELECT_SQL_Order_Modbus))
        return false;
    QSqlRecord rec = query.record();
    int id = rec.indexOf("id");
    int type = rec.indexOf("type");
    int name = rec.indexOf("name");
    int mark = rec.indexOf("mark");
    int value = rec.indexOf("value");
    int deviceId = rec.indexOf("deviceId");
    int rwType = rec.indexOf("rwType");
    int byteType = rec.indexOf("byteType");
    int registerType = rec.indexOf("registerType");
    int serverAddress = rec.indexOf("serverAddress");
    int startAddress = rec.indexOf("startAddress");
    int numberOfValues = rec.indexOf("numberOfValues");

    while (query.next())
    {
        OrderCom *info = new OrderCom;
        info->id = query.value(id).toInt();
        info->setType((Type)query.value(type).toInt());
        info->setName(query.value(name).toString());
        info->setMark(query.value(mark).toString());
        info->setValue(query.value(value));
        info->setDeviceId(query.value(deviceId).toInt());
        info->setRWType((RWType)query.value(rwType).toInt());
        info->setByteType((ByteType)query.value(byteType).toInt());
        info->setRegisterType(query.value(registerType).toInt());
        info->setServerAddress(query.value(serverAddress).toInt());
        info->setStartAddress(query.value(startAddress).toInt());
        info->setNumberOfValues(query.value(numberOfValues).toInt());
        list.push_back(info);
    }
    ModbusData::instance()->setOrderList(list);
    return true;
}


bool OrderCom::updateDb()
{
    QSqlQuery query;
    if (!query.prepare(UPDATE_SQL_Order_Modbus))
        return false;
    bindValue(query);
    query.bindValue(":id",this->id);
    if (!query.exec())
        return false;
    return true;
}
bool OrderCom::insertDb()
{
    QSqlQuery query;
    if (!query.prepare(INSERT_SQL_Order_Modbus))
        return false;

    bindValue(query);
    if (!query.exec())
        return false;
    this->id  = query.lastInsertId().toInt();
    return true;
}
bool OrderCom::removeDb()
{
    QSqlQuery query;
    if (!query.prepare(DELETE_SQL_Order_Modbus))
        return false;
    query.bindValue(":id", this->id);
    if (!query.exec())
        return false;
    return true;
}

bool OrderCom::bindValue(QSqlQuery& query)
{
    query.bindValue(":type", this->type());
    query.bindValue(":name", this->name());
    query.bindValue(":mark", this->mark());
    query.bindValue(":value", this->value());
    query.bindValue(":deviceId", this->deviceId());
    query.bindValue(":rwType", this->rwType());
    query.bindValue(":byteType", this->byteType());
    query.bindValue(":registerType", this->registerType());
    query.bindValue(":serverAddress", this->serverAddress());
    query.bindValue(":startAddress", this->startAddress());
    query.bindValue(":numberOfValues", this->numberOfValues());
    return true;
}

int OrderCom::registerType()
{
    return m_registerType;
}
int OrderCom::serverAddress()
{
    return m_serverAddress;
}
int OrderCom::startAddress()
{
    return m_startAddress;
}
int OrderCom::numberOfValues()
{
    return m_numberOfValues;
}

void OrderCom::setRegisterType(const int value)
{
    m_registerType = value;
}
void OrderCom::setServerAddress(const int value)
{
    m_serverAddress = value;
}
void OrderCom::setStartAddress(const int value)
{
    m_startAddress = value;
}
void OrderCom::setNumberOfValues(const int value)
{
    m_numberOfValues = value;
}
