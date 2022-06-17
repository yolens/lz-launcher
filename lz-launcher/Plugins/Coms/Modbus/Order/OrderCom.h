#ifndef ORDERCOM_H
#define ORDERCOM_H
#include "LZLib.h"
#include <QModbusDataUnit>

const QString Table_Order_Modbus = "Order_Modbus";
const auto CREATE_SQL_Order_Modbus = QLatin1String(R"(
    create table IF NOT EXISTS %1(
              id integer primary key
            , type integer
            , name varchar
            , mark varchar
            , value varchar
            , deviceId integer
            , rwType integer
            , byteType integer
            , registerType integer
            , serverAddress integer
            , startAddress integer
            , numberOfValues integer
    ))").arg(Table_Order_Modbus);

const QVector<QString> ALTER_Order_Modbus_LIST = {
    {"type integer"},
    {"name varchar"},
    {"mark varchar"},
    {"value varchar"},
    {"deviceId integer"},
    {"rwType integer "},
    {"byteType integer"},
    {"registerType integer"},
    {"serverAddress integer"},
    {"startAddress integer"},
    {"numberOfValues integer"},
};
const auto ALTER_SQL_Order_Modbus = QLatin1String(R"(
    alter table %1 add %2
    )").arg(Table_Order_Modbus).arg("%1");

const auto INSERT_SQL_Order_Modbus = QLatin1String(R"(
    insert into %1(
                    type            ,
                    name            ,
                    mark            ,
                    value           ,
                    deviceId        ,
                    rwType          ,
                    byteType        ,
                    registerType    ,
                    serverAddress   ,
                    startAddress    ,
                    numberOfValues
                  )
            values(
                    :type           ,
                    :name           ,
                    :mark           ,
                    :value          ,
                    :deviceId        ,
                    :rwType         ,
                    :byteType         ,
                    :registerType    ,
                    :serverAddress   ,
                    :startAddress    ,
                    :numberOfValues
                   )
    )").arg(Table_Order_Modbus);

const auto UPDATE_SQL_Order_Modbus = QLatin1String(R"(
    update %1 set
                    type=:type                      ,
                    name=:name                      ,
                    mark=:mark                      ,
                    value=:value                    ,
                    deviceId=:deviceId              ,
                    rwType=:rwType                  ,
                    byteType=:byteType              ,
                    registerType=:registerType      ,
                    serverAddress=:serverAddress    ,
                    startAddress=:startAddress      ,
                    numberOfValues=:numberOfValues
              where id=:id
    )").arg(Table_Order_Modbus);
const auto DELETE_SQL_Order_Modbus = QLatin1String(R"(
    delete from %1 where id=:id
    )").arg(Table_Order_Modbus);

const auto SELECT_SQL_Order_Modbus = QLatin1String(R"(
    select * from %1
    )").arg(Table_Order_Modbus);

class OrderCom : public LOrder
{

public:
    OrderCom();

    static bool loadDb();
    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;

    virtual int registerType() override;
    virtual int serverAddress() override;
    virtual int startAddress() override;
    virtual int numberOfValues() override;
    virtual void setRegisterType(const int value) override;
    virtual void setServerAddress(const int value) override;
    virtual void setStartAddress(const int value) override;
    virtual void setNumberOfValues(const int value) override;
private:
    bool bindValue(QSqlQuery& query);

    int     m_registerType = QModbusDataUnit::HoldingRegisters;   //功能码
    int     m_serverAddress = 1;  //服务器地址
    int     m_startAddress = 0;   //起始地址
    int     m_numberOfValues= 2; //写入个数
};


#endif // ORDERCOM_H
