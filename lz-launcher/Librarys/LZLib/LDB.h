#ifndef LDB_H
#define LDB_H
#include "LZLib_global.h"
#include <QString>
#include <QRect>
#include <QVector>

class LZLIB_EXPORT LDB
{
public:
    LDB();

    static bool init();

    virtual bool updateDb() = 0;
    virtual bool insertDb() = 0;
    virtual bool removeDb() = 0;

public:
    int id = 0;
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
const auto CREATE_SQL_LPoint = QLatin1String(R"(
    create table IF NOT EXISTS %1(
          id integer primary key
        , name varchar
        , type integer
        , attribute integer
        , max integer
        , chartId integer
    ))").arg(Table_LPoint);

const auto INSERT_SQL_LPoint = QLatin1String(R"(
    insert into %1(name, type, attribute, max, chartId)
            values(:name, :type, :attribute, :max, :chartId)
    )").arg(Table_LPoint);

const auto SELECT_SQL_LPoint = QLatin1String(R"(
    select * from %1
    )").arg(Table_LPoint);

const auto DELETE_SQL_LPoint = QLatin1String(R"(
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
const auto CREATE_SQL_LChart = QLatin1String(R"(
    create table IF NOT EXISTS %1(
          id integer primary key
        , type integer
        , x integer
        , y integer
        , sourcePointId integer
        , destPointId integer
    ))").arg(Table_LChart);

const auto INSERT_SQL_LChart = QLatin1String(R"(
    insert into %1(type, x, y, sourcePointId, destPointId)
            values(:type, :x, :y, :sourcePointId, :destPointId)
    )").arg(Table_LChart);
const auto UPDATE_SQL_LChart = QLatin1String(R"(
    update %1 set type=:type
                , x=:x
                , y=:y
                , sourcePointId=:sourcePointId
                , destPointId=:destPointId
    where id=:id
    )").arg(Table_LChart);
const auto DELETE_SQL_LChart = QLatin1String(R"(
    delete from %1 where id=:id
    )").arg(Table_LChart);

const auto SELECT_SQL_LChart = QLatin1String(R"(
    select * from %1
    )").arg(Table_LChart);


#include <QVariant>
class LZLIB_EXPORT LOrder : public LDB
{
public:
    enum Type {
        Modbus = 0,
    };
public:
    LOrder();
    virtual ~LOrder();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;

    void setType(const Type value) {m_type = value;};
    void setName(const QString& value) {m_name = value;};
    void setMark(const QString& value) {m_mark = value;};
    void setValue(const QVariant& value) {m_value = value;};
    void setDeviceId(const int value){m_deviceId = value;};
    Type type() {return m_type;};
    QString& name() {return m_name;};
    QString& mark() {return m_mark;};
    QVariant& value() {return m_value;};
    int deviceId(){return 0;};

    virtual QString registerType(){return "";};
    virtual int serverAddress(){return 0;};
    virtual int startAddress(){return 0;};
    virtual int numberOfValues(){return 0;};
    virtual void setRegisterType(const QString& value){};
    virtual void setServerAddress(const int value){};
    virtual void setStartAddress(const int value){};
    virtual void setNumberOfValues(const int value){};

    virtual void execute(){};
    virtual void write(){};
private:
    Type            m_type;
    QString         m_name;
    QString         m_mark;
    QVariant        m_value;
    int             m_deviceId; //设备id
};

class LZLIB_EXPORT LDevice : public LDB
{
public:
    enum Type {
        Modbus = 0,
    };
public:
    LDevice();
    virtual ~LDevice();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;


    Type type(){return m_type;};
    QString name(){return m_name;};
    QString mark(){return m_mark;};

    void setType(const Type value){m_type = value;};
    void setName(const QString& value){m_name = value;};
    void setMark(const QString& value){m_mark = value;};
    virtual QString port(){return QString();};
    virtual QString parity(){return QString();};
    virtual QString baudRate(){return QString();};
    virtual QString dataBits(){return QString();};
    virtual QString stopBits(){return QString();};
    virtual void setPort(const QString& value){};
    virtual void setParity(const QString& value){};
    virtual void setBaudRate(const QString& value){};
    virtual void setDataBits(const QString& value){};
    virtual void setStopBits(const QString& value){};
private:
    Type            m_type;
    QString         m_name;
    QString         m_mark;

};

#endif // LDB_H
