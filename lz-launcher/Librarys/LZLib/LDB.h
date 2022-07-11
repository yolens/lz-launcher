#ifndef LDB_H
#define LDB_H
#include "LZLib_global.h"
#include <QString>
#include <QRect>
#include <QVector>
#include <QVariant>

class QSqlQuery;

const QString ORDER_VALUE_ID = "order";
const QString VIEW_VALUE_ID = "view";

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


//产品信息
const QString Table_LProduct = "LProduct";
class LZLIB_EXPORT LProduct : public LDB
{
public:
    LProduct();
    virtual ~LProduct();

    static QList<LProduct*> get();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;
private:
    bool bindValue(QSqlQuery& query);
public:
    QString         name;
    QString         mark;
    int             mainUnitId = 0;
};
const auto CREATE_SQL_LProduct = QLatin1String(R"(
    create table IF NOT EXISTS %1(
          id integer primary key
        , name varchar
        , mark varchar
        , mainUnitId integer
    ))").arg(Table_LProduct);

const QVector<QString> ALTER_LProduct_LIST = {
    {"name varchar"},
    {"mark varchar"},
    {"mainUnitId integer"},
};
const auto ALTER_SQL_LProduct = QLatin1String(R"(
    alter table %1 add %2
    )").arg(Table_LProduct).arg("%1");

const auto INSERT_SQL_LProduct = QLatin1String(R"(
    insert into %1(
                      name
                    , mark
                    , mainUnitId
                  )
            values(
                      :name
                    , :mark
                    , :mainUnitId
                  )
    )").arg(Table_LProduct);

const auto UPDATE_SQL_LProduct = QLatin1String(R"(
    update %1 set
                      name=:name
                    , mark=:mark
                    , mainUnitId=:mainUnitId
    where id=:id
    )").arg(Table_LProduct);

const auto SELECT_SQL_LProduct = QLatin1String(R"(
    select * from %1
    )").arg(Table_LProduct);

const auto DELETE_SQL_LProduct = QLatin1String(R"(
    delete from %1 where id=:id
    )").arg(Table_LProduct);

const QString Table_LUnit = "LUnit";
class LZLIB_EXPORT LUnit : public LDB
{
public:
    LUnit();
    virtual ~LUnit();

    static QList<LUnit*> get();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;
private:
    bool bindValue(QSqlQuery& query);
public:
    QString         name;
    QString         mark;
    int             productId = 0;
};
const auto CREATE_SQL_LUnit = QLatin1String(R"(
    create table IF NOT EXISTS %1(
          id integer primary key
        , name varchar
        , mark varchar
        , productId integer
    ))").arg(Table_LUnit);

const QVector<QString> ALTER_LUnit_LIST = {
    {"name varchar"},
    {"mark varchar"},
    {"productId integer"},
};
const auto ALTER_SQL_LUnit = QLatin1String(R"(
    alter table %1 add %2
    )").arg(Table_LUnit).arg("%1");

const auto INSERT_SQL_LUnit = QLatin1String(R"(
    insert into %1(
                      name
                    , mark
                    , productId
                  )
            values(
                      :name
                    , :mark
                    , :productId
                  )
    )").arg(Table_LUnit);

const auto UPDATE_SQL_LUnit = QLatin1String(R"(
    update %1 set
                      name=:name
                    , mark=:mark
                    , productId=:productId
    where id=:id
    )").arg(Table_LUnit);

const auto SELECT_SQL_LUnit = QLatin1String(R"(
    select * from %1
    )").arg(Table_LUnit);

const auto DELETE_SQL_LUnit = QLatin1String(R"(
    delete from %1 where id=:id
    )").arg(Table_LUnit);





enum LCType {
    LC_None,
    LC_Node,
    LC_Line,
    LC_Virtual,
    LC_Start,
    LC_Finish,
    LC_Thread,
    LC_Branch,
};


const QString Table_LChart = "LChart";
class LZLIB_EXPORT LChart : public LDB
{
public:

public:
    LChart(const LCType type = LCType::LC_Node);
    virtual ~LChart();

