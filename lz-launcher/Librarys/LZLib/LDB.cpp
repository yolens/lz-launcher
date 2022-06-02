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


LPoint::LPoint()
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

        QSqlQuery query;
        query.exec(CREATE_SQL_LPoint);
    }
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

    while (query.next())
    {
        LPoint *info = new LPoint;
        info->id = query.value(id).toInt();
        info->type = (LPType)query.value(type).toInt();
        info->name = query.value(name).toString();
        info->attribute = (LPAttribute)query.value(attribute).toInt();
        info->max = query.value(max).toInt();
        info->chartId = query.value(chartId).toInt();
        list.push_back(info);
    }
    return list;
}

bool LPoint::updateDb()
{
    return true;
}

bool LPoint::insertDb()
{
    QSqlQuery query;
    if (!query.prepare(INSERT_SQL_LPoint))
        return false;

    query.bindValue(":name", this->name);
    query.bindValue(":type", this->type);
    query.bindValue(":attribute", this->attribute);
    query.bindValue(":max", this->max);
    query.bindValue(":chartId", this->chartId);
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
    }
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
    int sourcePointId = rec.indexOf("sourcePointId");
    int destPointId = rec.indexOf("destPointId");

    while (query.next())
    {
        LChart *info = new LChart;
        info->id = query.value(id).toInt();
        info->m_type = (LCType)query.value(type).toInt();
        info->m_pos = QPoint(query.value(x).toInt(), query.value(y).toInt());
        info->m_sourcePointId = query.value(sourcePointId).toInt();
        info->m_destPointId = query.value(destPointId).toInt();
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
    query.bindValue(":sourcePointId", this->m_sourcePointId);
    query.bindValue(":destPointId", this->m_destPointId);
    return true;
}
