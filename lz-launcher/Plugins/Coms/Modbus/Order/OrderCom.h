#ifndef ORDERCOM_H
#define ORDERCOM_H
#include "LZLib.h"
#include <QModbusDataUnit>

const QString Table_Order_Modbus = "Order_Modbus";
const auto CREATE_SQL_Order_Modbus = QLatin1String(R"(
    create table IF NOT EXISTS %1(
                                    id integer primary key,
                                    type integer            ,
                                    name varchar            ,
                                    mark varchar            ,
                                    value varchar           ,
                                    deviceId integer        ,
                                    registerType varchar    ,
                                    serverAddress integer   ,
                                    startAddress integer    ,
                                    numberOfValues integer
    ))").arg(Table_Order_Modbus);

const auto INSERT_SQL_Order_Modbus = QLatin1String(R"(
    insert into %1(
                    type            ,
                    name            ,
                    mark            ,
                    value           ,
                    deviceId        ,
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
private:
    QMap<QString, QModbusDataUnit::RegisterType> RegisterTypeM = {
        {"DiscreteInputs"       , QModbusDataUnit::DiscreteInputs},
        {"Coils"                , QModbusDataUnit::Coils},
        {"InputRegisters"       , QModbusDataUnit::InputRegisters},
        {"HoldingRegisters"     , QModbusDataUnit::HoldingRegisters},
    };
public:
    OrderCom();

    static bool loadDb();
    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;

    virtual QString registerType() override;
    virtual int serverAddress() override;
    virtual int startAddress() override;
    virtual int numberOfValues() override;
    virtual void setRegisterType(const QString& value) override;
    virtual void setServerAddress(const int value) override;
    virtual void setStartAddress(const int value) override;
    virtual void setNumberOfValues(const int value) override;
private:
    bool bindValue(QSqlQuery& query);

    QString m_registerType;   //功能码
    int     m_serverAddress;  //服务器地址
    int     m_startAddress;   //起始地址
    int     m_numberOfValues; //写入个数
};

#endif // ORDERCOM_H
