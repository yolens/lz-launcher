#include "LDB.h"
#include <QCoreApplication>
#include <QtSql>



LDB::LDB()
{

}

bool LDB::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbName = QCoreApplication::applicationDirPath() + "/data.db";
    db.setDatabaseName(dbName);

    if (!db.open())
        return false;

    return true;
}

LProduct::LProduct()
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

        QSqlQuery query;
        query.exec(CREATE_SQL_LProduct);
        foreach (QString col, ALTER_LProduct_LIST)
        {
            QString cmd = ALTER_SQL_LProduct.arg(col);
            query.exec(cmd);
        }
    }
}
LProduct::~LProduct()
{

}

QList<LProduct*> LProduct::get()
{
    QList<LProduct*> list;
    QSqlQuery query;
    if (!query.exec(SELECT_SQL_LProduct))
        return list;
    QSqlRecord rec = query.record();
    int id = rec.indexOf("id");
    int name = rec.indexOf("name");
    int mark = rec.indexOf("mark");
    int mainUnitId = rec.indexOf("mainUnitId");

    while (query.next())
    {
        LProduct *info = new LProduct;
        info->id = query.value(id).toInt();
        info->name = query.value(name).toString();
        info->mark = query.value(mark).toString();
        info->mainUnitId = query.value(mainUnitId).toInt();
        list.push_back(info);
    }
    return list;
}

bool LProduct::updateDb()
{
    QSqlQuery query;
    if (!query.prepare(UPDATE_SQL_LProduct))
        return false;
    bindValue(query);
    query.bindValue(":id",this->id);
    if (!query.exec())
        return false;
    return true;
}
bool LProduct::insertDb()
{
    QSqlQuery query;
    if (!query.prepare(INSERT_SQL_LProduct))
        return false;

    bindValue(query);
    if (!query.exec())
        return false;
    this->id  = query.lastInsertId().toInt();
    return true;
}
bool LProduct::removeDb()
{
    QSqlQuery query;
    if (!query.prepare(DELETE_SQL_LProduct))
        return false;
    query.bindValue(":id", this->id);
    if (!query.exec())
        return false;
    return true;
}


bool LProduct::bindValue(QSqlQuery& query)
{
    query.bindValue(":name", this->name);
    query.bindValue(":mark", this->mark);
    query.bindValue(":mainUnitId", this->mainUnitId);
    return true;
}


LUnit::LUnit()
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

        QSqlQuery query;
        query.exec(CREATE_SQL_LUnit);
        foreach (QString col, ALTER_LUnit_LIST)
        {
            QString cmd = ALTER_SQL_LUnit.arg(col);
            query.exec(cmd);
        }
    }
}
LUnit::~LUnit()
{

}

QList<LUnit*> LUnit::get()
{
    QList<LUnit*> list;
    QSqlQuery query;
    if (!query.exec(SELECT_SQL_LUnit))
        return list;
    QSqlRecord rec = query.record();
    int id = rec.indexOf("id");
    int name = rec.indexOf("name");
    int mark = rec.indexOf("mark");
    int productId = rec.indexOf("productId");

    while (query.next())
    {
        LUnit *info = new LUnit;
        info->id = query.value(id).toInt();
        info->name = query.value(name).toString();
        info->mark = query.value(mark).toString();
        info->productId = query.value(productId).toInt();
        list.push_back(info);
    }
    return list;
}

bool LUnit::updateDb()
{
    QSqlQuery query;
    if (!query.prepare(UPDATE_SQL_LUnit))
        return false;
    bindValue(query);
    query.bindValue(":id",this->id);
    if (!query.exec())
        return false;
    return true;
}
bool LUnit::insertDb()
{
    QSqlQuery query;
    if (!query.prepare(INSERT_SQL_LUnit))
        return false;

    bindValue(query);
    if (!query.exec())
        return false;
    this->id  = query.lastInsertId().toInt();
    return true;
}
bool LUnit::removeDb()
{
    QSqlQuery query;
    if (!query.prepare(DELETE_SQL_LUnit))
        return false;
    query.bindValue(":id", this->id);
    if (!query.exec())
        return false;
    return true;
}


bool LUnit::bindValue(QSqlQuery& query)
{
    query.bindValue(":name", this->name);
    query.bindValue(":mark", this->mark);
    query.bindValue(":productId", this->productId);
    return true;
}

LPoint::LPoint()
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

        QSqlQuery query;
        query.exec(CREATE_SQL_LPoint);
        foreach (QString col, ALTER_LPoint_LIST)
        {
            QString cmd = ALTER_SQL_LPoint.arg(col);
            query.exec(cmd);
        }
    }
}

LPoint::~LPoint()
{

}

QList<LPoint*> LPoint::get()
{
    QList<LPoint*> list;
    QSqlQuery query;
    if (!query.exec(SELECT_SQL_LPoint))
        return list;
    QSqlRecord rec = query.record();
    int id = rec.indexOf("id");
    int name = rec.indexOf("name");
    int type = rec.indexOf("type");
    int attribute = rec.indexOf("attribute");
    int max = rec.indexOf("max");
    int chartId = rec.indexOf("chartId");
    int valueId = rec.indexOf("valueId");
    int maxValue = rec.indexOf("maxValue");
    int minValue = rec.indexOf("minValue");

    while (query.next())
    {
        LPoint *info = new LPoint;
        info->id = query.value(id).toInt();
        info->type = (LPType)query.value(type).toInt();
        info->name = query.value(name).toString();
        info->attribute = (LPAttribute)query.value(attribute).toInt();
        info->max = query.value(max).toInt();
        info->chartId = query.value(chartId).toInt();
        info->valueId = query.value(valueId).toString();
        info->maxValue = query.value(maxValue);
        info->minValue = query.value(minValue);

        list.push_back(info);
    }
    return list;
}

