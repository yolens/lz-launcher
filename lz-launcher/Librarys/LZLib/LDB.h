#ifndef LDB_H
#define LDB_H
#include "LZLib_global.h"
#include <QString>
#include <QRect>
#include <QVector>


class LZLIB_EXPORT LCom
{

public:
    explicit LCom(){};
    ~LCom(){};

public:
    int id;
    QString name;
    QString mark;
};

class LZLIB_EXPORT LDB
{
public:
    LDB();

    static bool init();

    virtual bool updateDb() = 0;
    virtual bool insertDb() = 0;
    virtual bool removeDb() = 0;

public:
    int m_id = 0;
};



//流程图点信息
enum LPType {
    circuit,  //流程线
    value,    //值传递
};
enum LPAttribute {
    input,
    output
};
const QString Table_LPoint = "LPoint";
class LZLIB_EXPORT LPoint : public LDB
{
public:
    LPoint();

    static QList<LPoint*> get();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;

public:
    QString         name;
    LPType          type;
    LPAttribute     attribute;
    int             max = 1;
    int             chartId = 0; //父亲容器

    QRect           rect;
    int             count = 0;
};
const auto CREATE_ORDER_SQL = QLatin1String(R"(
    create table IF NOT EXISTS %1(
          id integer primary key
        , name varchar
        , type integer
        , attribute integer
        , max integer
        , chartId integer
    ))").arg(Table_LPoint);

const auto INSERT_ORDER_SQL = QLatin1String(R"(
    insert into %1(name, type, attribute, max, chartId)
            values(:name, :type, :attribute, :max, :chartId)
    )").arg(Table_LPoint);

const auto SELECT_ORDER_SQL = QLatin1String(R"(
    select * from %1
    )").arg(Table_LPoint);

const auto DELETE_ORDER_SQL = QLatin1String(R"(
    delete from %1 where id=:id
    )").arg(Table_LPoint);


enum LCType {
    LC_None,
    LC_Node,
    LC_Line,
    LC_Virtual,
    LC_Start,
    LC_Finish,
};

class QSqlQuery;
const QString Table_LChart = "LChart";
class LZLIB_EXPORT LChart : public LDB
{
public:
    LChart(const LCType type = LCType::LC_Node);

    static QList<LChart*> get();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;
    bool bindValue(QSqlQuery& query);
public:
    LCType          m_type = LCType::LC_Node;
    QPoint          m_pos = QPoint(0,0);          //位置信息
    int             m_sourcePointId = 0;
    int             m_destPointId = 0;
};
const auto CREATE_ORDER_SQL_LChart = QLatin1String(R"(
    create table IF NOT EXISTS %1(
          id integer primary key
        , type integer
        , x integer
        , y integer
        , sourcePointId integer
        , destPointId integer
    ))").arg(Table_LChart);

const auto INSERT_ORDER_SQL_LChart = QLatin1String(R"(
    insert into %1(type, x, y, sourcePointId, destPointId)
            values(:type, :x, :y, :sourcePointId, :destPointId)
    )").arg(Table_LChart);
const auto UPDATE_ORDER_SQL_LChart = QLatin1String(R"(
    update %1 set type=:type
                , x=:x
                , y=:y
                , sourcePointId=:sourcePointId
                , destPointId=:destPointId
    where id=:id
    )").arg(Table_LChart);
const auto DELETE_ORDER_SQL_LChart = QLatin1String(R"(
    delete from %1 where id=:id
    )").arg(Table_LChart);

const auto SELECT_ORDER_SQL_LChart = QLatin1String(R"(
    select * from %1
    )").arg(Table_LChart);


class LZLIB_EXPORT LOrder : public LDB
{
public:
    LOrder();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;


public:
    QString         name;
    QString         mark;

};



#endif // LDB_H