    static QList<LChart*> get();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;
    bool bindValue(QSqlQuery& query);
public:
    LCType          m_type = LCType::LC_Node;
    QPoint          m_pos = QPoint(0,0);          //位置信息
    int             m_width = 0;
    int             m_height = 0;
    int             m_sourcePointId = 0;
    int             m_destPointId = 0;
    int             m_orderId = 0;
    int             m_orderType = 0;
    int             m_unitId = 0;
    QString         m_name;

    int             m_delay = 100; //ms
    QVariant        m_value = QVariant();
};
const auto CREATE_SQL_LChart = QLatin1String(R"(
    create table IF NOT EXISTS %1(
          id integer primary key
        , type integer
        , x integer
        , y integer
        , width integer
        , height integer
        , sourcePointId integer
        , destPointId integer
        , orderId integer
        , orderType integer
        , unitId integer
        , delay integer
        , value varchar
        , name varchar
    ))").arg(Table_LChart);

const QVector<QString> ALTER_LChart_LIST = {
    {"type integer"},
    {"x integer"},
    {"y integer"},
    {"width integer"},
    {"height integer"},
    {"sourcePointId integer"},
    {"destPointId integer"},
    {"orderId integer"},
    {"orderType integer"},
    {"unitId integer"},
    {"delay integer"},
    {"value varchar"},
    {"name varchar"},
};
const auto ALTER_SQL_LChart = QLatin1String(R"(
    alter table %1 add %2
    )").arg(Table_LChart).arg("%1");


const auto INSERT_SQL_LChart = QLatin1String(R"(
    insert into %1(
                      type
                    , x
                    , y
                    , width
                    , height
                    , sourcePointId
                    , destPointId
                    , orderId
                    , orderType
                    , unitId
                    , delay
                    , value
                    , name
                   )
            values(
                      :type
                    , :x
                    , :y
                    , :width
                    , :height
                    , :sourcePointId
                    , :destPointId
                    , :orderId
                    , :orderType
                    , :unitId
                    , :delay
                    , :value
                    , :name
                   )
    )").arg(Table_LChart);
const auto UPDATE_SQL_LChart = QLatin1String(R"(
    update %1 set
                      type=:type
                    , x=:x
                    , y=:y
                    , width=:width
                    , height=:height
                    , sourcePointId=:sourcePointId
                    , destPointId=:destPointId
                    , orderId=:orderId
                    , orderType=:orderType
                    , unitId=:unitId
                    , delay=:delay
                    , value=:value
                    , name=:name
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
    enum RWType {
        Write = 0,
        Read,
    };
    enum ByteType {
        HEX = 0,
        DEC,
        Binary,
        Float,
    };

public:
    LOrder();
    virtual ~LOrder();

    QString valueId() {return ORDER_VALUE_ID;};

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;

    void setType(const Type value) {m_type = value;};
    void setName(const QString& value) {m_name = value;};
    void setMark(const QString& value) {m_mark = value;};
    void setValue(const QVariant& value) {m_value = value;};
    void setDeviceId(const int value){m_deviceId = value;};
    void setRWType(const RWType value){m_rwType = value;};
    void setByteType(const ByteType value){m_byteType = value;};
    Type type() {return m_type;};
    QString& name() {return m_name;};
    QString& mark() {return m_mark;};
    QVariant& value() {return m_value;};
    int deviceId(){return m_deviceId;};
    RWType rwType() {return m_rwType;};
    ByteType byteType() {return m_byteType;};

    virtual int registerType(){return 0;};
    virtual int serverAddress(){return 0;};
    virtual int startAddress(){return 0;};
    virtual int numberOfValues(){return 0;};
    virtual void setRegisterType(const int value){Q_UNUSED(value);};
    virtual void setServerAddress(const int value){Q_UNUSED(value);};
    virtual void setStartAddress(const int value){Q_UNUSED(value);};
    virtual void setNumberOfValues(const int value){Q_UNUSED(value);};

    virtual void execute(){};
    virtual void write(){};

    void setValueCallback(std::function<void(const QVariant value)> cb) {m_valueCallback = cb;};
    std::function<void(const QVariant value)> valueCallback() {return m_valueCallback;};
private:
    Type            m_type;
    QString         m_name;
    QString         m_mark;
    QVariant        m_value;
    int             m_deviceId; //设备id
    RWType          m_rwType = RWType::Write;
    ByteType        m_byteType = ByteType::HEX;

    std::function<void(const QVariant value)> m_valueCallback = nullptr;

};