bool LPoint::updateDb()
{
    QSqlQuery query;
    if (!query.prepare(UPDATE_SQL_LPoint))
        return false;
    bindValue(query);
    query.bindValue(":id",this->id);
    if (!query.exec())
        return false;
    return true;
}

bool LPoint::insertDb()
{
    QSqlQuery query;
    if (!query.prepare(INSERT_SQL_LPoint))
        return false;

    bindValue(query);
    if (!query.exec())
        return false;
    this->id  = query.lastInsertId().toInt();
    return true;
}

bool LPoint::removeDb()
{
    QSqlQuery query;
    if (!query.prepare(DELETE_SQL_LPoint))
        return false;
    query.bindValue(":id", this->id);
    if (!query.exec())
        return false;
    return true;
}

bool LPoint::bindValue(QSqlQuery& query)
{
    query.bindValue(":name", this->name);
    query.bindValue(":type", this->type);
    query.bindValue(":attribute", this->attribute);
    query.bindValue(":max", this->max);
    query.bindValue(":chartId", this->chartId);
    query.bindValue(":valueId", this->valueId);
    query.bindValue(":maxValue", this->maxValue);
    query.bindValue(":minValue", this->minValue);
    return true;
}


LOrder::LOrder()
{

}

LOrder::~LOrder()
{

}

bool LOrder::updateDb()
{
    return true;
}

bool LOrder::insertDb()
{

    return true;
}

bool LOrder::removeDb()
{
    return true;
}

LDevice::LDevice()
{

}
LDevice::~LDevice()
{

}
bool LDevice::updateDb()
{
    return true;
}

bool LDevice::insertDb()
{

    return true;
}

bool LDevice::removeDb()
{
    return true;
}

#include <QJsonObject>
#include <QJsonArray>
LChart::LChart(const LCType type)
{
    m_type = type;
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;
        QSqlQuery query;
        query.exec(CREATE_SQL_LChart);
        foreach (QString col, ALTER_LChart_LIST)
        {
            QString cmd = ALTER_SQL_LChart.arg(col);
            query.exec(cmd);
        }

    }
}

LChart::~LChart()
{

}

QList<LChart*> LChart::get()
{
    QList<LChart*> list;
    QSqlQuery query;
    if (!query.exec(SELECT_SQL_LChart))
        return list;
    QSqlRecord rec = query.record();
    int id = rec.indexOf("id");
    int type = rec.indexOf("type");
    int x = rec.indexOf("x");
    int y = rec.indexOf("y");
    int width = rec.indexOf("width");
    int height = rec.indexOf("height");
    int sourcePointId = rec.indexOf("sourcePointId");
    int destPointId = rec.indexOf("destPointId");
    int orderId = rec.indexOf("orderId");
    int orderType = rec.indexOf("orderType");
    int unitId = rec.indexOf("unitId");
    int delay = rec.indexOf("delay");
    int value = rec.indexOf("value");
    int name = rec.indexOf("name");

    while (query.next())
    {
        LChart *info = new LChart;
        info->id = query.value(id).toInt();
        info->m_type = (LCType)query.value(type).toInt();
        info->m_pos = QPoint(query.value(x).toInt(), query.value(y).toInt());
        info->m_width = query.value(width).toInt();
        info->m_height = query.value(height).toInt();
        info->m_sourcePointId = query.value(sourcePointId).toInt();
        info->m_destPointId = query.value(destPointId).toInt();
        info->m_orderId = query.value(orderId).toInt();
        info->m_orderType = query.value(orderType).toInt();
        info->m_unitId = query.value(unitId).toInt();
        info->m_delay = query.value(delay).toInt();
        info->m_value = query.value(value);
        info->m_name = query.value(name).toString();
        list.push_back(info);
    }
    return list;
}

bool LChart::updateDb()
{
    QSqlQuery query;
    if (!query.prepare(UPDATE_SQL_LChart))
        return false;
    bindValue(query);
    query.bindValue(":id",this->id);
    if (!query.exec())
        return false;
    return true;
}

bool LChart::insertDb()
{
    QSqlQuery query;
    if (!query.prepare(INSERT_SQL_LChart))
        return false;

    bindValue(query);
    if (!query.exec())
        return false;
    this->id  = query.lastInsertId().toInt();
    return true;
}

bool LChart::removeDb()
{
    QSqlQuery query;
    if (!query.prepare(DELETE_SQL_LChart))
        return false;
    query.bindValue(":id", this->id);
    if (!query.exec())
        return false;
    return true;
}

bool LChart::bindValue(QSqlQuery& query)
{
    query.bindValue(":type", this->m_type);
    query.bindValue(":x", m_pos.x());
    query.bindValue(":y", m_pos.y());
    query.bindValue(":width", m_width);
    query.bindValue(":height", m_height);
    query.bindValue(":sourcePointId", this->m_sourcePointId);
    query.bindValue(":destPointId", this->m_destPointId);
    query.bindValue(":orderId", this->m_orderId);
    query.bindValue(":orderType", this->m_orderType);
    query.bindValue(":unitId", this->m_unitId);
    query.bindValue(":delay", this->m_delay);
    query.bindValue(":value", this->m_value);
    query.bindValue(":name", this->m_name);
    return true;
}