class LZLIB_EXPORT LDevice : public LDB
{
public:
    enum DeviceState{
        disenabled = 0,
        connectSuc,
        connectFail,
    };
public:
    LDevice();
    virtual ~LDevice();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;


    LOrder::Type type(){return m_type;};
    QString name(){return m_name;};
    QString mark(){return m_mark;};

    void setType(const LOrder::Type value){m_type = value;};
    void setName(const QString& value){m_name = value;};
    void setMark(const QString& value){m_mark = value;};
    virtual QString port(){return QString();};
    virtual QString parity(){return QString();};
    virtual QString baudRate(){return QString();};
    virtual QString dataBits(){return QString();};
    virtual QString stopBits(){return QString();};
    virtual bool useEnable(){return false;};
    virtual void setPort(const QString& value){Q_UNUSED(value);};
    virtual void setParity(const QString& value){Q_UNUSED(value);};
    virtual void setBaudRate(const QString& value){Q_UNUSED(value);};
    virtual void setDataBits(const QString& value){Q_UNUSED(value);};
    virtual void setStopBits(const QString& value){Q_UNUSED(value);};
    virtual void setUseEnable(const bool value){Q_UNUSED(value);};

    virtual void execute(LOrder* order) = 0;
    virtual DeviceState deviceState() = 0;
private:
    LOrder::Type    m_type;
    QString         m_name;
    QString         m_mark;

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
    virtual ~LPoint();

    static QList<LPoint*> get();

    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;
private:
    bool bindValue(QSqlQuery& query);
public:
    QString         name;
    LPType          type;
    LPAttribute     attribute;
    int             max = 1;
    int             chartId = 0; //父亲容器
    QString         valueId; //对应值的入口
    QVariant        maxValue = 0;
    QVariant        minValue = 0;

    QRect           rect;
    int             count = 0;
    QVariant        outValue;
    LOrder::ByteType outByteType;
};
const auto CREATE_SQL_LPoint = QLatin1String(R"(
    create table IF NOT EXISTS %1(
          id integer primary key
        , name varchar
        , type integer
        , attribute integer
        , max integer
        , chartId integer
        , valueId varchar
        , maxValue varchar
        , minValue varchar
    ))").arg(Table_LPoint);

const QVector<QString> ALTER_LPoint_LIST = {
    {"name varchar"},
    {"type integer"},
    {"attribute integer"},
    {"max integer"},
    {"chartId integer"},
    {"valueId varchar"},
    {"maxValue varchar"},
    {"minValue varchar"},
};
const auto ALTER_SQL_LPoint = QLatin1String(R"(
    alter table %1 add %2
    )").arg(Table_LPoint).arg("%1");

const auto UPDATE_SQL_LPoint = QLatin1String(R"(
    update %1 set
                      name=:name
                    , type=:type
                    , attribute=:attribute
                    , max=:max
                    , chartId=:chartId
                    , valueId=:valueId
                    , maxValue=:maxValue
                    , minValue=:minValue
    where id=:id
    )").arg(Table_LPoint);

const auto INSERT_SQL_LPoint = QLatin1String(R"(
    insert into %1(
                      name
                    , type
                    , attribute
                    , max
                    , chartId
                    , valueId
                    , maxValue
                    , minValue
                  )
            values(
                      :name
                    , :type
                    , :attribute
                    , :max
                    , :chartId
                    , :valueId
                    , :maxValue
                    , :minValue
                  )
    )").arg(Table_LPoint);

const auto SELECT_SQL_LPoint = QLatin1String(R"(
    select * from %1
    )").arg(Table_LPoint);

const auto DELETE_SQL_LPoint = QLatin1String(R"(
    delete from %1 where id=:id
    )").arg(Table_LPoint);

#endif // LDB_H
